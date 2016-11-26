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
#include"lyriclabel.h"
#include"skinWidget.h"
#include"deskTopLrcWidget.h"
#include"Global_ValueGather.h"

mainWindow::mainWindow(QWidget *parent):baseWindow(parent)//设置背景就设置m_mainwid就行了
{
     mainwid=this;//global value
     m_border=8;
     setMinimumSize(1008,698);
     m_mainwid->setStyleSheet("QLabel{color:white;}");//"Widget{background: rgb(0,140,230);}"//border-image:url(:/image/skin/1.png)

     initLayout();//加载主要的layout
     initMediaPlayer();
     initNetwork();
     initTrayMenu();//加载系统图标菜单
     initWidgetMISC();
     initTimeline();
     initConnection();
     readSetting();
}
void mainWindow::readSetting()
{
    m_midstack0->setOriginalStatus();

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
    skinWidgetContentItem*item=(skinWidgetContentItem*)m_skinwid->signalMapper()->mapping(background);
    if(item!=NULL)
        emit item->clicked();
     else
        m_skinwid->setdefaultSkin();
///setInitplayMode
    m_playModeWid->setCurrentMode(PlayMode(playmodeindex));
///setVol
    setCurVol(vol);
///setListOpacity
    m_skinwid->m_sliderWidget->m_slider->setValue(opacity);
}

void mainWindow::saveSetting()
{
     QSettings setting("config.ini",QSettings::IniFormat,0);

    setting.beginGroup("mainWindow");
    setting.setValue("background",m_mainwid->currentSkinPath());
     setting.setValue("volume",curVol());

    setting.setValue("listopa",m_skinwid->m_sliderWidget->m_slider->value());
    setting.setValue("playmode",pMode);
    setting.endGroup();
}

void mainWindow::initMediaPlayer()
{
    m_ffplayer=new FFmpegPlayer(this);
}

void mainWindow::setOriginalStatus()
{
    m_bottomwid->slot_setLoveState(false);
    clearBackground();//原始背景
    m_traymenu->setCurrentSongName("极视听 传播好声音");
    m_bottomwid->m_mainslider->setValue(0);
    m_bottomwid->m_labposition->setText("00:00/00:00");
    m_bottomwid->m_labnowPlayname->setText("极视听音乐");
    m_bottomwid->m_mainslider->setRange(0,0);
    m_middwid->m_rightWid->m_lrcwid->clearLrc();
    m_middwid->m_rightWid->m_lrcwid->scrollTo(0);
    m_deskTopLrc->setOriginalStatus();
}

int mainWindow::curVol()
{
    return  m_volwid->m_slider->value();
}

void mainWindow::setCurVol(int value)
{
    m_volwid->m_slider->setValue(value);
    m_ffplayer->setVol(value);
}

void mainWindow::clearBackground()
{
    m_timeline->stop();
    m_pixvector.clear();
    m_mainwid->clearBg();
}

void mainWindow::setBackgroundPixmap(const QPixmap &pixmap)
{
   m_mainwid->setPixmap(pixmap);
}
void mainWindow::initWidgetMISC()
{
    m_volwid=new volSliderWidget(m_mainwid);
    m_bottomwid->m_btnvol->setPartnerSlider(m_volwid->m_slider);
    m_volwid->hide();
    m_bottomwid->m_btnvol->installEventFilter(this);
    m_volwid->installEventFilter(this);

    connect(m_volwid->m_slider,SIGNAL(valueChanged(int)),m_bottomwid->m_btnvol,SLOT(setButtonPixmap(int)));
    connect(m_bottomwid->m_btnvol,SIGNAL(setMute(int)),m_volwid->m_slider,SLOT(setValue(int)));
    connect(m_bottomwid->m_btnvol,SIGNAL(sig_hideVolWidget()),m_volwid,SLOT(hide()));
    m_volwid->m_slider->setValue(80);

    m_playModeWid=new playModeWidget(m_mainwid);
    m_playModeWid->hide();

    connect(m_bottomwid->m_btnplaymode,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayModeWidget()));
    connect(m_playModeWid,SIGNAL(sig_CurrentModeChange(PlayMode)),this,SLOT(slot_setPlayMode(PlayMode)));


    m_sertipswid=new topSearchTipsWidget(this);
    m_sertipswid->hide();
    m_topwid->m_lineEdit->installEventFilter(this);
    connect(m_sertipswid,SIGNAL(itemClicked(QListWidgetItem*)),m_topwid,SLOT(slot_serTipsClick(QListWidgetItem*)));

    m_skinwid=new skinWidget;
    m_skinwid->hide();
    connect(m_topwid->m_btnskin,SIGNAL(clicked(bool)),m_skinwid,SLOT(exec()));
    connect(m_skinwid->m_skincontwid,SIGNAL(sig_setBackground(QString)),m_mainwid,SLOT(setSkin(QString)));
    connect(m_skinwid->m_btnAdjustWindow,SIGNAL(clicked(bool)),this,SLOT(slot_adjustWindowNormalSize()));

    m_deskTopLrc=new deskTopLrcWidget;
    m_deskTopLrc->show();
    connect(m_middwid->m_rightWid->m_lrcwid,SIGNAL(sig_currentLrcChange(QString,QString)),m_deskTopLrc,SLOT(slot_currentLrcChange(QString,QString)));
    connect(m_middwid->m_rightWid->m_lrcwid,SIGNAL(sig_currentPrecentChange(QString,float&,int&)),m_deskTopLrc,SLOT(slot_precentChange(QString,float&,int&)));
}

