#include "FFmpegPlayer.h"
#include<QDebug>
#include<windows.h>
#include<QTime>
#include<QImage>
#include<Global_ValueGather.h>
#define USE_MUTE 1

static bool isquit=false; //清空了
int VOL=80;
// 包队列初始化
void packet_queue_init(PacketQueue* q)
{
    q->last_pkt = NULL;
    q->first_pkt = NULL;
#if USE_MUTE
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
#endif
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
#if USE_MUTE
    SDL_LockMutex(q->mutex);
#endif
    if (!q->last_pkt) // 队列为空，新插入元素为第一个元素
        q->first_pkt = pktl;
    else // 插入队尾
        q->last_pkt->next = pktl;

    q->last_pkt = pktl;

    q->nb_packets++;
    q->size += pkt->size;
#if USE_MUTE
    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
#endif
    return 0;
}

// 从队列中取出packet
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block) {
    AVPacketList *pkt1;
    int ret;
#if USE_MUTE
    SDL_LockMutex(q->mutex);
#endif
    for (;;)
    {
        if (isquit)
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
        } else {
#if USE_MUTE
            SDL_CondWait(q->cond, q->mutex);
#endif
        }
    }
#if USE_MUTE
    SDL_UnlockMutex(q->mutex);
#endif
    return ret;
}

void packet_queue_flush(PacketQueue *q)
{
#if USE_MUTE
    SDL_LockMutex(q->mutex);
#endif
    AVPacketList *pkt, *pkt1;
    for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1)
    {
        pkt1 = pkt->next;
        if(pkt1->pkt.data != (uint8_t *)"FLUSH")
        {

        }
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);

    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
#if USE_MUTE
    SDL_UnlockMutex(q->mutex);
#endif
}
//////////////////////////////////////////////解码音频数据

int audio_decode_frame(mediaState* MS, uint8_t* audio_buf, int buf_size)
{
    int len1;
    int data_size = 0;
    while (true)
    {
        if (isquit)
            return -1;
        while (MS->audio_pkt_size > 0)
        {
            int got_frame = 0;
            len1 = avcodec_decode_audio4(MS->acct, MS->frame, &got_frame, &MS->pkt);
            if (len1 < 0) // 出错，跳过
            {
                MS->audio_pkt_size = 0;
                break;
            }

            MS->audio_pkt_data += len1;
            MS->audio_pkt_size -= len1;
            data_size = 0;
            if (got_frame)
                data_size = av_samples_get_buffer_size(nullptr, MS->acct->channels,MS-> frame->nb_samples, MS->acct->sample_fmt, 1);
            if (MS->frame->channels > 0 && MS->frame->channel_layout == 0)
                MS->frame->channel_layout = av_get_default_channel_layout(MS->frame->channels);
            else if (MS->frame->channels == 0 && MS->frame->channel_layout > 0)
                MS->frame->channels = av_get_channel_layout_nb_channels(MS->frame->channel_layout);
            if (MS->swr_ctx)
            {
                swr_free(&MS->swr_ctx);
                MS->swr_ctx = nullptr;
            }
            MS->swr_ctx = swr_alloc_set_opts(nullptr, MS->wanted_frame->channel_layout,
                                         (AVSampleFormat)MS->wanted_frame->format,
                                         MS->wanted_frame->sample_rate,
                                         MS->frame->channel_layout,
                                         (AVSampleFormat)MS->frame->format,
                                         MS->frame->sample_rate, 0, nullptr);

            if (!MS->swr_ctx || swr_init(MS->swr_ctx) < 0)
            {
                qDebug() << "swr_init failed:" << endl;
                break;
            }
            int dst_nb_samples = av_rescale_rnd(swr_get_delay(MS->swr_ctx, MS->frame->sample_rate) + MS->frame->nb_samples, MS->frame->sample_rate, MS->frame->sample_rate, AVRounding(1));
            int len2 = swr_convert(MS->swr_ctx, &audio_buf, dst_nb_samples,(const uint8_t**)MS->frame->data, MS->frame->nb_samples);//这个才是最重要的~前面所做的工作都是为这个
            if (len2 < 0)
            {
                qDebug() << "swr_convert failed\n";
                break;
            }
//[][]相当重要的一步，转换成时间
            int resampled_data_size = len2 * MS->wanted_frame->channels* av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);
            int n = 2 * MS->audio_st->codec->channels;
            MS->audio_clock += (double)resampled_data_size/(double)(n * MS->audio_st->codec->sample_rate);
//[][]
            return MS->wanted_frame->channels * len2 * av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);
        }
        if (MS->pkt.buf)
            av_free_packet(&MS->pkt);


        if (packet_queue_get(&MS->audioq,&MS->pkt,0)<=0)
        {
            return -1;
        }
        if (MS->pkt.pts != AV_NOPTS_VALUE)
        {
            MS->audio_clock = (double)av_q2d(MS->audio_st->time_base) * (double)MS->pkt.pts;
        }
        MS->audio_pkt_data =MS->pkt.data;
        MS->audio_pkt_size = MS->pkt.size;
    }
}

