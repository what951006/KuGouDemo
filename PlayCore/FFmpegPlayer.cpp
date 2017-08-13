#include "FFmpegPlayer.h"



int g_nVol=100;

QMutex g_Amutex;
QMutex g_Vmutex;
static bool g_isQuit=false; //清空了
static FFmpegPlayer *g_player=NULL;

// 包队列初始化
void packet_queue_init(PacketQueue* q)
{
    memset(q, 0, sizeof(PacketQueue));
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

// 放入packet到队列中，不带头指针的队列
int packet_queue_put(PacketQueue*q, AVPacket *pkt)
{
    AVPacketList *pktl;
    if (av_dup_packet(pkt) < 0)
        return -1;

    pktl = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pktl)
        return -1;

    pktl->pkt = *pkt;
    pktl->next = nullptr;

    SDL_LockMutex(q->mutex);

    if (!q->last_pkt) // 队列为空，新插入元素为第一个元素
        q->first_pkt = pktl;
    else // 插入队尾
        q->last_pkt->next = pktl;

    q->last_pkt = pktl;

    q->nb_packets++;
    q->size += pkt->size;

    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
    return 0;
}

// 从队列中取出packet
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;)
    {
        if (g_isQuit)
         return -1;
        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt) {
                q->last_pkt = NULL;
            }
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else
        {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

void packet_queue_flush(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);
    AVPacketList *pkt, *pkt1;
    for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1)
    {
        pkt1 = pkt->next;
        if(pkt1->pkt.data != (uint8_t *)"FLUSH")
        {
            ;
        }
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);

    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
    SDL_UnlockMutex(q->mutex);
}
//////////////////////////////////////////////
// 解码音频数据

int audio_decode_frame(mediaState* MS, uint8_t* audio_buf, int buf_size)
{
    static AVFrame *pframe=NULL;        //一帧
    static SwrContext*pSwr_ctx=NULL;    //转换
    AVPacket packet;                    //包

    int decodeLen=0;
    int data_size = 0;
    int got_frame = 0;
    int audio_pkt_size=0;
    uint8_t* audio_pkt_data=NULL;

    if(pframe)
       av_frame_free(&pframe);
    pframe=av_frame_alloc();

    while (true)
    {
        if (packet_queue_get(&MS->audioq, &packet, 0) < 0)
        {
            return -1;
        }
        audio_pkt_data =packet.data;
        audio_pkt_size = packet.size;

        if (packet.pts != AV_NOPTS_VALUE)
        {
               MS->audio_clock = (double)av_q2d(MS->aStream->time_base) * (double)packet.pts;
        }

        while (audio_pkt_size > 0)
        {
            decodeLen = avcodec_decode_audio4(MS->acct, pframe, &got_frame, &packet);
            if (decodeLen < 0) // 出错，跳过
                break;

            audio_pkt_data += decodeLen;
            audio_pkt_size -= decodeLen;

            if (got_frame)
                data_size = av_samples_get_buffer_size(NULL, MS->acct->channels,pframe->nb_samples, MS->acct->sample_fmt, 0);

            if (pframe->channels > 0 && pframe->channel_layout == 0)
                pframe->channel_layout = av_get_default_channel_layout(pframe->channels);
            else if (pframe->channels == 0 && pframe->channel_layout > 0)
                pframe->channels = av_get_channel_layout_nb_channels(pframe->channel_layout);


            if (pSwr_ctx)
                swr_free(&pSwr_ctx);
            pSwr_ctx = swr_alloc_set_opts(nullptr, MS->wanted_frame->channel_layout,
                                         (AVSampleFormat)MS->wanted_frame->format,
                                         MS->wanted_frame->sample_rate,
                                         pframe->channel_layout,
                                         (AVSampleFormat)pframe->format,
                                         pframe->sample_rate, 0, nullptr);
            swr_init(pSwr_ctx);

            int dst_nb_samples = av_rescale_rnd(swr_get_delay(pSwr_ctx, pframe->sample_rate) + pframe->nb_samples, pframe->sample_rate, pframe->sample_rate, AVRounding(1));
            int len2 = swr_convert(pSwr_ctx, &audio_buf, dst_nb_samples,(const uint8_t**)pframe->data, pframe->nb_samples);
            if (len2 < 0)
                break;

            int resampled_data_size = len2 * MS->wanted_frame->channels* av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);
            int n = 2 * MS->acct->channels;
            MS->audio_clock += (double)resampled_data_size/(double)(n * MS->acct->sample_rate);

            av_free_packet(&packet);
            return MS->wanted_frame->channels * len2 * av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);
        }
    }
    return -1;
}

