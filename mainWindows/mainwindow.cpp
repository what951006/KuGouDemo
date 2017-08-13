#include "mainwindow.h"
#include<QVBoxLayout>
#include<QApplication>
#include<QDesktopWidget>
#include<QSystemTrayIcon>
#include<QDir>
#include<QLineEdit>
#include<QTime>
#include<QDebug>
#include<QSettings>
#include<QDebug>
#include <io.h>

#include"myTablePlayListFinal.h"
#include"middleLeftStackWidget0.h"
#include"topwidgets.h"
#include"middleWidgets.h"
#include"bottomWidgets.h"
#include"trayiconmenu.h"
#include"middlewidgetleft.h"
#include"middleWidgetRight.h"
#include"middleSearchWidget.h"
#include"mynetwork.h"
#include"topSearchTipsWidget.h"
#include"topwidgets.h"
#include"middleWidgets.h"
#include"bottomWidgets.h"
#include"trayiconmenu.h"
#include"skinWidget.h"
#include"deskTopLrcWidget.h"
#include"Global_ValueGather.h"


mainWindow *mainWindow::s_pMainWnd=NULL;

mainWindow*mainWindow::InitInstance()
{
    if(!s_pMainWnd)
        s_pMainWnd=new mainWindow;
    return s_pMainWnd;
}

mainWindow::mainWindow(QWidget *parent):
    baseWindow(parent),
    m_topwid(this),
    m_middwid(this),
    m_bottomwid(this),
    m_ffplayer(this),

    m_volwid(this),
    m_playModeWid(this),
    m_sertipswid(this),
    m_skinwid(this)
{
     s_pMainWnd=this;

     m_border=8;
     setMinimumSize(1008,698);
     m_mainwid->setStyleSheet("QLabel{color:white;}"
                              "QWidget{background:transparent;}");//"Widget{background: rgb(0,140,230);}"//border-image:url(:/image/skin/1.png)


     initLayout();//加载主要的layout
     initNetwork();
     initTrayMenu();//加载系统图标菜单
     initWidgetMISC();
     initTimeline();
     initConnection();
     readSetting();

     UpdateListConn();
}

void mainWindow::UpdateListConn()
{
    QVector<myTablePlayListFinal*> &vecotr=m_middwid.m_leftWid.GetStackWid0()->myTablePlayListFinalVector();
    for(int i=0;i<vecotr.count();i++)
    {
        middleLeftStackWidget0 *pStack0=m_middwid.m_leftWid.GetStackWid0();
        disconnect(&vecotr.at(i)->m_Btntable,SIGNAL(sig_addPlayList()),pStack0,SLOT(slot_addPlayListWithRename()));//添加播放列表
        connect(&vecotr.at(i)->m_Btntable,SIGNAL(sig_addPlayList()),pStack0,SLOT(slot_addPlayListWithRename()));

        disconnect(&vecotr.at(i)->m_Btntable,SIGNAL(sig_delPlayList()),pStack0,SLOT(slot_removePlayList()));//移除播放列表
        connect(&vecotr.at(i)->m_Btntable,SIGNAL(sig_delPlayList()),pStack0,SLOT(slot_removePlayList()));

        disconnect(&vecotr.at(i)->m_table,SIGNAL(sig_play(int)),pStack0,SLOT(slot_playIndex(int)));//播放triggered
        connect(&vecotr.at(i)->m_table,SIGNAL(sig_play(int)),pStack0,SLOT(slot_playIndex(int)));

        disconnect(&vecotr.at(i)->m_table,SIGNAL(sig_playMv(int)),pStack0,SLOT(slot_playMVIndex(int)));//播放MV!!
        connect(&vecotr.at(i)->m_table,SIGNAL(sig_playMv(int)),pStack0,SLOT(slot_playMVIndex(int)));

       disconnect(&vecotr.at(i)->m_table.m_playingWid,SIGNAL(sig_clicked(QString)),&m_middwid.m_rightWid,SLOT(slot_search(QString)));//clickand 搜索
       connect(&vecotr.at(i)->m_table.m_playingWid,SIGNAL(sig_clicked(QString)),&m_middwid.m_rightWid,SLOT(slot_search(QString)));//搜索

       disconnect(&vecotr.at(i)->m_table.m_playingWid.m_btnLab,SIGNAL(clicked(bool)),&m_middwid.m_rightWid,SLOT(slot_setLrcShowStack()));
       connect(&vecotr.at(i)->m_table.m_playingWid.m_btnLab,SIGNAL(clicked(bool)),&m_middwid.m_rightWid,SLOT(slot_setLrcShowStack()));

       disconnect(&vecotr.at(i)->m_table,SIGNAL(sig_requestMv(QString)),this,SIGNAL(sig_requestMv(QString)));
       connect(&vecotr.at(i)->m_table,SIGNAL(sig_requestMv(QString)),this,SIGNAL(sig_requestMv(QString)));
    }
}

