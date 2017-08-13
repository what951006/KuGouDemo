#include "bottomWidgets.h"
#include<QHBoxLayout>
#include<QTime>

#include "myTablePlayListFinal.h"
#include"middleLeftStackWidget0.h"
#include"mainwindow.h"
#include"FFmpegPlayer.h"
bottomWidgets::bottomWidgets(QWidget *parent) : baseWidget(parent)
{
    init();
    setMouseTracking(true);
}
void bottomWidgets::init()
{
    setStyleSheet("bottomWidgets{background:transparent;}");
    setMinimumSize(1000,66);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QHBoxLayout *mainlyout=new QHBoxLayout;
////////////////////////////////////////////////////////////////////firstlayout
    QHBoxLayout *layout=new QHBoxLayout;
    m_btnnext=new myPushButton(this);
    m_btnprevious=new myPushButton(this);
    m_btnPlay=new myPushButton(this);

    m_btnnext->setFixedSize(44,44);
    m_btnprevious->setFixedSize(44,44);
    m_btnPlay->setFixedSize(44,44);

    m_btnnext->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_next (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_next (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_next (3).png);}");
    m_btnprevious->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_previous (1).png);}"
                                 "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_previous (2).png);}"
                                 "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_previous (3).png);}");
    m_btnPlay->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_play (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_play (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_play (3).png);}");

    layout->addWidget(m_btnprevious);
    layout->addWidget(m_btnPlay);
    layout->addWidget(m_btnnext);
    layout->setSpacing(15);
    layout->setContentsMargins(0,0,0,0);
    /////////////////////////////////////////////////////////////////////////secondlayout
    QHBoxLayout *layout2=new QHBoxLayout;
    m_mainslider=new mySlider(Qt::Horizontal,this);
    m_mainslider->installEventFilter(this);
    m_mainslider->setRange(0,1000);
    m_mainslider->setMinimumSize(403,12);
    m_mainslider->setMaximumHeight(12);
    m_mainslider->setStyleSheet("QSlider::groove:horizontal{border-radius:2px;height:3px;}"
                               "QSlider::sub-page:horizontal{background:rgb(255, 255, 160);}"
                               "QSlider::add-page:horizontal{background:rgb(200,200,209);}"
                               "QSlider::handle:horizontal{background:rgb(255, 255, 160);width:8px;border-radius:4px;margin:-3px 0px -3px 0px;}");

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(14);
    font.setBold(true);

    QHBoxLayout *layout2_1=new QHBoxLayout;
    m_labnowPlayname=new TextMovingWidget("极视听音乐",this);
    m_labnowPlayname->setMinimumWidth(250);
    m_labnowPlayname->setFixedHeight(30);
    m_labnowPlayname->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_labnowPlayname->setFont(font);

    font.setPixelSize(12);
    font.setBold(false);
    m_labposition=new QLabel("00:00/00:00",this);
    m_labposition->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    m_labposition->setFixedSize(80,24);
    m_labposition->setFont(font);

    layout2_1->addWidget(m_labnowPlayname);
    layout2_1->addWidget(m_labposition);

    QVBoxLayout *vlayout=new QVBoxLayout;
    vlayout->addLayout(layout2_1);
    vlayout->addWidget(m_mainslider);
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0,0,0,10);
    layout2->addLayout(vlayout);

    //////////////////////////////////////////////////////////////////////////////////////thirdlayout
    QHBoxLayout *layout3=new QHBoxLayout;

     m_btnplaylist=new myPushButton(this);
     m_btnlrc=new myPushButton(this);
     m_btnequalizer=new myPushButton("",this);
     m_btnvol=new volButton2(":/image/bottomwidget/btn_vol (1).png",":/image/bottomwidget/btn_vol (2).png",":/image/bottomwidget/btn_vol (3).png",this);
     m_btnplaymode=new myPushButton(this);
     m_btnmore=new myPushButton(this);
     m_btndownload=new myPushButton(this);
     m_btnfavorite=new myPushButton(this);

     m_btnplaylist->setFixedSize(60,20);
     m_btnlrc->setFixedSize(20,20);
     m_btnequalizer->setFixedSize(48,20);
     m_btnvol->setFixedSize(20,20);
     m_btnplaymode->setFixedSize(20,20);
     m_btnmore->setFixedSize(20,20);
     m_btndownload->setFixedSize(20,20);
     m_btnfavorite->setFixedSize(20,20);


     m_btnplaylist->setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/bottomwidget/btn_miniplaylist (1).png);}"
                            "QPushButton::hover{background-image:url(:/image/bottomwidget/btn_miniplaylist (2).png);}"
                            "QPushButton::pressed{background-image:url(:/image/bottomwidget/btn_miniplaylist (3).png);}");
     m_btnlrc->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_lrc (1).png);}"
                            "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_lrc (2).png);}"
                            "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_lrc (3).png);}");

     m_btnequalizer->setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/bottomwidget/btn_equalizer (1).png);}"
                               "QPushButton::hover{background-image:url(:/image/bottomwidget/btn_equalizer (2).png);}"
                               "QPushButton::pressed{background-image:url(:/image/bottomwidget/btn_equalizer (3).png);}");

     m_btnplaymode->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_listrandom (1).png);}"
                            "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_listrandom (2).png);}" );

     m_btnmore->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_more (1).png);}"
                              "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_more (2).png);}"
                              "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_more (3).png);}");

     m_btndownload->setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/bottomwidget/btn_download (1).png);}"
                                  "QPushButton::hover{background-image:url(:/image/bottomwidget/btn_download (2).png);}"
                                  "QPushButton::pressed{background-image:url(:/image/bottomwidget/btn_download (3).png);}");

     m_btnfavorite->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_favorite_no (1).png);}"
                            "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_favorite_no (2).png);}"
                            "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_favorite_no (3).png);}");


    layout3->addWidget(m_btnfavorite);
    layout3->addWidget(m_btndownload);
    layout3->addWidget(m_btnmore);
    layout3->addWidget(m_btnplaymode);
    layout3->addWidget(m_btnvol);
    layout3->addWidget(m_btnequalizer);
    layout3->addWidget(m_btnlrc);
    layout3->addWidget(m_btnplaylist);

    layout3->setSpacing(18);
    layout3->setContentsMargins(0,0,0,0);
    ////////////////////////////////////////
    mainlyout->addLayout(layout);
    mainlyout->addLayout(layout2);
    mainlyout->addLayout(layout3);
    mainlyout->setSpacing(30);
    mainlyout->setContentsMargins(12,0,0,0);
    setLayout(mainlyout);
}