/**
  @brief:每回调一次该方法，把len的长度全部读完！
*/

void audio_callback(void* userdata, Uint8* stream, int len)
{

    mediaState* MS = (mediaState*)userdata;
    uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE];
    memset(stream, 0, len);
    static int audio_buf_pos=0;
    static int audio_buf_size=0;
  //使用静态变量的原因是audio_buf_size一次数据长度为4608,mp3哈，回调的len又太小，不能让它每执行audio_callback函数就来执行audio_decode_frame解码函数

    while(len>0) //要读len这么长的数据长能退出
    {
        if(audio_buf_pos>=audio_buf_size)//如果完成
        {
            audio_buf_size = audio_decode_frame(MS, audio_buff, sizeof(audio_buff));
            if (audio_buf_size < 0)
                return;
            audio_buf_pos=0;
        }

       int audio_len= audio_buf_size-audio_buf_pos;
       if(audio_len>len)
           audio_len=len;


        SDL_MixAudio(stream, audio_buff+audio_buf_pos, audio_len, g_nVol);
        len-=audio_len;
        audio_buf_pos+=audio_len;
        stream+=audio_len;
     }
}


static double synchronize_video(mediaState *MS, AVFrame *src_frame, double pts)//用于音视频同步
{

    double frame_delay;

    if (pts != 0)
    {
        /* if we have pts, set video clock to it */
        MS->video_clock = pts;
    } else
    {
        /* if we aren't given a pts, set it to the clock */
        pts = MS->video_clock;
    }
    /* update the video clock */
    frame_delay = av_q2d(MS->vcct->time_base);
    /* if we are repeating a frame, adjust clock accordingly */
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
    MS->video_clock += frame_delay;
    return pts;
}

int video_thread(void *arg)
{
    g_Vmutex.lock();
    mediaState *is = (mediaState *) arg;
    AVPacket pkt1, *packet = &pkt1;

    int ret, got_picture, numBytes;

    double video_pts = 0; //当前视频的pts
    double audio_pts = 0; //音频pts


    ///解码视频相关
    AVFrame *pFrame, *pFrameRGB;
    uint8_t *out_buffer_rgb; //解码后的rgb数据
    struct SwsContext *img_convert_ctx;  //用于解码后的视频格式转换

    AVCodecContext *pCodecCtx = is->vcct; //视频解码器

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    ///这里我们改成了 将解码后的YUV数据转换成RGB32
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
            AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    out_buffer_rgb = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer_rgb, AV_PIX_FMT_RGB32,
            pCodecCtx->width, pCodecCtx->height);

    while(!g_isQuit)
    {
        if (SDL_AUDIO_PAUSED == SDL_GetAudioStatus()) //判断暂停
        {
            SDL_Delay(1);
            continue;
        }
        if (packet_queue_get(&is->videoq, packet, 0) <= 0) //非block
        {
            SDL_Delay(1); //队列只是暂时没有数据而已
            continue;
        }

        //收到这个数据 说明刚刚执行过跳转 现在需要把解码器的数据 清除一下
        if(strcmp((char*)packet->data,FLUSH_DATA) == 0)
        {
            avcodec_flush_buffers(is->vStream->codec);
            av_free_packet(packet);
            continue;
        }

        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

        if (ret < 0) {
            qDebug()<<"decode error.\n";
            av_free_packet(packet);
            continue;
        }
//音视频同步
        if (packet->dts == AV_NOPTS_VALUE && pFrame->opaque&& *(uint64_t*) pFrame->opaque != AV_NOPTS_VALUE)
        {
            video_pts = *(uint64_t *) pFrame->opaque;
        }
        else if (packet->dts != AV_NOPTS_VALUE)
        {
            video_pts = packet->dts;
        }
        else
        {
            video_pts = 0;
        }

        video_pts *= av_q2d(is->vStream->time_base);
        video_pts = synchronize_video(is, pFrame, video_pts);

      /*  if (is->seek_req)
        {
            //发生了跳转 则跳过关键帧到目的时间的这几帧
           if (video_pts < is->seek_pos)
           {
               av_free_packet(packet);
               continue;
           }
           else
           {
               is->seek_req = 0;
           }
        }*/

        while(!g_isQuit)
        {
            audio_pts = is->audio_clock;

            //主要是 跳转的时候 我们把video_clock设置成0了
            //因此这里需要更新video_pts
            //否则当从后面跳转到前面的时候 会卡在这里
            video_pts = is->video_clock;
            if (video_pts <= audio_pts)
                break;
            int delayTime = (video_pts - audio_pts) * 1000;
            delayTime = delayTime > 5 ? 5:delayTime;
            SDL_Delay(delayTime);
        }
//同步结束

        if (got_picture)
        {
            sws_scale(img_convert_ctx,
                    (uint8_t const * const *) pFrame->data,
                    pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                    pFrameRGB->linesize);

            //把这个RGB数据 用QImage加载
            QImage tmpImg((uchar *)out_buffer_rgb,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
            QImage image = tmpImg.copy(); //把图像复制一份 传递给界面显示
            emit g_player->sig_CurImageChange(image); //调用激发信号的函数
        }
        av_free_packet(packet);
    }

    av_free(pFrame);
    av_free(pFrameRGB);
    av_free(out_buffer_rgb);
    emit g_player->sig_CurImageChange(QImage()); //刷新下MV背景

    g_player->FreeVideoAlloc();
    g_Vmutex.unlock();
    return 0;
}