void mainWindow::initNetwork()
{
    m_net=new MyNetWork();

    qRegisterMetaType<QVector<QPixmap>>("QVector<QPixmap>");

    connect(m_net,SIGNAL(sig_setBgpix(QVector<QPixmap>,QString)),this,SLOT(slot_setBgPix(QVector<QPixmap>,QString)));
    connect(this,SIGNAL(sig_requestBgPic(QString)),m_net,SLOT(requestBgPic(QString)));              //request net to search background
    connect(this,SIGNAL(sig_requestAlbum(QString,QString)),m_net,SLOT(requestalbum(QString,QString)));   //request net to search Album picture
    connect(this,SIGNAL(sig_requestLrc(QString,qint64,QString)),m_net,SLOT(requestlrc(QString,qint64,QString)));   //request net to search lyrics
    connect(m_net,SIGNAL(dolrcworkfinished(QByteArray,QString)),m_middwid->m_rightWid->m_lrcwid,SLOT(slot_replyLrc(QByteArray,QString)));
    connect(m_net,SIGNAL(setpic(QByteArray,QString)),m_midstack0,SLOT(slot_setlabelpic(QByteArray,QString)));

    connect(m_middwid->m_rightWid,SIGNAL(sig_requestSong(QString)),m_net,SLOT(requestSong(QString)));           //request net to search song
    connect(m_middwid->m_rightWid->m_searchwid,SIGNAL(sig_requestSongNextPage()),m_net,SLOT(requestSongNextPage()));

    connect(m_net,SIGNAL(sig_reqSongNextPagefinished(QByteArray)),m_middwid->m_rightWid->m_searchwid,SLOT(slot_addRequestSong(QByteArray)));
    connect(m_net,SIGNAL(sig_reqSongfinished(QByteArray)),m_middwid->m_rightWid->m_searchwid,SLOT(slot_requestSong(QByteArray)));


    connect(this,SIGNAL(sig_requestMv(QString)),m_net,SLOT(requestMv(QString)));
    connect(m_net,SIGNAL(sig_requestMvfinished(QString)),m_midstack0,SLOT(slot_showMvWidget(QString)));

}

void mainWindow::initTimeline()
{
    m_timeline=new QTimeLine(100000);
    m_timeline->setCurveShape(QTimeLine::LinearCurve);
    connect(m_timeline,SIGNAL(frameChanged(int)),SLOT(slot_timelineAnimation(int)));
    m_timeline->setLoopCount(0);
}
bool mainWindow::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_topwid->m_lineEdit)//lineedit的
    {
        if(e->type()==QEvent::FocusOut)
        {
            m_sertipswid->hide();
        }
        if(e->type()==QEvent::FocusIn)
        {
            m_sertipswid->show();
            m_sertipswid->setGeometry(m_topwid->m_lineEdit->x()+4,m_topwid->m_lineEdit->height()+m_topwid->m_lineEdit->y()+6,m_topwid->m_lineEdit->width(),160);
        }
    }
    if(m_volwid==o)//music volumn widget
    {
        if(e->type()==QEvent::Enter)
        {
           m_bottomwid->m_btnvol-> m_timer->stop();
        }
        if(e->type()==QEvent::Leave)
        {
            m_bottomwid->m_btnvol-> m_timer->start(500);
        }
    }
    if(m_bottomwid->m_btnvol==o)//music volumn button
    {
        if(e->type()==QEvent::Enter)
        {
            m_volwid->setGeometry(m_bottomwid->m_btnvol->pos().x(),
                                  m_topwid->height()+m_middwid->height()-m_volwid->height()+m_bottomwid->m_btnvol->pos().y(),
                                  30,
                                  310);
            m_volwid->show();
            m_volwid->raise();
        }
    }
    return QObject::eventFilter(o,e);
}
void mainWindow::slot_setPlayMode(PlayMode mode)
{
    pMode=mode;
    m_bottomwid->setPlayModeBtnStyle(PlayMode(mode));
    foreach(myTablePlayListFinal*fin,m_midstack0->myTablePlayListFinalVector())//every single playlist
        fin->setPlayMode(mode);
}