void bottomWidgets::setRandomStyle()
{
    m_btnplaymode->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_listrandom (1).png);}"
                                 "QPushButton:hover{border-image:url(:/image/bottomwidget/btn_listrandom (2).png);}");
}

void bottomWidgets::setInOrderStyle()
{
    m_btnplaymode->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_listcircle (1).png);}"
                                 "QPushButton:hover{border-image:url(:/image/bottomwidget/btn_listcircle (2).png);}");
}

void bottomWidgets::setOneCycleStyle()
{
    m_btnplaymode->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_listscircle_single (1).png);}"
                                 "QPushButton:hover{border-image:url(:/image/bottomwidget/btn_listscircle_single (2).png);}");
}

void bottomWidgets::setPlayModeBtnStyle(PlayMode mode)
{
    if(mode==PlayMode::playInOrder)
    {
        setInOrderStyle();
    }
    else if(mode==PlayMode::playRandom)
    {
        setRandomStyle();
    }
    else if(mode==PlayMode::playOneCircle)
    {
        setOneCycleStyle();
    }
    else
    {
        setInOrderStyle();
    }
}

 void bottomWidgets::setPauseStyle()
{
    m_btnPlay->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_pause (1).png);}"
                           "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_pause (2).png);}"
                           "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_pause (3).png);}");
     m_labnowPlayname->setText(m_labnowPlayname->Text().remove("缓冲中---->> "));

}

 void bottomWidgets::setPlayStyle()
{
    m_btnPlay->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_play (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_play (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_play (3).png);}");
    m_labnowPlayname->setText(m_labnowPlayname->Text().remove("缓冲中---->> "));
 }

 void bottomWidgets::setBufferStaus()
 {
   /*  if(!m_labnowPlayname->Text().contains(QString("缓冲中---->> ")))
     m_labnowPlayname->setText(QString("缓冲中---->> ")+m_labnowPlayname->Text());*/

 }

void bottomWidgets::setCurrentSongName(const QString& str)
{
    m_labnowPlayname->setText(str);
}

void bottomWidgets::setLoveState()
{
    m_btnfavorite->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_favorite_yes (1).png);}"
                           "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_favorite_yes (2).png);}"
                           "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_favorite_yes (3).png);}");
}

void bottomWidgets::setUnLoveState()
{
    m_btnfavorite->setStyleSheet("QPushButton{border-image:url(:/image/bottomwidget/btn_favorite_no (1).png);}"
                           "QPushButton::hover{border-image:url(:/image/bottomwidget/btn_favorite_no (2).png);}"
                           "QPushButton::pressed{border-image:url(:/image/bottomwidget/btn_favorite_no (3).png);}");

}

bool bottomWidgets::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_mainslider)
    {
        QMouseEvent *ev=static_cast<QMouseEvent*>(e);
        if(e->type()==QEvent::MouseButtonPress)
        {
            if(ev->button()==Qt::LeftButton)
            {
                int dur = m_mainslider->maximum()- m_mainslider->minimum();
                int pos = m_mainslider->minimum() + dur * ((double)ev->x() / m_mainslider->width());
                if(pos != m_mainslider->sliderPosition())
                 {
                    if(myTablePlayListFinal::getCurrentList())
                    {
                        m_mainslider->setValue(pos);
                        mainWindow::GetInstance()->player()->seek(m_mainslider->value()*AV_TIME_BASE);
                    }
                 }
            }
        }
    }
    return QObject::eventFilter(o,e);
}

void bottomWidgets::mouseMoveEvent(QMouseEvent *e)
{
    baseWidget::mouseMoveEvent(e);
}

void bottomWidgets::slot_setLoveState(bool islove)
{
    if(islove)
    {
        setLoveState();
    }
        else
    {
        setUnLoveState();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











