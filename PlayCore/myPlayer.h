/******************************
 * Qt player using libVLC     *
 * By protonux                * 
 * Under WTFPL                *
 ******************************/

#ifndef MYPLAYER
#define MYPLAYER


#include <QtGui>
#include <vlc/vlc.h>
#include<QObject>

enum PlayerState{playingState,pausingState};

class myPlayer : public QObject
{
    Q_OBJECT
  public:
               myPlayer(QObject*p);
               virtual ~myPlayer();
               qint64 currentMediaLength();
               void setVideoWidget(void*);
               PlayerState currentPlayerState();
public slots:
               void setMedia(QString filepath);
               void play();
               void pause();
               void stop();

               void  slot_changeVolume(int);
               void slot_changePosition(int);
               void updateInterface();
signals:
               void sig_durationChange(qint64);
               void sig_stateChange(PlayerState);
               void sig_currentMediaChange(const QString&);
               void sig_endOfMedia();
               void sig_positionChange(double precent,qint64 length);
private:
               libvlc_instance_t *vlcInstance;
               libvlc_media_player_t *vlcPlayer;
               int m_vol;
};


#endif