// 解码后的回调函数
void audio_callback(void* userdata, Uint8* stream, int len)
{
    mediaState* MS = (mediaState*)userdata;
    int len1, audio_size;
    SDL_memset(stream, 0, len);
    if (isquit)
        return;
    while (len > 0)
    {
         uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE*2];
        if (MS->audio_buf_index >= MS->audio_buf_size)
        {
            audio_size = audio_decode_frame(MS, audio_buff, sizeof(audio_buff));
            if (audio_size < 0)
            {
                MS->audio_buf_size = 1024;
                SDL_memset(audio_buff, 0, MS->audio_buf_size);
            }
            else
                MS->audio_buf_size = audio_size;

            MS->audio_buf_index = 0;
        }

        len1 = MS->audio_buf_size - MS->audio_buf_index;
        if (len1 > len)
            len1 = len;

        SDL_MixAudio(stream, audio_buff + MS->audio_buf_index, len, VOL);

        len -= len1;
        stream += len1;
        MS->audio_buf_index += len1;
    }

}

static double synchronize_video(mediaState *MS, AVFrame *src_frame, double pts) {

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
    frame_delay = av_q2d(MS->video_st->codec->time_base);
    /* if we are repeating a frame, adjust clock accordingly */
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
    MS->video_clock += frame_delay;
    return pts;
}

int video_thread(void *arg)
{
    mediaState *is = (mediaState *) arg;
    AVPacket pkt1, *packet = &pkt1;

    int ret, got_picture, numBytes;

    double video_pts = 0; //当前视频的pts
    double audio_pts = 0; //音频pts


    ///解码视频相关
    AVFrame *pFrame, *pFrameRGB;
    uint8_t *out_buffer_rgb; //解码后的rgb数据
    struct SwsContext *img_convert_ctx;  //用于解码后的视频格式转换

    AVCodecContext *pCodecCtx = is->video_st->codec; //视频解码器

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

    while(1)
    {
        if (isquit)
        {
            break;
        }

        if (SDL_AUDIO_PAUSED == SDL_GetAudioStatus()) //判断暂停
        {
            SDL_Delay(1);
            continue;
        }
        if (packet_queue_get(&is->videoq, packet, 0) <= 0)
        {
            SDL_Delay(1); //队列只是暂时没有数据而已
            continue;
        }

        //收到这个数据 说明刚刚执行过跳转 现在需要把解码器的数据 清除一下
        if(strcmp((char*)packet->data,FLUSH_DATA) == 0)
        {
            avcodec_flush_buffers(is->video_st->codec);
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

        video_pts *= av_q2d(is->video_st->time_base);
        video_pts = synchronize_video(is, pFrame, video_pts);

       /* if (is->seek_flag_video)
        {
            //发生了跳转 则跳过关键帧到目的时间的这几帧
           if (video_pts < is->seek_time)
           {
               av_free_packet(packet);
               continue;
           }
           else
           {
               is->seek_flag_video = 0;
           }
        }*/

        while(1)
        {
            if (isquit)
            {
                break;
            }
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
            emit ffplayerPointer->sig_CurImageChange(image); //调用激发信号的函数
        }

        av_free_packet(packet);
    }
    av_free(pFrame);
    av_free(pFrameRGB);
    av_free(out_buffer_rgb);
}

int interrupt_cb(void *ctx)//只有正在播放才会做这个函数//网络不畅就会一直做这里 1秒10连发
{
   mediaState *MS=(mediaState*)ctx;
   int time=QTime::currentTime().msec();
   static int changeValue=0;
   int  cz=time-changeValue;
   if(cz>95&&cz<105)
   {
       MS->isBuffering=true;
       ffplayerPointer->updateStatus();
   }
   changeValue=time;
   return 0;
}

void FFmpegPlayer::FreeAllocSpace() //存在内在
{
    SDL_CloseAudio();//Close SDL
    SDL_Quit();


    if(m_MS.wanted_frame) //avframe freee
    {
        av_frame_free(&m_MS.wanted_frame);
    }

    if(m_MS.frame) //avframe freee
    {
       av_frame_free(&m_MS.frame);
    }

    if(m_MS.afct) //format context
    {
        avformat_close_input(&m_MS.afct);
        avformat_free_context(m_MS.afct);
    }
    if(m_MS.acct)//audio context
    {
        avcodec_close(m_MS.acct);
        avcodec_free_context(&m_MS.acct);
    }
    if(m_MS.vcct)//video context
    {
        avcodec_close(m_MS.vcct);
        avcodec_free_context(&m_MS.vcct);
    }

    if(m_MS.swr_ctx) //重采样 freee
    {
        swr_free(&m_MS.swr_ctx);
    }

    if(m_MS.audio_pkt_data)//buff free
    {
        av_freep(m_MS.audio_pkt_data);
    }

    packet_queue_flush(&m_MS.audioq);//队列freee
    packet_queue_flush(&m_MS.videoq);//队列freee

    m_MS={0};//自动将能初始化为0的都初始化为NULL

}



FFmpegPlayer::FFmpegPlayer(QObject *parent) : QThread(parent)
{
    ffplayerPointer=this;
    m_timer=new QTimer;
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timerWork()));
    m_timer->start(30);


    av_register_all();
    avformat_network_init();
    CoInitializeEx(NULL, COINIT_MULTITHREADED);//防止有些windows64找不到audio设备
    m_MS={0};//自动将能初始化为0的都初始化为0
}


