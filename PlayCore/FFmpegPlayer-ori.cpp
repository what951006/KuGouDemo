#include "FFmpegPlayer.h"
#include<QDebug>
#include<windows.h>

#include"Global_ValueGather.h"

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio  2387131548
#define USE_SDL 1


static  Uint8  *audio_chunk;
static  Uint32  audio_len;
static  Uint8  *audio_pos;
static  qint64 audio_buf_index;
static    AVPacket	*packet;
static   AVFormatContext	*pFormatCtx;
static int vol=80;
static int	audioStream;
static int  changeValue;
static bool isBuffering;
static int isquit=0;

static AVCodecContext	*pCodecCtx;
static AVCodec			*pCodec;
static uint8_t			*out_buffer;
static AVFrame			*pFrame;
static SDL_AudioSpec wanted_spec;

FFmpegPlayer::FFmpegPlayer(QObject *parent) : QThread(parent)
{
    ffplayerPointer=this;
    m_prePos=0;
    av_register_all();//
    avformat_network_init(); //

    m_timer=new QTimer;
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timerWork()));
    m_timer->start(30);

    pFormatCtx=NULL;
    audioStream=0;
    pCodecCtx=NULL;
    pCodec=NULL;
    packet=NULL;
    out_buffer=NULL;
    pFrame=NULL;

    m_seekTime=0;
    m_seek_request=false;

}

FFmpegPlayer::~FFmpegPlayer()
{

}

void FFmpegPlayer::seek(qint64 time)
{
    if(!pFormatCtx)
        return;
    m_seekTime=time;
    m_seek_request=true;
}

void FFmpegPlayer::slot_timerWork()
{
    if(packet&&!isBuffering)
    emit sig_PositionChange(getCurrentTime());

    updateStatus();
}

qint64 FFmpegPlayer::getDuration()
{
    if(!pFormatCtx)
        return 0;
    return pFormatCtx->duration;
}

qint64 FFmpegPlayer::getCurrentTime()
{
    if(!packet)
        return 0;
    if(pFormatCtx->streams[audioStream]->cur_dts<=0||pFormatCtx->streams[audioStream]->cur_dts>=1000000000000000)
      return m_prePos;
    AVRational aVRational2 = {1, AV_TIME_BASE};
    qint64 pos= av_rescale_q(pFormatCtx->streams[audioStream]->cur_dts, aVRational2,
                                        pFormatCtx->streams[audioStream]->time_base)/200;
    m_prePos=pos;
    return pos;
}

void fill_audio(void *udata,Uint8 *stream,int len){
    Q_UNUSED(udata)
    //SDL 2.0
    SDL_memset(stream, 0, len);
    if(audio_len==0)		/*  Only  play  if  we  have  data  left  */
            return;
    len=(len>audio_len?audio_len:len);	/*  Mix  as  much  data  as  possible  */

    SDL_MixAudio(stream,audio_pos,len,vol);
    audio_pos += len;
    audio_len -= len;
    audio_buf_index +=len;
}
 int interrupt_cb(void *ctx)//只有正在播放才会做这个函数//网络不畅就会一直做这里 1秒10连发
{
    int time=QTime::currentTime().msec();
    int  cz=time-changeValue;
    if(cz>95&&cz<105)
    {
        isBuffering=true;
        ffplayerPointer->updateStatus();
    }
    changeValue=time;
    return 0;
}

void FFmpegPlayer::setMedia(const QString str)
{
    stop();
    m_path=str;
    emit sig_CurrentMediaChange(m_path);
    start();
}

void FFmpegPlayer::pause()
{
    SDL_PauseAudio(1);
    m_timer->stop();
    slot_timerWork();
}

void FFmpegPlayer::stop()
{
    isquit=1;
    m_path="";
    Sleep(200);

    audio_len=0;
    audio_pos=NULL;
    audio_buf_index=0;
    audio_chunk=NULL;
}

void FFmpegPlayer::play()
{
    SDL_PauseAudio(0);
    m_timer->start(30);
}

void FFmpegPlayer::updateStatus()
{
    if(!pFormatCtx)
        return;
       emit sig_CurrentMediaStatus(getPlayerStatus());
}

void FFmpegPlayer::setVol(int  v)
{
    vol=v;
}

PlayerStatus FFmpegPlayer::getPlayerStatus() const
{
    if(isBuffering)
        return PlayerStatus::bufferingStatus;
    if(SDL_AUDIO_PLAYING ==SDL_GetAudioStatus())
            return PlayerStatus::playingStatus;
    return PlayerStatus::pausingStatus;
}