void mainWindow::readSetting()
{
    QSettings setting("config.ini",QSettings::IniFormat,0);
    setting.beginGroup("mainWindow");

    QString background=setting.value("background").toString();

    int vol=setting.value("volume").toInt();
    if(vol==0)
        vol=80;

    int opacity=setting.value("listopa").toInt();
    if(opacity==0)
        opacity=120;

    int playmodeindex=0;
    playmodeindex=setting.value("playmode").toInt();

    setting.endGroup();

/// setInitSkin
    skinWidgetContentItem*item=(skinWidgetContentItem*)m_skinwid.signalMapper()->mapping(background);
    if(item!=NULL)
        emit item->clicked();
     else
        m_skinwid.setdefaultSkin();
///setInitplayMode
    m_playModeWid.setCurrentMode(PlayMode(playmodeindex));
///setVol
    setCurVol(vol);
///setListOpacity
    m_skinwid.m_sliderWidget->m_slider->setValue(opacity);
}

void mainWindow::saveSetting()
{
     QSettings setting("config.ini",QSettings::IniFormat,0);

    setting.beginGroup("mainWindow");
    setting.setValue("background",m_mainwid-> currentSkinPath());
     setting.setValue("volume",curVol());

    setting.setValue("listopa",m_skinwid.m_sliderWidget->m_slider->value());
    setting.setValue("playmode",pMode);
    setting.endGroup();
}

void mainWindow::setOriginalStatus()
{
    m_bottomwid.slot_setLoveState(false);
    clearBackground();//原始背景
    m_traymenu.setCurrentSongName("极视听 传播好声音");
    m_bottomwid.m_labposition->setText("00:00/00:00");
    m_bottomwid.m_labnowPlayname->setText("极视听音乐");
    m_bottomwid.m_mainslider->setRange(0,0);
   // m_middwid->m_rightWid->m_lrcwid->clearLrc();
   // m_middwid->m_rightWid->m_lrcwid->scrollTo(0);
   // m_deskTopLrc->setOriginalStatus();
}

int mainWindow::curVol()
{
    return  m_volwid.m_slider->value();
}

void mainWindow::setCurVol(int value)
{
    m_volwid.m_slider->setValue(value);
    m_ffplayer.setVol(value);
}

void mainWindow::clearBackground()
{
    m_timeline.stop();
    m_picList.clear();
    m_mainwid->clearBg();
}

