/******************************
 * Qt player using libVLC     *
 * By protonux                *
 *                            *
 * Under WTFPL                *
 ******************************/

#include "myplayer.h"
#include <vlc/vlc.h>
#include<QMessageBox>
#include<QMenu>
#include<QMenuBar>
#include<QSlider>
#include<QFileDialog>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<stdlib.h>
#define qtu( i ) ((i).toUtf8().constData())

myPlayer::myPlayer(QObject*p):QObject(p) {
    vlcPlayer = NULL;
    m_vol=80;
    /* Initialize libVLC */
    vlcInstance = libvlc_new(0, NULL);

    /* Complain in case of broken installation */
    if (vlcInstance == NULL) {
        exit(1);
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateInterface()));
    timer->start(800);
}

myPlayer::~myPlayer() {
    /* Release libVLC instance on quit */
    if(vlcPlayer)
        stop();
    if (vlcInstance)
        libvlc_release(vlcInstance);
}

void myPlayer::setMedia(QString fileOpen) {

    emit sig_currentMediaChange(fileOpen);
    std::string strPath(fileOpen.toStdString());
    bool bURL = false;
    std::vector<std::string> vctURL;

    vctURL.push_back("http");
    vctURL.push_back("https");
    vctURL.push_back("ftp");
    vctURL.push_back("rstp");

    for (unsigned i = 0; i < vctURL.size(); i++)
    {
        // 实际使用请判断大小写
        if (! strPath.compare(0, vctURL[i].size(), vctURL[i]))
        {
            bURL = true;
            break;
        }
    }
    libvlc_media_t * vlcMedia=NULL;
    if (bURL)
       vlcMedia = libvlc_media_new_location(vlcInstance, qtu(fileOpen)); //network media stream
    else
        vlcMedia = libvlc_media_new_path(vlcInstance, qtu(fileOpen.replace(":/",":\\"))); //local media


    if (!vlcMedia)
        return;
    stop(); // have to stop and release last player pointer
    /* Create a new libvlc player */
    vlcPlayer = libvlc_media_player_new_from_media (vlcMedia);
    /* Release the media */
    libvlc_media_release(vlcMedia);

    /* And start playback */
     libvlc_media_player_play (vlcPlayer);
    /* Update playback button */
}

void myPlayer::play() {
    if (!vlcPlayer)
        return;

    if (libvlc_media_player_is_playing(vlcPlayer))
    {

        /* Pause */
        libvlc_media_player_pause(vlcPlayer);
         emit sig_stateChange(PlayerState::pausingState);
    }
    else
    {
        emit sig_stateChange(PlayerState::playingState);
        /* Play again */
        libvlc_media_player_play(vlcPlayer);
    }
}

void myPlayer::pause()
{
    if (!vlcPlayer)
        return;

    if (libvlc_media_player_is_playing(vlcPlayer))
    {
        /* Pause */
        libvlc_media_player_pause(vlcPlayer);
         emit sig_stateChange(PlayerState::pausingState);
    }
}

void myPlayer::slot_changeVolume(int vol) { /* Called on volume slider change */
    if(vlcPlayer)
     libvlc_audio_set_volume (vlcPlayer,m_vol);/*update vol*/
     m_vol=vol;
}

void myPlayer::slot_changePosition(int pos) { /* Called on position slider change */
    if (!vlcPlayer)
        return;
    if(pos>1000)/*lyric widget control*/
    {
        int dur=currentMediaLength();
        libvlc_media_player_set_position(vlcPlayer, (float)pos/dur);
    }
    else
    {
        libvlc_media_player_set_position(vlcPlayer, (float)pos/1000.0);
    }
}

void myPlayer::updateInterface() { //Update interface and check if song is finished
    if (!vlcPlayer)
        return;
    if (libvlc_media_player_is_playing(vlcPlayer))
    {
         emit sig_stateChange(PlayerState::playingState);
         float pos = libvlc_media_player_get_position(vlcPlayer);
         emit sig_positionChange(pos,currentMediaLength());
    }
    else
        emit sig_stateChange(PlayerState::pausingState);

    libvlc_audio_set_volume (vlcPlayer,m_vol);/*update vol*/
   if (libvlc_media_player_get_state(vlcPlayer) == libvlc_Ended)
   {
     /* float pos = libvlc_media_player_get_position(vlcPlayer);
      if(pos>=0.9)
      {
          this->stop();
      }*/
       emit sig_endOfMedia();
   }
}

void myPlayer::stop() {
    if(vlcPlayer) {
        /* stop the media player */
        libvlc_media_player_stop(vlcPlayer);

        /* release the media player */
        libvlc_media_player_release(vlcPlayer);

        /* Reset application values */
        vlcPlayer = NULL;
    }
}

qint64 myPlayer::currentMediaLength()
{
    if(!vlcPlayer)
        return 0;
    return libvlc_media_player_get_length(vlcPlayer);
}

void myPlayer::setVideoWidget(void * p)
{
    if(vlcPlayer)
        libvlc_media_player_set_hwnd(vlcPlayer, p);
}

PlayerState myPlayer::currentPlayerState()
{
    if (libvlc_media_player_is_playing(vlcPlayer))
         return PlayerState::playingState;
         return PlayerState::pausingState;

}
