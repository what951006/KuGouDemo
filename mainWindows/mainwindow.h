#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QTimeLine>
#include <qthread.h>
#include <qmetatype.h>
#include <stdio.h>

#include"FFmpegPlayer.h"
#include"myMediaList.h"
#include"basewindow.h"
#include"baseWidget.h"


class middleWidgets;
class TopWidgets;
class MyNetWork;
class loadPixThread;
class topSearchTipsWidget;
class trayIconMenu;
class bottomWidgets;
class volSliderWidget;
class playModeWidget;
class skinWidget;
class FFmpegPlayer;
class middleLeftStackWidget0;
class deskTopLrcWidget;

class mainWindow:public baseWindow
{
   friend class baseWidget;
    Q_OBJECT
public:
   explicit mainWindow(QWidget*parent=0);
    ~mainWindow();
   void initLayout();
   void initTrayMenu();
   void initMediaPlayer();
   void initConnection();
   void initWidgetMISC();
   void initNetwork();
   void initTimeline();
   void readSetting();
   void saveSetting();
   void setOriginalStatus();

   int curVol();
   void setCurVol(int);
   void clearBackground();
   void setBackgroundPixmap(const QPixmap&);

  inline deskTopLrcWidget*deskTopLrcWid(){return m_deskTopLrc;}
  inline middleLeftStackWidget0* middleStack0(){return m_midstack0;}
  inline FFmpegPlayer* player(){return m_ffplayer;}
  inline TopWidgets*topWidget(){return m_topwid;}
  inline middleWidgets*middleWidget(){return m_middwid;}
  inline bottomWidgets *bottomWidget(){return m_bottomwid;}
  inline trayIconMenu *trayMenu(){return m_traymenu;}
  inline MyNetWork *myNetWork(){return m_net;}
protected:
   virtual bool eventFilter(QObject *, QEvent *);
   virtual void closeEvent(QCloseEvent *);
   virtual void mouseDoubleClickEvent(QMouseEvent *);
public slots:

    void slot_currentMediaError();

    void slot_setPlayerPlayOrPause();
    void slot_OpenDeskTopLrc();
    void slot_timelineAnimation(int);
    void slot_setBgPix(const QVector<QPixmap>&,const QString &);

    void slot_quitApp();

    void slot_setPlayMode(PlayMode);
    void slot_setPlayModeWidget();

    void slot_currentMediaChanged(const QString&);
    void slot_iconIsActived(QSystemTrayIcon::ActivationReason);

    void slot_positionChange(qint64);
    void slot_playerStatusChanged(PlayerStatus);

    void slot_adjustWindowNormalSize();
protected:
    QTimeLine *m_timeline;
    QVector<QPixmap> m_pixvector;
    QTimer *_timer;
signals:
     void sig_requestMv(const QString&);
     void sig_requestBgPic(const QString&);
     void sig_requestLrc(const QString &,qint64,const QString&);
     void sig_requestAlbum(const QString&,const QString&);
private:
   volSliderWidget *m_volwid;
   playModeWidget *m_playModeWid;
   topSearchTipsWidget *m_sertipswid;
   skinWidget *m_skinwid;
   TopWidgets *m_topwid;
   middleWidgets *m_middwid;
   bottomWidgets *m_bottomwid;
   QSystemTrayIcon * system_tray;
   trayIconMenu *m_traymenu;
   MyNetWork *m_net;
   PlayMode pMode; //use for saving the now playmode when exited
   deskTopLrcWidget *m_deskTopLrc;
   FFmpegPlayer *m_ffplayer;
   middleLeftStackWidget0* m_midstack0;

};

#endif // MAINWINDOW_H