inline void mainWindow::setCurBGPic(const QString&strPic)
{
   m_mainwid->setCurBGPic(strPic);
}
void mainWindow::initWidgetMISC()
{
    m_bottomwid.m_btnvol->setPartnerSlider(m_volwid.m_slider);
    m_volwid.hide();
    m_bottomwid.m_btnvol->installEventFilter(this);
    m_volwid.installEventFilter(this);

    connect(m_volwid.m_slider,SIGNAL(valueChanged(int)),m_bottomwid.m_btnvol,SLOT(setButtonPixmap(int)));
    connect(m_bottomwid.m_btnvol,SIGNAL(setMute(int)),m_volwid.m_slider,SLOT(setValue(int)));
    connect(m_bottomwid.m_btnvol,SIGNAL(sig_hideVolWidget()),&m_volwid,SLOT(hide()));
    m_volwid.m_slider->setValue(80);


    m_playModeWid.hide();

    connect(m_bottomwid.m_btnplaymode,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayModeWidget()));
    connect(&m_playModeWid,SIGNAL(sig_CurrentModeChange(PlayMode)),this,SLOT(slot_setPlayMode(PlayMode)));



    m_sertipswid.hide();
    m_topwid.m_lineEdit.installEventFilter(this);
    connect(&m_sertipswid,SIGNAL(itemClicked(QListWidgetItem*)),&m_topwid,SLOT(slot_serTipsClick(QListWidgetItem*)));

    m_skinwid.hide();
    connect(&m_topwid.m_btnskin,SIGNAL(clicked(bool)),&m_skinwid,SLOT(exec()));
    connect(m_skinwid.m_skincontwid,SIGNAL(sig_setBackground(QString)),m_mainwid,SLOT(setSkin(QString)));
    connect(m_skinwid.m_btnAdjustWindow,SIGNAL(clicked(bool)),this,SLOT(slot_adjustWindowNormalSize()));

  /*  m_deskTopLrc=new deskTopLrcWidget;
    m_deskTopLrc->show();
    connect(m_middwid->m_rightWid->m_lrcwid,SIGNAL(sig_currentLrcChange(QString,QString)),m_deskTopLrc,SLOT(slot_currentLrcChange(QString,QString)));
    connect(m_middwid->m_rightWid->m_lrcwid,SIGNAL(sig_currentPrecentChange(QString,float&,int&)),m_deskTopLrc,SLOT(slot_precentChange(QString,float&,int&)));*/
}

void mainWindow::initNetwork()
{
    QThread *thread=new QThread;
    m_net.moveToThread(thread);
    thread->start();

    connect(&m_net,SIGNAL(sig_setBgpix(QStringList,QString)),this,SLOT(slot_setBgPix(QStringList,QString)));
    connect(this,SIGNAL(sig_requestBgPic(QString)),&m_net,SLOT(requestBgPic(QString)));              //request net to search background
    connect(this,SIGNAL(sig_requestAlbum(QString,QString)),&m_net,SLOT(requestalbum(QString,QString)));   //request net to search Album picture
    connect(this,SIGNAL(sig_requestLrc(QString,qint64,QString)),&m_net,SLOT(requestlrc(QString,qint64,QString)));   //request net to search lyrics
    connect(&m_net,SIGNAL(dolrcworkfinished(QByteArray,QString)),this,SLOT(slot_replyLrc(QByteArray,QString)));
    connect(&m_net,SIGNAL(setpic(QString,QString)),m_midstack0,SLOT(slot_setlabelpic(QString,QString)));

    connect(&m_middwid.m_rightWid,SIGNAL(sig_requestSong(QString)),&m_net,SLOT(requestSong(QString)));           //request net to search song
    connect(m_middwid.m_rightWid.getSearWidget(),SIGNAL(sig_requestSongNextPage()),&m_net,SLOT(requestSongNextPage()));

    connect(&m_net,SIGNAL(sig_reqSongNextPagefinished(QByteArray)),m_middwid.m_rightWid.getSearWidget(),SLOT(slot_addRequestSong(QByteArray)));
    connect(&m_net,SIGNAL(sig_reqSongfinished(QByteArray)),m_middwid.m_rightWid.getSearWidget(),SLOT(slot_requestSong(QByteArray)));


    connect(this,SIGNAL(sig_requestMv(QString)),&m_net,SLOT(requestMv(QString)));
    connect(&m_net,SIGNAL(sig_requestMvfinished(QString)),m_midstack0,SLOT(slot_showMvWidget(QString)));

}

