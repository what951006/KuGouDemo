#include "trayiconmenu.h"
#include<QAction>
#include<QWidgetAction>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<mySlider.h>
#include<QPushButton>
#include"myPushButton.h"
trayIconMenu::trayIconMenu(QWidget*parent)
    :QMenu(parent)
    ,m_volWid(this)
    ,m_wid6(this)
{
   /* setStyleSheet("QMenu{background-color: white;border: 1px solid rgb(214,214,214);border-radius:10px;}"
                  "QMenu::item{height: 28px;padding: 0px 20px 0px 40px;font-size : 12px;color: rgb(102, 102, 102);font-family: 微软雅黑;}"
                  "QMenu::item:focus{padding: -1;}"
                  "QMenu::item:!enabled{color:gray;}"
                  "QMenu::item:hover{color: white;background-color: rgb(22, 154, 243);}"
                  "QMenu::icon{position: absolute;left: 12px;}"
                  "QMenu::separator{height:1px;background: rgb(209,209,209);margin:4px 0px 4px 0px;}");*/

    setFixedWidth(200);
    setContentsMargins(4,5,4,10);
    init_level6();
    init_level5();
    init_level4();
    init_level3();
    init_level2();
    init_level1();
    init_level0();
}
void trayIconMenu::init_level0()
{
    QAction *act_close=new QAction(QIcon(":/image/trayMenu/menu_close.png"),"退出",this);
    addAction(act_close);

    connect(act_close, SIGNAL(triggered()),this,SIGNAL(sig_close()));
}
void trayIconMenu::init_level1()
{
    addSeparator();
    addAction(QIcon(":/image/trayMenu/menu_log.png"),"登录");
}
void trayIconMenu::init_level2()
{
    addSeparator();
    addAction(QIcon(":/image/trayMenu/menu_setting.png"),"选项设置");
}
void trayIconMenu::init_level3()
{
    addAction(QIcon(":/image/trayMenu/menu_lock.png"),"锁定歌词");
}
void trayIconMenu::init_level4()
{
    addSeparator();
    QAction *act=new QAction(QIcon(":/image/trayMenu/menu_lrc.png"),"显示桌面歌词",this);
    addAction(act);
    connect(act,SIGNAL(triggered(bool)),this,SIGNAL(sig_OpenDeskTopLrc()));
}

void trayIconMenu::init_level5()
{
    addSeparator();

    QWidgetAction *action_wid=new QWidgetAction(this);
    action_wid->setDefaultWidget(&m_volWid);
    addAction(action_wid);
    m_volWid.raise();
}
void trayIconMenu::init_level6()
{
    QWidgetAction *action_wid=new QWidgetAction(this);
    action_wid->setDefaultWidget(&m_wid6);
    addAction(action_wid);
}

void trayIconMenu::setCurrentSongName(QString songname)
{
    m_wid6.music_name_label.setText(songname);
}

void trayIconMenu::setPauseStyle()
{
    m_wid6.m_play_button.setStyleSheet("QPushButton{border-image:url(:/image/trayMenu/menu_pause (1).png);}"
                                                   "QPushButton::hover{border-image:url(:/image/trayMenu/menu_pause (2).png);}"
                                                   "QPushButton::pressed{border-image:url(:/image/trayMenu/menu_pause (3).png);}");
}