void mainWindow::slot_setPlayModeWidget()
{
  if(!m_playModeWid->hasFocus())
  {
      m_playModeWid->setGeometry(m_bottomwid->m_btnplaymode->x()-(m_playModeWid->width()-m_bottomwid->m_btnplaymode->width())/2,
                                 m_topwid->height()+m_middwid->height()-m_playModeWid->height()+m_bottomwid->m_btnplaymode->height(),
                                 150,
                                 150);
      m_playModeWid->setFocus();
      m_playModeWid->show();
  }
  else
  {
      m_playModeWid->hide();
  }
}
void mainWindow::initConnection()
{
    connect(m_skinwid->m_sliderWidget->m_slider,SIGNAL(valueChanged(int)),m_middwid->m_leftWid,SLOT(setWidgetOpacity(int)));
    connect(m_topwid->m_btnmini,SIGNAL(clicked(bool)),SLOT(showMinimized()));
    connect(m_topwid->m_btnexit,SIGNAL(clicked(bool)),SLOT(close()));

    qRegisterMetaType<PlayerStatus>("PlayerStatus");
    connect(m_ffplayer,SIGNAL(sig_CurrentMediaError()),m_ffplayer,SIGNAL(sig_CurrentMediaFinished()));
    connect(m_ffplayer,SIGNAL(sig_CurrentMediaChange(QString)),this,SLOT(slot_currentMediaChanged(QString)));
    connect(m_ffplayer,SIGNAL(sig_PositionChange(qint64)),this,SLOT(slot_positionChange(qint64)));
    connect(m_ffplayer,SIGNAL(sig_PositionChange(qint64)),m_middwid->m_rightWid->m_lrcwid, SLOT(positionChanged(qint64)));
    connect(m_middwid->m_rightWid->m_lrcwid, SIGNAL(changeToPosition(qint64)), m_ffplayer,SLOT(seek(qint64)));
    connect(m_ffplayer,SIGNAL(sig_CurrentMediaStatus(PlayerStatus)),this,SLOT(slot_playerStatusChanged(PlayerStatus)));
    connect(m_ffplayer,SIGNAL(sig_CurrentMediaFinished()),m_midstack0,SLOT(slot_endOfMedia()));

    connect(m_bottomwid->m_btnnext,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnnextSong()));
    connect(m_bottomwid->m_btnprevious,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnpreSong()));
    connect(m_bottomwid->m_btnPlay,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayerPlayOrPause()));

    connect(m_traymenu->m_wid6->m_backward_button,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnpreSong()));
    connect(m_traymenu->m_wid6->m_forward_button,SIGNAL(clicked(bool)),m_midstack0,SLOT(slot_btnnextSong()));
    connect(m_traymenu->m_wid6->m_play_button,SIGNAL(clicked(bool)),this,SLOT(slot_setPlayerPlayOrPause()));

    connect(m_volwid->m_slider,SIGNAL(valueChanged(int)),m_ffplayer,SLOT(setVol(int)));
    connect(m_traymenu->m_volWid->m_slider_vol,SIGNAL(valueChanged(int)),m_volwid->m_slider,SLOT(setValue(int)));
    connect(m_traymenu->m_volWid->m_slider_vol,SIGNAL(sliderMoved(int)),m_volwid->m_slider,SLOT(setValue(int)));
    connect(m_volwid->m_slider,SIGNAL(valueChanged(int)),m_traymenu->m_volWid->m_slider_vol,SLOT(setValue(int)));

    connect(m_topwid->m_lineEdit,SIGNAL(returnPressed()),m_middwid->m_rightWid,SLOT(slot_setSearchStack()));
    connect(m_topwid->m_lineEdit,SIGNAL(textChanged(QString)),m_sertipswid,SLOT(slot_textchanged(QString)));

    connect(trayMenu(),SIGNAL(sig_OpenDeskTopLrc()),this,SLOT(slot_OpenDeskTopLrc()));
    connect(bottomWidget()->m_btnlrc,SIGNAL(clicked(bool)),this,SLOT(slot_OpenDeskTopLrc()));
}
void mainWindow::slot_currentMediaChanged(const QString &media)//setnowplaytext
{
    setOriginalStatus();

    myTablePlayListFinal*finalwid=m_midstack0->nowPlayFinalTable();
    if(media.isEmpty()||!m_midstack0->nowPlayFinalTable())//如果为空
        return;
    foreach(myTablePlayListFinal*f,m_midstack0->myTablePlayListFinalVector())
    {
        disconnect(f->m_table,SIGNAL(sig_setLoveState(bool)),m_bottomwid,SLOT(slot_setLoveState(bool)));
        disconnect(m_bottomwid->m_btnfavorite,SIGNAL(clicked(bool)),f->getPlayingWidget()->m_btnLove,SLOT(click()));
    }
    connect(m_bottomwid->m_btnfavorite,SIGNAL(clicked(bool)),m_midstack0->nowPlayFinalTable()->getPlayingWidget()->m_btnLove,SLOT(click()));
    connect(m_midstack0->nowPlayFinalTable()->m_table,SIGNAL(sig_setLoveState(bool)),m_bottomwid,SLOT(slot_setLoveState(bool)));
    bool islove= finalwid->m_table->m_playingWid->isLoved();
    m_bottomwid->slot_setLoveState(islove);

    QString songname= finalwid->currentSongName();
    m_bottomwid->setCurrentSongName(songname);
    m_traymenu->setCurrentSongName(songname);
    m_midstack0->nowPlayFinalTable()->setCurrentSongDuration("00:00/00:00");

///////////////////////////////////////////////////////////////////////////////////////////////////////请求background
    QString author= songname.split("-").at(0);
    QDir dir("D:/ExcellentAlbum/"+author);
    dir.mkpath("D:/ExcellentAlbum/"+author);
    QFileInfoList list = dir.entryInfoList();
    dir.setFilter(QDir::Files|QDir::NoSymLinks);
    int file_count = list.size();
    if(file_count==2)//文件夹下为空的话
         emit sig_requestBgPic(author);
     else//本地加载
    {
        m_timeline->setFrameRange(0,file_count-3);
        m_timeline->setDuration((file_count-2)*10000);
        QFileInfo file_info;
        for(int i=0; i<file_count;i++)
        {
            file_info= list.at(i);
            QString suffix = file_info.suffix();
            if(suffix==QString("jpg"))
            {
                QString absolute_file_path = file_info.absoluteFilePath();
                m_pixvector<<QPixmap(absolute_file_path);
            }
        }

        m_timeline->start();
        setBackgroundPixmap(m_pixvector.value(0));
    }
///////////////////////////////////////////////////////////////////////Execellentlrc文件夹下的
    QString path2lrc="D://ExcellentLrc/"+songname+".lrc";   //lrc url
    QString ablumurl="D://ExcellentAlbum/"+songname+".jpg";  //albums url
    QFile file3(path2lrc);//歌词文件第二个
////////////////////////////////////////////////////////////////////////ALBUMS REQUEST
   finalwid->setCurrentSongAlbumPic(QPixmap(":/image/middlewidget/lab_Author.png"));
   QFile file(ablumurl);
   if(file.exists())//如果存在专辑图片 加载图片
       finalwid->setCurrentSongAlbumPic(QPixmap(ablumurl));
   else
        emit sig_requestAlbum(songname,ablumurl);
//////////////////////////////////////////////////////////////////////////////////////LRC REQUEST

    if(file3.exists())//如果存在在D:ExcellentLrc中的话
      {
        if(!file3.open(QIODevice::ReadOnly))
            return;
          QByteArray byt=file3.readAll();
          file3.close();
          m_middwid->m_rightWid->m_lrcwid->analyzeLrc(byt,path2lrc);
      }
      else//如果两个地方都没有歌词文件
      {
        QString songdur= finalwid->currentSongDuration();
        QTime time= QTime::fromString(songdur,"mm:ss");
        int dur=time.minute()*60*1000+time.second()*1000;
        emit sig_requestLrc(songname,dur,path2lrc);
      }
    file3.close();
}

