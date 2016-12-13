#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#define MAX_AUDIO_FRAME_SIZE  192000
#define SDL_AUDIO_BUFFER_SIZE  1024
#define MAX_AUDIO_SIZE ( 10*16 * 1024)
#define MAX_VIDEO_SIZE ( 10*256 * 1024)
#define FLUSH_DATA "FLUSH"

extern "C"
{
    #include <libavcodec\avcodec.h>
    #include <libavformat\avformat.h>
    #include <libswscale\swscale.h>
    #include <libswresample\swresample.h>
    #include <include/SDL2/SDL.h>
    #include <include/SDL2/SDL_thread.h>
}

extern int VOL;
#include<QThread>
#include<QTimer>
#include<QImage>

enum PlayerStatus{playingStatus,pausingStatus,stopStatus,bufferingStatus};

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;




typedef struct{
    AVFormatContext* afct; //
    AVPacket pkt; //
    ////////////////////////////common part
    SwrContext* swr_ctx ;//
    AVFrame *wanted_frame;//
    uint8_t* audio_pkt_data;
    int audio_pkt_size; //
    AVFrame *frame; //

    AVCodecContext *acct;//
    AVStream *audio_st;
    int audiostream;
    double audio_clock;
    unsigned int audio_buf_size; //
    unsigned int audio_buf_index; //
    bool isBuffering;
    bool seek_req;
    qint64 seek_pos;
    PacketQueue audioq; //
    ///////////////////// audio and video
    AVCodecContext *vcct;
    int videostream;
    double video_clock;
    PacketQueue videoq;
    AVStream *video_st;
    SDL_Thread *video_tid;  //视频线程id
}mediaState;




class FFmpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegPlayer(QObject *parent = 0);
    void setMedia(const QString);
    void stop();
    void pause(){SDL_PauseAudio(1);}
    void play(){ SDL_PauseAudio(0);}

    inline void updateStatus(){ if(!m_MS.acct)return;emit sig_CurrentMediaStatus(getPlayerStatus());}
    /*zero  means pause ,one means playing*/
    PlayerStatus getPlayerStatus() const;

    /*duration with now playing the media */
    inline qint64 getDuration(){ if(!m_MS.acct)return 0;return m_MS.afct->duration;}

    /*get current media time value*/
    inline qint64 getCurrentTime(){return m_MS.audio_clock*1000000;}

    QTimer *m_timer;
    void FreeAllocSpace();
protected:

    virtual void run();
signals:
    void sig_CurImageChange(QImage);
    void sig_CurrentMediaChange(const QString&);
    void sig_CurrentMediaDurationChange(qint64);
    void sig_PositionChange(qint64);
    void sig_CurrentMediaFinished();
    void sig_CurrentMediaStatus(PlayerStatus);
    void sig_CurrentMediaError();
public slots:
    void slot_timerWork();

    void setVol(int vol){VOL=vol;}

    void seek(qint64 );
private:
    QString m_url;
    mediaState m_MS;
};

#endif // FFMPEGPLAYER_H
