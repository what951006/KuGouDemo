#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#include <QImage>
#include <QDebug>
#include <windows.h>
#include <QMutex>
#include <QThread>
#include <QTime>
#include <QTimer>


#define MAX_AUDIO_FRAME_SIZE  192000
#define SDL_AUDIO_BUFFER_SIZE  1024
#define MAX_AUDIO_SIZE (25 * 16 * 1024)
#define MAX_VIDEO_SIZE (25 * 256 * 1024)
#define FLUSH_DATA "FLUSH"

extern "C"
{
    # include <libavcodec\avcodec.h>
    # include <libavformat\avformat.h>
    # include <libswscale\swscale.h>
    # include <libswresample\swresample.h>
    # include <include/SDL2/SDL.h>
    # include <include/SDL2/SDL_thread.h>
}


typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

enum PlayerStatus{playingStatus,pausingStatus,stopStatus,bufferingStatus};


typedef struct{
    AVFormatContext* fct; //格式上下文

    AVFrame *wanted_frame;//音频目标帧
    AVCodecContext *acct;//音频解码上下文
    AVStream *aStream;
    PacketQueue audioq; //音频队列

    HANDLE VideoHD;
    HANDLE AudioHD;

    AVCodecContext *vcct;//视频解码上下文
    AVStream *vStream;
    PacketQueue videoq; //视频队列

    bool seek_req;
    qint64 seek_pos;

    double video_clock;
    double audio_clock;
}mediaState;

extern int g_nVol;


class FFmpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegPlayer(QObject *parent = 0);
    ~FFmpegPlayer(){}
//---------------------------------
    void setMedia(const QString &url,bool isMV=false);

    void stop();

    void pause(){SDL_PauseAudio(1);updateStatus();}

    void play(){ SDL_PauseAudio(0);updateStatus();}

    inline void updateStatus(){ if(m_MS.fct) emit sig_CurrentMediaStatus(getPlayerStatus());}

    /*duration with now playing the media */
    inline qint64 getDuration(){ if(m_MS.fct)return m_MS.fct->duration;return 0;}

    /*get current media time  millisecond*/
    inline qint64 getCurrentTime(){return m_MS.audio_clock*1000;}

//----------------------------------

    PlayerStatus getPlayerStatus();

    void FreeCommSpace();
    void FreeVideoAlloc();
    void FreeAudioAlloc();
protected:
    virtual void run();
signals:
    void sig_BufferingPrecent(double);
    void sig_CurImageChange(QImage);
    void sig_CurrentMediaChange(const QString&,bool isMv);
    void sig_CurrentMediaDurationChange(qint64);
    void sig_PositionChange(qint64);
    void sig_CurrentMediaFinished();
    void sig_CurrentMediaStatus(PlayerStatus);
    void sig_CurrentMediaError();

public slots:
    void slot_timerWork(){emit sig_PositionChange(getCurrentTime());}

    void setVol(int vol){g_nVol=vol;}

    void seek(qint64 );
private:
    QString m_url;

    mediaState m_MS;

    QTimer m_timer;
};

#endif // FFMPEGPLAYER_H