void mainWindow::slot_positionChange(qint64 length)
{
    if( m_midstack0->nowPlayFinalTable()==NULL)
        return;
    int pos=length/1000000;
    int dur= m_ffplayer->getDuration()/1000000;
    QTime time;
    time.setHMS(0,dur/60,dur%60);
    QString durstr= time.toString("mm:ss");

    time.setHMS(0,pos/60,pos%60);
    QString posstr= time.toString("mm:ss");


    m_bottomwid->m_labposition->setText(posstr+"/"+durstr);
    m_midstack0->nowPlayFinalTable()->setCurrentSongDuration(posstr+"/"+durstr);

    m_bottomwid->m_mainslider->setRange(0,dur);
    m_bottomwid->m_mainslider->setValue(pos);
}

void mainWindow::slot_playerStatusChanged(PlayerStatus status)//用于设置图标
{
    if(status==pausingStatus)
    {
        m_bottomwid->setPlayStyle();
        m_traymenu->setPlayStyle();
    }
    if(status==playingStatus)
    {
        m_bottomwid->setPauseStyle();
        m_traymenu->setPauseStyle();
    }
    if(status==bufferingStatus)
    {
        m_bottomwid->setBufferStaus();
    }
}

void mainWindow::slot_adjustWindowNormalSize()
{
    showNormal();
    setGeometry((QApplication::desktop()->width()-1000)/2,(QApplication::desktop()->height()-690)/2,1008,698);
}