void mainWindow::initTimeline()
{
    m_timeline.setCurveShape(QTimeLine::LinearCurve);
    connect(&m_timeline,SIGNAL(frameChanged(int)),SLOT(slot_timelineAnimation(int)));
    m_timeline.setLoopCount(0);
}
bool mainWindow::eventFilter(QObject *o, QEvent *e)
{
    if(o==&m_topwid.m_lineEdit)//lineedit的
    {
        if(e->type()==QEvent::FocusOut)
        {
            m_sertipswid.hide();
        }
        if(e->type()==QEvent::FocusIn)
        {
            m_sertipswid.show();
            m_sertipswid.setGeometry(m_topwid.m_lineEdit.x()+4,m_topwid.m_lineEdit.height()+m_topwid.m_lineEdit.y()+6,m_topwid.m_lineEdit.width(),160);
        }
    }
    if(&m_volwid==o)//music volumn widget
    {
        if(e->type()==QEvent::Enter)
        {
           m_bottomwid.m_btnvol-> m_timer->stop();
        }
        if(e->type()==QEvent::Leave)
        {
            m_bottomwid.m_btnvol-> m_timer->start(500);
        }
    }
    if(m_bottomwid.m_btnvol==o)//music volumn button
    {
        if(e->type()==QEvent::Enter)
        {
            m_volwid.setGeometry(m_bottomwid.m_btnvol->pos().x(),
                                  m_topwid.height()+m_middwid.height()-m_volwid.height()+m_bottomwid.m_btnvol->pos().y(),
                                  30,
                                  310);
            m_volwid.show();
            m_volwid.raise();
        }
    }
    return QObject::eventFilter(o,e);
}
void mainWindow::slot_setPlayMode(PlayMode mode)
{
    pMode=mode;
    m_bottomwid.setPlayModeBtnStyle(PlayMode(mode));
    foreach(myTablePlayListFinal*fin,m_midstack0->myTablePlayListFinalVector())//every single playlist
        fin->setPlayMode(mode);
}

