#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H
extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswresample/swresample.h"
    #include "SDL2/SDL.h"
} //need to configure C++11

#include<QObject>
#include<QThread>
#include<QTimer>
#include<QTime>

enum PlayerStatus{playingStatus,pausingStatus,stopStatus,bufferingStatus};

class FFmpegPlayer : public QThread
{
    Q_OBJECT
public:
    explicit FFmpegPlayer(QObject *parent = 0);
    ~FFmpegPlayer();

    void setMedia(const QString );

    void pause();

    void stop();

    void play();

    void updateStatus();

    /*zero  means pause ,one means playing*/
    PlayerStatus getPlayerStatus() const;

    /*duration with now playing the media */
    qint64 getDuration();

    /*get current media time value*/
    qint64 getCurrentTime();

    QTimer *m_timer;
public slots:
    void setVol(int);

    void seek(qint64 );

    void slot_timerWork();
protected:
    virtual void run();
    void FreeAllocMemory();
signals:
    void sig_CurrentMediaChange(const QString&);
    void sig_CurrentMediaDurationChange(qint64);
    void sig_PositionChange(qint64);
    void sig_CurrentMediaFinished();
    void sig_CurrentMediaStatus(PlayerStatus);
    void sig_CurrentMediaError();
private:
    QString m_path;
    bool m_seek_request;
    qint64 m_seekTime;
    qint64 m_prePos;
};

#endif // FFMPEGPLAYER_H