void trayIconMenu::setPlayStyle()
{
    m_wid6.m_play_button.setStyleSheet("QPushButton{border-image:url(:/image/trayMenu/menu_play (1).png);}"
                                       "QPushButton::hover{border-image:url(:/image/trayMenu/menu_play (2).png);}"
                                       "QPushButton::pressed{border-image:url(:/image/trayMenu/menu_play (3).png);}");
}
//////////////////////////////////////////////////////////////////////////////////////////////////
volWidget::volWidget(QWidget *parent)
    :QWidget(parent)
    ,m_slider_vol(Qt::Horizontal,this)
{
    QHBoxLayout*lyout=new QHBoxLayout;

    m_slider_vol.setFixedSize(140,20);
    m_slider_vol.setStyleSheet("QSlider::groove:horizontal{border-radius:2px;height:2px;}"
                                "QSlider::sub-page:horizontal{background:rgb(104,104,104);}"
                                "QSlider::add-page:horizontal{background:rgb(209,209,209);}"
                                "QSlider::handle:horizontal{background:rgb(104,104,104);width:8px;border-radius:4px;margin:-3px 0px -3px 0px;}");
    m_slider_vol.setCursor(Qt::PointingHandCursor);
    volButton *btn=new volButton(":/image/trayMenu/menu_vol (1).png",":/image/trayMenu/menu_vol (2).png",":/image/trayMenu/menu_vol (3).png",this);
    btn->setParentSlider(&m_slider_vol);

    btn->setFixedSize(16,16);
    lyout->addSpacing(12);
    lyout->addWidget(btn,0,Qt::AlignLeft|Qt::AlignVCenter);
    lyout->addSpacing(0);
    lyout->addWidget(&m_slider_vol,0,Qt::AlignLeft|Qt::AlignVCenter);
    lyout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Fixed));
    lyout->setContentsMargins(0,0,0,0);
    setLayout(lyout);
    setFixedSize(190,30);
    connect(&m_slider_vol,SIGNAL(valueChanged(int)),btn,SLOT(setButtonPixmap(int)));
    connect(btn,SIGNAL(setMute(int)),&m_slider_vol,SLOT(setValue(int)));
    m_slider_vol.setValue(80);
}
//////////////////////////////////////////////////////////////////////////////////////////////
playWidget::playWidget(QWidget *parent)
    :QWidget(parent)
    ,m_play_button (this)
    ,m_backward_button (this)
    ,m_forward_button(this)
    ,music_name_label(this)
{
    music_name_label.setStyleSheet("color: rgb(134,134,134);");
    music_name_label.setFixedHeight(20);

    m_play_button.setCursor(QCursor(Qt::PointingHandCursor));
    m_backward_button.setCursor(QCursor(Qt::PointingHandCursor));
    m_forward_button.setCursor(QCursor(Qt::PointingHandCursor));

    m_play_button.setFlat(true);
    m_backward_button.setFlat(true);
    m_forward_button.setFlat(true);
    m_play_button.setStyleSheet("QPushButton{border-image:url(:/image/trayMenu/menu_play (1).png);}"
                               "QPushButton::hover{border-image:url(:/image/trayMenu/menu_play (2).png);}"
                               "QPushButton::pressed{border-image:url(:/image/trayMenu/menu_play (3).png);}");

    m_backward_button.setStyleSheet("QPushButton{border-image:url(:/image/trayMenu/menu_previous (1).png);}"
                                   "QPushButton::hover{border-image:url(:/image/trayMenu/menu_previous (2).png);}"
                                   "QPushButton::pressed{border-image:url(:/image/trayMenu/menu_previous (3).png);}");

    m_forward_button.setStyleSheet("QPushButton{border-image:url(:/image/trayMenu/menu_next (1).png);}"
                                  "QPushButton::hover{border-image:url(:/image/trayMenu/menu_next (2).png);}"
                                  "QPushButton::pressed{border-image:url(:/image/trayMenu/menu_next (3).png);}");

    m_play_button.setFixedSize(32,32);
    m_backward_button.setFixedSize(32,32);
    m_forward_button.setFixedSize(32,32);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(&m_backward_button);
    layout->addWidget(&m_play_button);
    layout->addWidget(&m_forward_button);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);

    QVBoxLayout *layout2 = new QVBoxLayout();

    layout2->addLayout(layout);
    layout2->addWidget(&music_name_label,0,Qt::AlignHCenter);
    layout2->setSpacing(2);
    layout2->setContentsMargins(0,2,0,0);
    music_name_label.setText("极视听 传播好音乐");
    music_name_label.setEnabled(false);
    setLayout(layout2);
    setFixedSize(190,65);
}