FFmpegPlayer::FFmpegPlayer(QObject *parent) : QThread(parent)
{
    g_player=this;
    SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_EVENTS);
    av_register_all();
    avformat_network_init();
    memset(&m_MS,0,sizeof(m_MS));

    m_timer=new QTimer;
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timerWork()));
    m_timer->start(50);
}

void FFmpegPlayer::setMedia(const QString &url,bool isMV)
{
    stop();
    emit sig_CurrentMediaChange(url,isMV);

    m_url=url;
    start(HighestPriority);
    updateStatus();
}
inline void FFmpegPlayer::FreeCommSpace()
{
    avformat_close_input(&m_MS.fct);
    memset(&m_MS,0,sizeof(m_MS));
}
inline void FFmpegPlayer::FreeVideoAlloc()
{
    avcodec_close(m_MS.vcct);
    packet_queue_flush(&m_MS.videoq);
}
void FFmpegPlayer::FreeAudioAlloc()
{
     avcodec_close(m_MS.acct);
     av_frame_free(&m_MS.wanted_frame);
     packet_queue_flush(&m_MS.audioq);
}

void FFmpegPlayer::seek(qint64 pos)
{
    if(m_MS.seek_req)
        return;

    m_MS.seek_req=1;
    m_MS.seek_pos=pos;
}


void FFmpegPlayer::stop()
{
    g_isQuit=1;
    g_Amutex.lock();
    SDL_CloseAudio();//Close SDL
    g_Amutex.unlock();


    g_Vmutex.lock();
    g_Vmutex.unlock();
    FreeCommSpace();
    updateStatus();
}


PlayerStatus FFmpegPlayer::getPlayerStatus()
{
    if(SDL_AUDIO_PAUSED==SDL_GetAudioStatus())
        return pausingStatus;
    if(SDL_AUDIO_PLAYING==SDL_GetAudioStatus())
        return playingStatus;
    return stopStatus;
}