void mainWindow::slot_setPlayModeWidget()
{
  if(!m_playModeWid.hasFocus())
  {
      m_playModeWid.setGeometry(m_bottomwid.m_btnplaymode->x()-(m_playModeWid.width()-m_bottomwid.m_btnplaymode->width())/2,
                                 m_topwid.height()+m_middwid.height()-m_playModeWid.height()+m_bottomwid.m_btnplaymode->height(),
                                 150,
                                 150);
      m_playModeWid.setFocus();
      m_playModeWid.show();
  }
  else
  {
      m_playModeWid.hide();
  }
}
void mainWindow::initConnection()
{
    connect(m_skinwid.m_sliderWidget->m_slider,SIGNAL(valueChanged(int)),&m_middwid.m_leftWid,SLOT(setWidgetOpacity(int)));
    connect(&m_topwid.m_btnmini,SIGNAL(clicked(bool)),SLOT(showMinimized()));
    connect(&m_topwid.m_btnexit,SIGNAL(clicked(bool)),SLOT(close()));

    qRegisterMetaType<PlayerStatus>("PlayerStatus");
    connect(&m_ffplayer,SIGNAL(sig_CurrentMediaError()),&m_ffplayer,SIGNAL(sig_CurrentMediaFinished()));
    connect(&m_ffplayer,SIGNAL(sig_CurrentMediaChange(QString,bool)),this,SLOT(slot_currentMediaChanged(QString,bool)));
    connect(&m_ffplayer,SIGNAL(sig_PositionChange(qint64)),this,SLOT(slot_positionChange(qint64)));
    connect(&m_ffplayer,SIGNAL(sig_PositionChange(qint64)),m_middwid.m_rightWid.getLrcWidget(), SLOT(positionChanged(qint64)));
    connect(m_middwid.m_rightWid.getLrcWidget(), SIGNAL(changeToPosition(qint64)), &m_ffplayer,SLOT(seek(qint64)));
    connect(&m_ffplayer,SIGNAL(sig_CurrentMediaStatus(PlayerStatus)),this,SLOT(slot_playerStatusChanged(PlayerStatus)));
    connect(&m_ffplayer,SIGNAL(sig_CurrentMediaFinished()),m_midstack0,SLOT(slot_endOfMedia()));
    connect(&m_ffplayer,SIGNAL(sig_CurImageChange(QImage)),&m_middwid.m_rightWid,SLOT(slot_imageMV(QImage)));

    connect(m_bottomwid.m_btnnext,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnnextSong()));
    connect(m_bottomwid.m_btnprevious,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnpreSong()));
    connect(m_bottomwid.m_btnPlay,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayerPlayOrPause()));

    connect(m_traymenu.m_wid6->m_backward_button,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnpreSong()));
    connect(m_traymenu.m_wid6->m_forward_button,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnnextSong()));
    connect(m_traymenu.m_wid6->m_play_button,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayerPlayOrPause()));

    connect(m_volwid.m_slider,SIGNAL(valueChanged(int)),&m_ffplayer,SLOT(setVol(int)));
    connect(m_traymenu.m_volWid->m_slider_vol,SIGNAL(valueChanged(int)),m_volwid.m_slider,SLOT(setValue(int)));
    connect(m_traymenu.m_volWid->m_slider_vol,SIGNAL(sliderMoved(int)),m_volwid.m_slider,SLOT(setValue(int)));
    connect(m_volwid.m_slider,SIGNAL(valueChanged(int)),m_traymenu.m_volWid->m_slider_vol,SLOT(setValue(int)));

    connect(&m_topwid.m_lineEdit,SIGNAL(returnPressed()),&m_middwid.m_rightWid,SLOT(slot_setSearchStack()));
    connect(&m_topwid.m_lineEdit,SIGNAL(textChanged(QString)),&m_sertipswid,SLOT(slot_textchanged(QString)));

    connect(trayMenu(),SIGNAL(sig_OpenDeskTopLrc()),this,SLOT(slot_OpenDeskTopLrc()));
    connect(bottomWidget()->m_btnlrc,SIGNAL(clicked(bool)),this,SLOT(slot_OpenDeskTopLrc()));
}
void mainWindow::slot_currentMediaChanged(const QString &media, bool isMV)//setnowplaytext
{
    QString strTemp;
    QString songname;

     clearBackground();//原始背景
   // setOriginalStatus();

    myTablePlayListFinal*finalwid=myTablePlayListFinal::getCurrentList();
    if(media.isEmpty() || !finalwid)//如果为空
        return;

    foreach(myTablePlayListFinal*f,m_midstack0->myTablePlayListFinalVector())
    {
        disconnect(&f->m_table,SIGNAL(sig_setLoveState(bool)),&m_bottomwid,SLOT(slot_setLoveState(bool)));
        disconnect(m_bottomwid.m_btnfavorite,SIGNAL(clicked(bool)),&f->getPlayingWidget()->m_btnLove,SLOT(click()));
    }
    connect(m_bottomwid.m_btnfavorite,SIGNAL(clicked(bool)),&finalwid->getPlayingWidget()->m_btnLove,SLOT(click()));
    connect(&finalwid->m_table,SIGNAL(sig_setLoveState(bool)),&m_bottomwid,SLOT(slot_setLoveState(bool)));
    bool islove= finalwid->m_table.m_playingWid.isLoved();
    m_bottomwid.slot_setLoveState(islove);

    songname= finalwid->currentMusicName();
    m_bottomwid.setCurrentSongName(songname);
    m_traymenu.setCurrentSongName(songname);
    finalwid->setCurrentSongDuration("00:00/00:00");

    if(!isMV)
    {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////请求background
        strTemp= songname.split("-").at(0);
        QDir dir("D:/ExcellentAlbum/"+strTemp);
        dir.mkpath("D:/ExcellentAlbum/"+strTemp);
        QFileInfoList list = dir.entryInfoList();
        dir.setFilter(QDir::Files|QDir::NoSymLinks);
        int file_count = list.size();
        if(file_count==2)//文件夹下为空的话
             emit sig_requestBgPic(strTemp);
         else//本地加载
        {
            m_timeline.setFrameRange(0,file_count-3);
            m_timeline.setDuration((file_count-2)*10000);
            QFileInfo file_info;
            for(int i=0; i<file_count;i++)
            {
                file_info= list.at(i);
                strTemp= file_info.suffix();
                if("jpg"==strTemp)
                {
                    strTemp= file_info.absoluteFilePath();
                    m_picList<<strTemp;
                }
            }
            m_timeline.start();
            setCurBGPic(m_picList.value(0));
        }

        ///////////////////////////////////////////////////////////////////////LRC
            strTemp="";
            strTemp+="D://ExcellentLrc/";
            strTemp+=songname;
            strTemp+=".lrc";
            if(_access(strTemp.toLocal8Bit().data(),0) != -1)//歌词存在
            {
                QFile file(strTemp);
                if(!file.open(QIODevice::ReadOnly))
                    return;
                QByteArray byt=file.readAll();
                m_middwid.m_rightWid.getLrcWidget()->analyzeLrcContent(byt,strTemp);
                file.close();
            }
            else
            {
                QString songdur= finalwid->currentSongDuration();
                QTime time= QTime::fromString(songdur,"mm:ss");
                int dur=time.minute()*60*1000+time.second()*1000;
                emit sig_requestLrc(songname,dur,strTemp);
            }

        ////////////////////////////////////////////////////////////////////////ALBUMS REQUEST

            strTemp="";
            strTemp+="D://ExcellentAlbum/";
            strTemp+=songname;
            strTemp+=".jpg";  //albums url
            finalwid->setCurrentSongAlbumPic(":/image/middlewidget/lab_Author.png");

           if(_access(strTemp.toLocal8Bit().data(),0) != -1)//如果存在专辑图片 加载图片
               finalwid->setCurrentSongAlbumPic(strTemp);
           else
               emit sig_requestAlbum(songname,strTemp);
       //////////////////////////////////////////////////////////////////////////////////////LRC REQUEST
    }
}