void FFmpegPlayer::setMedia(const QString url)
{
    stop();
    emit sig_CurrentMediaChange(url);
    m_url=url;
    start();
    setPriority(QThread::HighPriority);
}

void FFmpegPlayer::stop()
{
    isquit=1;
    Sleep(150);//等待退出线程
    m_url="";
}


PlayerStatus FFmpegPlayer::getPlayerStatus() const
{
    if(m_MS.isBuffering)
        return PlayerStatus::bufferingStatus;
    if(SDL_AUDIO_PLAYING ==SDL_GetAudioStatus())
            return PlayerStatus::playingStatus;
    return PlayerStatus::pausingStatus;
}



void FFmpegPlayer::slot_timerWork()
{
    if(m_MS.frame&&!m_MS.isBuffering)
    emit sig_PositionChange(getCurrentTime());
    updateStatus();
}
void FFmpegPlayer::seek(qint64 pos)
{
    if(!m_MS.seek_req)
    {
        m_MS.seek_pos=pos;
        m_MS.seek_req=true;
    }
}
void FFmpegPlayer::run()
{
    isquit=0;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    packet_queue_init(&m_MS.audioq);
    packet_queue_init(&m_MS.videoq);

    // 读取文件头，将格式相关信息存放在AVFormatContext结构体中
    if (avformat_open_input(&m_MS.afct, m_url.toUtf8().data(), nullptr, nullptr) != 0)
    {
        FreeAllocSpace();
        return; // 打开失败
    }

    m_MS.afct->interrupt_callback.callback = interrupt_cb;//--------注册回调函数
    m_MS.afct->interrupt_callback.opaque = &m_MS;
    // 检测文件的流信息
    if (avformat_find_stream_info(m_MS.afct, nullptr) < 0)
    {
        FreeAllocSpace();
        return; // 没有检测到流信息 stream infomation
    }
    //查找第一个视频流 video stream
    m_MS.audiostream = -1;
    m_MS.videostream = -1;
    for (unsigned int i = 0; i < m_MS.afct->nb_streams; i++)
    {
        if (m_MS.afct->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            m_MS.videostream = i;
        }
        if (m_MS.afct->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO  && m_MS.audiostream < 0)
        {
            m_MS.audiostream = i;
        }
    }
    // 3. 根据读取到的流信息查找相应的解码器并打开

    if (m_MS.audiostream == -1&&m_MS.videostream==-1)
    {
        FreeAllocSpace();
        return; // 没有检测到流信息 stream infomation
    }
//[0][1] for audio
    if(m_MS.audiostream!=-1)
    {
        m_MS.acct = m_MS.afct->streams[m_MS.audiostream]->codec; // codec context
        AVCodec* acodec = avcodec_find_decoder(m_MS.acct->codec_id);
        if (!acodec)
        {
            qDebug() << "Unsupported codec!" << endl;
            FreeAllocSpace();
            return;
        }
        m_MS.audio_st=m_MS.afct->streams[m_MS.audiostream];
        avcodec_open2(m_MS.acct, acodec, nullptr); //open
    }
//[2][3]for video
    if(m_MS.videostream!=-1)
    {
        m_MS.vcct = m_MS.afct->streams[m_MS.videostream]->codec; // codec context
        AVCodec* vcodec = avcodec_find_decoder(m_MS.vcct->codec_id);
        if (!vcodec)
        {
            qDebug() << "Unsupported codec!" << endl;
            FreeAllocSpace();
            return;
        }
        m_MS.video_st=m_MS.afct->streams[m_MS.videostream];
        avcodec_open2(m_MS.vcct, vcodec, nullptr); //open
    }

//[][]
    // Set audio settings from codec info
    SDL_AudioSpec wanted_spec, spec;
    wanted_spec.freq = m_MS.acct->sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = m_MS.acct->channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = &m_MS;

    if (SDL_OpenAudio(&wanted_spec, &spec) < 0)
    {
        qDebug() << "Open audio failed:" << SDL_GetError() << endl;
        FreeAllocSpace();
        return ;
    }
    m_MS.wanted_frame=av_frame_alloc();
    m_MS.frame=av_frame_alloc();

    m_MS.wanted_frame->format = AV_SAMPLE_FMT_S16;
    m_MS.wanted_frame->sample_rate = spec.freq;
    m_MS.wanted_frame->channel_layout = av_get_default_channel_layout(spec.channels);
    m_MS.wanted_frame->channels = spec.channels;
    if(m_MS.videostream!=-1)
        m_MS.video_tid = SDL_CreateThread(video_thread, "video_thread", &m_MS);
    SDL_PauseAudio(0);

    int get=0;
    AVPacket packet;
    while (true) //这里有一个顺序！先判断退出线程信号~再 读 再写入
    {
        SDL_Delay(1);
        if (isquit)
        {
            wanted_spec.callback=NULL;
            wanted_spec.userdata=NULL;
            break;
        }
        if(get<0&&!m_MS.audioq.first_pkt)//end of the file 队列不为空！而且完成@！
        {
            wanted_spec.callback=NULL;
            wanted_spec.userdata=NULL;
            break;
        }
        //seek part
        if (m_MS.seek_req)
        {
            int stream_index = -1;
            int64_t seek_target = m_MS.seek_pos;

            if (m_MS.videostream >= 0)
                stream_index = m_MS.videostream;
            else if (m_MS.audiostream >= 0)
                stream_index = m_MS.audiostream;

            AVRational aVRational = {1, AV_TIME_BASE};
            if (stream_index >= 0)
            {
                seek_target = av_rescale_q(seek_target, aVRational,
                        m_MS.afct->streams[stream_index]->time_base);
            }

            if (av_seek_frame(m_MS.afct, stream_index, seek_target, AVSEEK_FLAG_BACKWARD) < 0)
            {
                  fprintf(stderr, "%s: error while seeking\n",m_MS.afct->filename);
            }
            else
            {
                if (m_MS.audiostream >= 0)
                {
                    AVPacket packet; //分配一个packet
                    av_new_packet(&packet, 10);
                    strcpy((char*)packet.data,FLUSH_DATA);
                    packet_queue_flush(&m_MS.audioq); //清除队列
                    packet_queue_put(&m_MS.audioq, &packet); //往队列中存入用来清除的包
                    av_free_packet(&packet);
                }
                if (m_MS.videostream >= 0)
                {
                    AVPacket packet; //分配一个packet
                    av_new_packet(&packet, 10);
                    strcpy((char*)packet.data,FLUSH_DATA);
                    packet_queue_flush(&m_MS.videoq); //清除队列
                    packet_queue_put(&m_MS.videoq, &packet); //往队列中存入用来清除的包
                    m_MS.video_clock = 0;
                    av_free_packet(&packet);
                }

            }
            m_MS.seek_req = 0;
        }
        if (m_MS.audioq.size > MAX_AUDIO_SIZE || m_MS.videoq.size > MAX_VIDEO_SIZE)//防止一下子把音频全部读完了~
            continue;
        get= av_read_frame(m_MS.afct, &packet);
        if(get==0)//=0就是正确的~再添加进队列
        {
            if(packet.stream_index == m_MS.videostream)
                  packet_queue_put(&m_MS.videoq,&packet);
            else if (packet.stream_index == m_MS.audiostream)
                     packet_queue_put(&m_MS.audioq, &packet);
            else
                av_free_packet(&packet);
        }

    }
    if(!isquit) //It finished automatically when played to end of the media
    emit sig_CurrentMediaFinished();
    isquit=1;
    FreeAllocSpace();
}