void FFmpegPlayer::run()
{

    g_Amutex.lock();
    g_isQuit=0;
    // 读取文件头，将格式相关信息存放在AVFormatContext结构体中
    if (avformat_open_input(&m_MS.fct, m_url.toUtf8().data(), nullptr, nullptr) != 0)
    {
        qDebug()<<"open error";
        FreeCommSpace();
        g_Amutex.unlock();
        return; // 打开失败
    }
    // 检测文件的流信息
    if (avformat_find_stream_info(m_MS.fct, nullptr) < 0)
    {
        FreeCommSpace();
        g_Amutex.unlock();
        return; // 没有检测到流信息 stream infomation
    }

    //查找第一个视频流 video stream
    int audioStream = -1;
    int videoStream= -1;
    for (unsigned int i = 0; i < m_MS.fct->nb_streams; ++i)
    {
        if (AVMEDIA_TYPE_AUDIO == m_MS.fct->streams[i]->codec->codec_type)
        {
            audioStream = i;
            m_MS.aStream= m_MS.fct->streams[i];
            m_MS.acct = m_MS.fct->streams[i]->codec; // codec context
            continue;
        }
        else if (AVMEDIA_TYPE_VIDEO == m_MS.fct->streams[i]->codec->codec_type)
        {
            videoStream = i;
            m_MS.vStream= m_MS.fct->streams[i];
            m_MS.vcct = m_MS.fct->streams[i]->codec; // codec context
            continue;
        }
    }

    // 3. 根据读取到的流信息查找相应的解码器并打开
    if (-1==audioStream && -1==videoStream)
    {
        qDebug()<<"find no stream";
        FreeCommSpace();
        g_Amutex.unlock();
        return; // 没有检测到流信息 stream infomation
    }

    if(-1!=audioStream)
    {
        AVCodec* pACodec = avcodec_find_decoder(m_MS.acct->codec_id);
        if(pACodec)
            avcodec_open2(m_MS.acct, pACodec, nullptr);
        else
        {
            qDebug()<<"find audio decoder error";
            FreeCommSpace();
            g_Amutex.unlock();
            return;
        }
    }

    if(-1!=videoStream)
    {
        AVCodec* pVCodec = avcodec_find_decoder(m_MS.vcct->codec_id);
        if(pVCodec)
        {
            avcodec_open2(m_MS.vcct, pVCodec, nullptr);
            CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(video_thread),&m_MS,NULL,NULL);
        }
        else
        {
            qDebug()<<"find video decoder error";
            FreeCommSpace();
            g_Amutex.unlock();
            return;
        }
    }

    // Set audio settings from codec info
    SDL_AudioSpec wanted_spec, spec;
    wanted_spec.freq = m_MS.acct->sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = m_MS.acct->channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = &m_MS;
   // CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
    {
        qDebug() << "Open audio failed:" << SDL_GetError() << endl;
        FreeCommSpace();
        g_Amutex.unlock();
        return ;
    }
    m_MS.wanted_frame=av_frame_alloc();
    m_MS.wanted_frame->format = AV_SAMPLE_FMT_S16;
    m_MS.wanted_frame->sample_rate = spec.freq;
    m_MS.wanted_frame->channel_layout = av_get_default_channel_layout(spec.channels);
    m_MS.wanted_frame->channels = spec.channels;

    SDL_PauseAudio(0);
/*---------------test----------------------*/


/*---------------test----------------------*/
    AVPacket packet;
    while (!g_isQuit) //这里有一个顺序！先判断再 读 再写入
    {

        if (m_MS.seek_req)
        {
                int stream_index = -1;

                if (videoStream !=-1)
                    stream_index = videoStream;
                if (audioStream!=-1)
                    stream_index =audioStream;


                AVRational aVRational = {1, AV_TIME_BASE};
                if (stream_index >= 0)
                {
                    m_MS.seek_pos = av_rescale_q(m_MS.seek_pos, aVRational,
                                    m_MS.fct->streams[stream_index]->time_base);
                }

                SDL_PauseAudio(1);
//block here
                if (av_seek_frame(m_MS.fct, stream_index, m_MS.seek_pos, AVSEEK_FLAG_ANY) < 0)
                {
                    qDebug()<<"seek error";
                }
                else
                {
                    AVPacket packet; //分配一个packet
                    av_new_packet(&packet, 10);
                    strcpy((char*)packet.data,FLUSH_DATA);

                    qDebug()<<"seek 成功";
                    if (audioStream!=-1) //audio
                    {
                        packet_queue_flush(&m_MS.audioq); //清除队列
                        packet_queue_put(&m_MS.audioq, &packet); //往队列中存入用来清除的包
                    }
                    if (videoStream!=-1)
                    {

                        packet_queue_flush(&m_MS.videoq); //清除队列
                        packet_queue_put(&m_MS.videoq, &packet); //往队列中存入用来清除的包
                        m_MS.video_clock = 0;
                    }
                }
                SDL_PauseAudio(0);
                m_MS.seek_req = 0;
        }

        SDL_Delay(10);

        if(m_MS.videoq.size>MAX_VIDEO_SIZE)
            continue;

        if (m_MS.audioq.size > MAX_AUDIO_SIZE)
            continue; //这个才是关键！

        int result=av_read_frame(m_MS.fct, &packet);
       if(0==result)
       {
            if (packet.stream_index == audioStream)
                packet_queue_put(&m_MS.audioq, &packet);//添加到队列中去
            else if(packet.stream_index == videoStream)
                packet_queue_put(&m_MS.videoq, &packet);//添加到队列中去
            else
                av_free_packet(&packet);
       }
       else if(result<0&& 0==m_MS.audioq.size)
       {
         //  g_isQuit=1;
           break;
       }
    }
    if(!g_isQuit) //It finished automatically when played to end of the media
        emit sig_CurrentMediaFinished();

    FreeAudioAlloc();
    g_Amutex.unlock();
}