void mainWindow::slot_positionChange(qint64 length)
{
    myTablePlayListFinal*finalwid=myTablePlayListFinal::getCurrentList();

    if( !finalwid)
        return;
    int pos=length/1000;
    int dur= m_ffplayer.getDuration()/1000000;
    QTime time;
    time.setHMS(0,dur/60,dur%60);
    QString durstr= time.toString("mm:ss");

    time.setHMS(0,pos/60,pos%60);
    QString posstr= time.toString("mm:ss");

    m_bottomwid.m_labposition->setText(posstr+"/"+durstr);
    finalwid->setCurrentSongDuration(posstr+"/"+durstr);

    m_bottomwid.m_mainslider->setRange(0,dur);
    m_bottomwid.m_mainslider->setValue(pos);
}

void mainWindow::slot_playerStatusChanged(PlayerStatus status)//用于设置图标
{
    if(status==pausingStatus)
    {
        m_bottomwid.setPlayStyle();
        m_traymenu.setPlayStyle();
    }
    if(status==playingStatus)
    {
        m_bottomwid.setPauseStyle();
        m_traymenu.setPauseStyle();
    }
    if(status==bufferingStatus)
    {
        m_bottomwid.setBufferStaus();
    }
}

void mainWindow::slot_adjustWindowNormalSize()
{
    showNormal();
    setGeometry((QApplication::desktop()->width()-1000)/2,(QApplication::desktop()->height()-690)/2,1008,698);
}