void mainWindow::initLayout()
{
    m_topwid=new TopWidgets(this);
    m_middwid=new middleWidgets(this);
    m_bottomwid=new bottomWidgets(this);
    m_middwid->setInitParent(this);//do passing the pointer;
    m_bottomwid->setInitMainWindow(this); //do passing the pointer;

    QVBoxLayout *vlyout=new QVBoxLayout;
    vlyout->addWidget(m_topwid);
    vlyout->addWidget(m_middwid);
    vlyout->addWidget(m_bottomwid);
    vlyout->setSpacing(0);
    vlyout->setContentsMargins(0,0,0,0);
    m_mainwid->setLayout(vlyout);

    m_midstack0=m_middwid->m_leftWid->m_Swidget0;//init this value;
}
void mainWindow::initTrayMenu()
{
    system_tray = new QSystemTrayIcon(this);
    m_traymenu=new trayIconMenu;
    system_tray->setContextMenu(m_traymenu);
    system_tray ->setToolTip(QString("酷狗音乐"));
    system_tray ->setIcon(QIcon(":/image/main/trayIcon.png"));
    system_tray->show();
    connect(system_tray ,SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slot_iconIsActived(QSystemTrayIcon::ActivationReason)));
    connect(m_traymenu,SIGNAL(sig_close()),this,SLOT(slot_quitApp()));
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
    if(system_tray->isVisible())
    {
      hide();
      event->ignore();
    }
    else
    event->accept();
}
mainWindow::~mainWindow()
{
    delete system_tray;

    m_ffplayer->stop();
    m_ffplayer->deleteLater();
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
    if(m_midstack0->nowPlayFinalTable()==NULL)
        return;
    if(m_ffplayer->getPlayerStatus()==playingStatus)
    {
        m_ffplayer->pause();
    }
     else if(m_ffplayer->getPlayerStatus()==pausingStatus||m_ffplayer->getPlayerStatus()==bufferingStatus)
    {
        m_ffplayer->play();
    }
}
void mainWindow::slot_OpenDeskTopLrc()
{
    if(m_deskTopLrc->isHidden())
        m_deskTopLrc->show();
    else
        m_deskTopLrc->hide();
}

void mainWindow::slot_timelineAnimation(int value)
{
    setBackgroundPixmap(m_pixvector.value(value));
}

void mainWindow::slot_setBgPix(const QVector<QPixmap> &pixvector,const QString &name)
{
    if(m_midstack0->nowPlayFinalTable()!=NULL)
    {
        QString songname=m_midstack0->nowPlayFinalTable()->currentSongName();
        if(songname.split("-").at(0)==name)
        {
            setBackgroundPixmap(pixvector.value(0));
            m_timeline->setFrameRange(0,pixvector.count()-1);
            if(pixvector.count()!=0)
            m_timeline->setDuration((pixvector.count())*10000);

            m_pixvector=pixvector;
            m_timeline->start();
        }
    }
}
