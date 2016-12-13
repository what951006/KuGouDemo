#include "FFmpegPlayer.h"
#include<QDebug>
#include<windows.h>
#include<QTime>

#include"Global_ValueGather.h"


FFmpegPlayer *ffmpegPointer=NULL;
static bool isquit=false; //清空了
static int VOL=80;
// 包队列初始化
void packet_queue_init(PacketQueue* q)
{
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

// 放入packet到队列中，不带头指针的队列
static int packet_queue_put(PacketQueue*q, AVPacket *pkt)
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
static int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block) {
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

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
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

static void packet_queue_flush(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);
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
    SDL_UnlockMutex(q->mutex);
}
//////////////////////////////////////////////解码音频数据

int audio_decode_frame(mediaState* MS, uint8_t* audio_buf, int buf_size)
{
    int len1;
    int data_size = 0;
    if (isquit)
        return -1;

    while (true)
    {
        while (MS->audio_pkt_size > 0)
        {
            if (isquit)
                return -1;

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
                                         MS->frame->sample_rate, 0, nullptr);//无限分配 ？
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
            return MS->wanted_frame->channels * len2 * av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);

            if (data_size <= 8)
                continue; // No data yet,get more frames
            return data_size; // we have data,return it and come back for more later
        }
        if (MS->pkt.buf)
            av_free_packet(&MS->pkt);
        if (packet_queue_get(&MS->audioq,&MS->pkt,1)<=0)
            return -1;
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
       //buff一定要*2以上，如果小了就会在memcpy的时候变小！
       uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE*2];
        if (MS->audio_buf_index >= MS->audio_buf_size)
        {
            audio_size = audio_decode_frame(MS, audio_buff, sizeof(audio_buff));
            if (isquit)
                return;
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
        MS->audio_buf_index += len1;//这个就代表的当前位置
    }
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
       ffmpegPointer->updateStatus();
   }
   changeValue=time;
   return 0;
}

void FFmpegPlayer::FreeAllocSpace() //存在内在
{
    SDL_CloseAudio();//Close SDL
    SDL_Quit();

    if(m_MS.wanted_frame)
    av_frame_free(&m_MS.wanted_frame);
    if(m_MS.frame)
    av_frame_free(&m_MS.frame);
    if(m_MS.acct)
    avcodec_close(m_MS.acct);
    if(m_MS.afct)
    avformat_close_input(&m_MS.afct);
    if(m_MS.swr_ctx)
    swr_free(&m_MS.swr_ctx);
    if(m_MS.audio_pkt_data)
    av_freep(m_MS.audio_pkt_data);
    if(m_MS.audioq.first_pkt)
    packet_queue_flush(&m_MS.audioq);
    m_MS={0};//自动将能初始化为0的都初始化为0

}


FFmpegPlayer::FFmpegPlayer(QObject *parent) : QThread(parent)
{
    ffplayerPointer=this;
    ffmpegPointer=this;
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
    emit sig_CurrentMediaChange(url);
    stop();
    m_url=url;
    start();
    setPriority(QThread::HighestPriority);
}

void FFmpegPlayer::stop()
{
    isquit=1;
    Sleep(150);//等待退出线程
}

void FFmpegPlayer::pause()
{
    SDL_PauseAudio(1);
}

void FFmpegPlayer::play()
{
    SDL_PauseAudio(0);
}
qint64 FFmpegPlayer::getDuration()
{
    if(!m_MS.afct)
            return 0;
        return m_MS.afct->duration;
}
PlayerStatus FFmpegPlayer::getPlayerStatus() const
{
    if(m_MS.isBuffering)
        return PlayerStatus::bufferingStatus;
    if(SDL_AUDIO_PLAYING ==SDL_GetAudioStatus())
            return PlayerStatus::playingStatus;
    return PlayerStatus::pausingStatus;
}

void FFmpegPlayer::updateStatus()
{
    if(!m_MS.acct)
        return;
       emit sig_CurrentMediaStatus(getPlayerStatus());
}

void FFmpegPlayer::slot_timerWork()
{
    if(m_MS.frame&&!m_MS.isBuffering)
    emit sig_PositionChange(getCurrentTime());
    updateStatus();
}
qint64 FFmpegPlayer::getCurrentTime()//dts是解码的位置 ，pts是显示时间的位置
{
    if(!m_MS.frame)
        return 0;
    static  int prePos=0;
    qint64 curdts=m_MS.acct->pts_correction_last_pts;
    qDebug()<<curdts;
    if(curdts<=0)
      return prePos;
    AVRational aVRational2 = {1, AV_TIME_BASE};
    qint64 pos= av_rescale_q(curdts, aVRational2,m_MS.afct->streams[m_MS.audiostream]->time_base)/200;
    prePos=pos;

    return pos;
}
void FFmpegPlayer::setVol(int vol)
{
    VOL=vol;
}
void FFmpegPlayer::seek(qint64)
{

}
void FFmpegPlayer::run()
{
    isquit=0;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
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
    for (unsigned int i = 0; i < m_MS.afct->nb_streams; i++)
    {
        if (m_MS.afct->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            m_MS.audiostream = i;
            break;
        }
    }
    // 3. 根据读取到的流信息查找相应的解码器并打开
    if (m_MS.audiostream == -1)
    {
        FreeAllocSpace();
        return; // 没有检测到流信息 stream infomation
    }
    m_MS.acct = m_MS.afct->streams[m_MS.audiostream]->codec; // codec context
    AVCodec* pCodec = avcodec_find_decoder(m_MS.acct->codec_id);

    if (!pCodec)
    {
        qDebug() << "Unsupported codec!" << endl;
        FreeAllocSpace();
        return;
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

    avcodec_open2(m_MS.acct, pCodec, nullptr);
    packet_queue_init(&m_MS.audioq);
    SDL_PauseAudio(0);

    AVPacket packet;
    while (true) //这里有一个顺序！先判断退出线程信号~再 读 再写入
    {
        SDL_Delay(10);
        if (isquit)
        {
            wanted_spec.callback=NULL;
            wanted_spec.userdata=NULL;
            break;
        }
        if (m_MS.audioq.size > MAX_AUDIO_SIZE )//防止一下子把音频全部读完了~
            continue;
        if(av_read_frame(m_MS.afct, &packet)==0)//=0就是正确的~再添加进队列
        {

            if (packet.stream_index == m_MS.audiostream)
                packet_queue_put(&m_MS.audioq, &packet);
            else
                av_free_packet(&packet);
        }
    }
    FreeAllocSpace();
}