void mainWindow::slot_replyLrc(QByteArray lrc, QString strName)
{
    myTablePlayListFinal*pCurList=myTablePlayListFinal::getCurrentList();
    if(!pCurList)
        return;

    if(pCurList->currentMusicName()==strName)
        m_middwid.m_rightWid.getLrcWidget()->analyzeLrcContent(lrc,strName);

}

void mainWindow::initLayout()
{
    QVBoxLayout *vlyout=new QVBoxLayout;
    vlyout->addWidget(&m_topwid);
    vlyout->addWidget(&m_middwid);
    vlyout->addWidget(&m_bottomwid);
    vlyout->setSpacing(0);
    vlyout->setContentsMargins(0,0,0,0);
    m_mainwid->setLayout(vlyout);
//
    m_midstack0=m_middwid.m_leftWid.GetStackWid0();//init this value;
}
void mainWindow::initTrayMenu()
{
    m_system_tray.setContextMenu(&m_traymenu);
    m_system_tray.setToolTip(QString("酷狗音乐"));
    m_system_tray.setIcon(QIcon(":/image/main/trayIcon.png"));
    m_system_tray.show();
    connect(&m_system_tray ,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slot_iconIsActived(QSystemTrayIcon::ActivationReason)));
    connect(&m_traymenu,SIGNAL(sig_close()),this,SLOT(slot_quitApp()));
}
void mainWindow::slot_quitApp()
{
    saveSetting();
    qApp->quit();
}
void mainWindow::slot_iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
     { //点击托盘显示窗口
       case QSystemTrayIcon::Trigger:
       case QSystemTrayIcon::DoubleClick:
         {
            showNormal();
            activateWindow();
            break;
         }
         default:
          break;
     }
}


void mainWindow::closeEvent(QCloseEvent *event)
{
    if(m_system_tray.isVisible())
    {
      hide();
      event->ignore();
    }
    else
    event->accept();
}
mainWindow::~mainWindow()
{
    m_ffplayer.stop();
}
void mainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    if(!isMaximized())
    {
        showMaximized();
        setGeometry(-4,-4,width()+8,height()+8);//最大显示
    }
    else
        showNormal();
}

void mainWindow::slot_currentMediaError()
{
   m_midstack0->slot_endOfMedia();
}

void mainWindow::slot_setPlayerPlayOrPause()
{
    myTablePlayListFinal*finalwid=myTablePlayListFinal::getCurrentList();

    if(!finalwid)
        return;
    if(m_ffplayer.getPlayerStatus()==playingStatus)
    {
        m_ffplayer.pause();
    }
     else if(m_ffplayer.getPlayerStatus()==pausingStatus||m_ffplayer.getPlayerStatus()==bufferingStatus)
    {
        m_ffplayer.play();
    }
}
void mainWindow::slot_OpenDeskTopLrc()
{
   /* if(m_deskTopLrc->isHidden())
        m_deskTopLrc->show();
    else
        m_deskTopLrc->hide();*/
}

void mainWindow::slot_timelineAnimation(int index)
{
    setCurBGPic(m_picList.value(index));
}

void mainWindow::slot_setBgPix(const QStringList& piclist,const QString &name)
{
    myTablePlayListFinal*finalwid=myTablePlayListFinal::getCurrentList();
    if(!finalwid)
        return;

    QString strAuthor=finalwid->currentAuthor();
    if(strAuthor ==  name)
    {
        setCurBGPic(piclist.value(0));
        m_timeline.setFrameRange(0,piclist.count()-1);
        if(piclist.count()!=0)
            m_timeline.setDuration((piclist.count())*10000);

        m_picList=piclist;
        m_timeline.start();
    }
}
