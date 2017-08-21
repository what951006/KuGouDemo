#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QTimeLine>
#include <qthread.h>
#include <qmetatype.h>
#include <stdio.h>

#include "FFmpegPlayer.h"
#include "myMediaList.h"
#include "basewindow.h"
#include "baseWidget.h"
#include "topwidgets.h"
#include "middleWidgets.h"
#include "mynetwork.h"
#include "bottomWidgets.h"
#include "trayiconmenu.h"
#include "skinWidget.h"


class mainWindow:public baseWindow
{
   friend class baseWidget;
    Q_OBJECT
public:
    ~mainWindow();
 //get Instance
   static mainWindow *GetInstance(){return s_pMainWnd;}

//initialize Instance
   static mainWindow * InitInstance();

//update playlist connection
   void UpdateListConn();

   void initLayout();

   void initTrayMenu();

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

   void setCurBGPic(const QString&);

//  inline deskTopLrcWidget*deskTopLrcWid(){return m_deskTopLrc;}
   middleLeftStackWidget0* middleStack0(){return m_pLefStack0;}

   FFmpegPlayer* player(){return &m_ffplayer;}

   TopWidgets*topWidget(){return &m_topwid;}

   middleWidgets*middleWidget(){return &m_middwid;}

   bottomWidgets *bottomWidget(){return &m_bottomwid;}

   trayIconMenu *trayMenu(){return &m_traymenu;}

   MyNetWork *myNetWork(){return &m_net;}
protected:
   explicit mainWindow(QWidget*parent=0);

   virtual bool eventFilter(QObject *, QEvent *);

   virtual void closeEvent(QCloseEvent *);

   virtual void mouseDoubleClickEvent(QMouseEvent *);

   static mainWindow *s_pMainWnd;
public slots:

    void slot_currentMediaError();

    void slot_setPlayerPlayOrPause();

    void slot_OpenDeskTopLrc();

    void slot_timelineAnimation(int);

    void slot_setBgPix(const QStringList&,const QString &);

    void slot_quitApp();

    void slot_setPlayMode(PlayMode);
    void slot_setPlayModeWidget();

    void slot_currentMediaChanged(const QString&,bool isMV);

    void slot_iconIsActived(QSystemTrayIcon::ActivationReason);

    void slot_positionChange(qint64);

    void slot_playerStatusChanged(PlayerStatus);

    void slot_adjustWindowNormalSize();

    void slot_replyLrc(QByteArray,QString);
protected:

signals:
     void sig_requestMv(const QString&);

     void sig_requestBgPic(const QString&);

     void sig_requestLrc(const QString & keywords,qint64 nDur,const QString& strLocation,const QString&strHash);

     void sig_requestAlbum(const QString&,const QString&);
private:
    TopWidgets m_topwid;
    middleWidgets m_middwid;
    bottomWidgets m_bottomwid;
    FFmpegPlayer m_ffplayer;

    volSliderWidget m_volwid;
    playModeWidget m_playModeWid;
    topSearchTipsWidget m_sertipswid;
    skinWidget m_skinwid;

    QSystemTrayIcon m_system_tray;
    trayIconMenu m_traymenu;
    MyNetWork m_net;
    PlayMode pMode; //use for saving the now playmode when exited
    //   deskTopLrcWidget *m_deskTopLrc;

    QTimeLine m_timeline;
    QStringList m_picList;

    middleLeftStackWidget0* m_pLefStack0;
};

#endif // MAINWINDOW_H