void FFmpegPlayer::run()
{
    isquit=0;
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->interrupt_callback.callback = interrupt_cb;//--------注册回调函数
    pFormatCtx->interrupt_callback.opaque = NULL;
    //Open
    if(avformat_open_input(&pFormatCtx,m_path.toStdString().data(),NULL,NULL)!=0){
        qDebug()<<"Couldn't open input stream";
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }

    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        qDebug()<<"Couldn't find stream information.\n";
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }

    // Find the first audio stream
    audioStream=-1;
    for(unsigned int i=0; i < pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
        {
            audioStream=i;
            break;
        }
    if(audioStream==-1){
        qDebug()<<"Didn't find a audio stream.\n";
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }
    // Get a pointer to the codec context for the audio stream
    pCodecCtx=pFormatCtx->streams[audioStream]->codec;

    // Find the decoder for the audio stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        qDebug()<<"Codec not found.\n";
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        qDebug()<<"Could not open codec.\n";
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }

    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    av_init_packet(packet);

    //Out Audio Param
    uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
    //nb_samples: AAC-1024 MP3-1152
    int framesize=pCodecCtx->frame_size;

    AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
    int out_sample_rate=44100;
    int out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
    //Out Buffer Size
    int out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,framesize,out_sample_fmt, 1);

    out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);
    pFrame=av_frame_alloc();

//SDL------------------
#if USE_SDL
    //Init
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        qDebug()<< "Could not initialize SDL - %s\n"<< SDL_GetError();
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }
    //SDL_AudioSpec
    wanted_spec.freq = out_sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = out_channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = framesize;
    wanted_spec.callback = fill_audio;
    wanted_spec.userdata = pCodecCtx;

    if (SDL_OpenAudio(&wanted_spec, NULL)<0)
    {
        qDebug()<<"can't open audio.\n";
        FreeAllocMemory();
        emit sig_CurrentMediaError();
        return ;
    }
#endif
    //FIX:Some Codec's Context Information is missing
    int64_t in_channel_layout=av_get_default_channel_layout(pCodecCtx->channels);
    //Swr

    struct SwrContext *au_convert_ctx = swr_alloc();
    au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
        in_channel_layout,pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
    swr_init(au_convert_ctx);

    /*emit Duration*/
    emit sig_CurrentMediaDurationChange(getDuration());
    audio_buf_index=0;
    audio_len=0;
    audio_pos=0;
    int got_picture=0;

    av_seek_frame(pFormatCtx, audioStream, 0, NULL);
   // pFormatCtx->streams[audioStream]->cur_dts=0;
    while(true)
    {
        if(isquit)
            break;
        if(av_read_frame(pFormatCtx, packet)==0)
        {
            if(isquit)
                break;
            if(packet->stream_index==audioStream)
            {
                    int ret = avcodec_decode_audio4( pCodecCtx, pFrame,&got_picture, packet);
                    if ( ret < 0 ) {
                        qDebug()<<"Error in decoding audio frame  or quit signal.\n";
                        break ;
                    }
                    if ( got_picture > 0 )
                        swr_convert(au_convert_ctx,&out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pFrame->data , pFrame->nb_samples);

                    //seek action ,users do this
                    if(m_seek_request)
                    {
                        AVRational aVRational = {1, AV_TIME_BASE};
                        if (audioStream >= 0)
                        m_seekTime = av_rescale_q(m_seekTime, aVRational,
                                    pFormatCtx->streams[audioStream]->time_base);

                        if (av_seek_frame(pFormatCtx, audioStream, (int64_t)m_seekTime, NULL) < 0)
                            qDebug()<<"%s: error while seeking\n";
                        m_seek_request=false;
                    }
    #if USE_SDL
                    while(audio_len>0)//Wait until finish
                        SDL_Delay(1);

                    //Set audio buffer (PCM data)
                    audio_chunk = (Uint8 *) out_buffer;
                    //Audio buffer length
                    audio_len =out_buffer_size;
                    audio_pos = audio_chunk;
                    //Play
                    SDL_PauseAudio(0);
                    isBuffering=false;
    #endif
            }
            if(packet)
            {
                av_free_packet(packet);
            }
        }

    }
    FreeAllocMemory();
    if(!isquit)
    emit sig_CurrentMediaFinished();
}

void FFmpegPlayer::FreeAllocMemory()
{
    if(packet)
    av_free_packet(packet);

#if USE_SDL
    SDL_CloseAudio();//Close SDL
    SDL_Quit();
#endif
    if(out_buffer)
    av_free(out_buffer);
    // Close the codec
    if(pCodecCtx)
    avcodec_close(pCodecCtx);
    // Close the video file
    if(pFormatCtx)
    avformat_close_input(&pFormatCtx);


    packet=NULL;
    out_buffer=NULL;
    pCodecCtx=NULL;
    pFormatCtx=NULL;
}

