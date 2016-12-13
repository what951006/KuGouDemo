#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

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

#include<QThread>
#include<QTimer>

enum PlayerStatus{playingStatus,pausingStatus,stopStatus,bufferingStatus};

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;




typedef struct{
    SwrContext* swr_ctx ;//
    AVFrame *wanted_frame;//
    uint8_t* audio_pkt_data;
    int audio_pkt_size; //
    AVFrame *frame; //
    AVFormatContext* afct; //
    AVCodecContext *acct;//

    int audiostream;
    unsigned int audio_buf_size; //
    unsigned int audio_buf_index; //
    bool isBuffering;

    PacketQueue audioq; //
    AVPacket pkt; //
}mediaState;




class FFmpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegPlayer(QObject *parent = 0);
    void setMedia(const QString);
    void stop();
    void pause();
    void play();

    void updateStatus();
    /*zero  means pause ,one means playing*/
    PlayerStatus getPlayerStatus() const;

    /*duration with now playing the media */
    qint64 getDuration();

    /*get current media time value*/
    qint64 getCurrentTime();

    QTimer *m_timer;
    void FreeAllocSpace();
protected:
    virtual void run();
signals:
    void sig_CurrentMediaChange(const QString&);
    void sig_CurrentMediaDurationChange(qint64);
    void sig_PositionChange(qint64);
    void sig_CurrentMediaFinished();
    void sig_CurrentMediaStatus(PlayerStatus);
    void sig_CurrentMediaError();
public slots:
    void slot_timerWork();

    void setVol(int);

    void seek(qint64 );
private:
    QString m_url;
    mediaState m_MS;
};

#endif // FFMPEGPLAYER_H
