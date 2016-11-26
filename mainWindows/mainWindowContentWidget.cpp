#include "mainWindowContentWidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>

#include"myMediaList.h"
playModeWidget::playModeWidget(QWidget*p):baseWidget(p)
{
    setStyleSheet("QWidget{background-color:rgb(64,64,64,200);border-radius:5px;}");
    setFixedSize(100,150);
    init();
}

void  playModeWidget::init()
{
    QVBoxLayout *vlyout=new QVBoxLayout;
    m_btnOrder=new playModeButton(":/image/bottomwidget/btn_listcircle (1).png",":/image/bottomwidget/btn_listcircle (2).png","列表循环",this);
    m_btnRandom=new playModeButton(":/image/bottomwidget/btn_listrandom (1).png",":/image/bottomwidget/btn_listrandom (2).png","随机播放",this);
    m_btnOneCircle=new playModeButton(":/image/bottomwidget/btn_listscircle_single (1).png",":/image/bottomwidget/btn_listscircle_single (2).png","单曲循环",this);
    m_btnOrder->setPlayMode(PlayMode::playInOrder);
    m_btnRandom->setPlayMode(PlayMode::playRandom);
    m_btnOneCircle->setPlayMode(PlayMode::playOneCircle);

    vlyout->addWidget(m_btnOrder);
    vlyout->addWidget(m_btnRandom);
    vlyout->addWidget(m_btnOneCircle);
    vlyout->setContentsMargins(0,0,0,0);
    setLayout(vlyout);

    m_vector<<m_btnOrder<<m_btnRandom<<m_btnOneCircle;

    connect(m_btnOrder,SIGNAL(clicked(bool)),this,SLOT(slot_setCurrentMode()));
    connect(m_btnRandom,SIGNAL(clicked(bool)),this,SLOT(slot_setCurrentMode()));
    connect(m_btnOneCircle,SIGNAL(clicked(bool)),this,SLOT(slot_setCurrentMode()));
}

void playModeWidget::setCurrentMode(PlayMode mode)
{
    for(int i=0;i<m_vector.size();i++)
    {
        if(mode==m_vector.value(i)->playMode())
        {
            m_vector.value(i)->setCheckedBtn(true);
        }
        else
        {
             m_vector.value(i)->setCheckedBtn(false);
        }
    }
    emit sig_CurrentModeChange(mode);
}

void playModeWidget::slot_setCurrentMode()
{
   playModeButton*btn=(playModeButton*)sender();
   setCurrentMode(btn->playMode());
}

/////////////////////////////////////////////////////////////////
volSliderWidget::volSliderWidget(QWidget *p):baseWidget(p)
{
    QHBoxLayout *hlyout=new QHBoxLayout;
    setStyleSheet("QWidget{background-color:rgb(64,64,64,200);border-radius:5px;}");
    m_slider=new mySlider(Qt::Vertical,this);

    m_slider->setStyleSheet("QSlider{background:transparent;}"
                            "QSlider::groove:vertical{background:transparent;border-radius:2px;width:3px;}"
                            "QSlider::sub-page:vertical{background:rgb(122,122,122);}"
                            "QSlider::add-page:vertical{background:rgb(255, 255, 160);}"
                            "QSlider::handle:vertical{background:rgb(255, 255, 160); height: 8px; border-radius: 4px; margin-left: -3px;  margin-right: -3px;  }");
    m_slider->setFixedSize(12,85);

    hlyout->addWidget(m_slider);
    hlyout->setContentsMargins(0,0,0,0);
    setLayout(hlyout);
    setFixedSize(30,110);
    connect(m_slider,SIGNAL(valueChanged(int)),this,SLOT(slot_sliderMove(int)));

}
bool volSliderWidget::eventFilter(QObject *o, QEvent *e)
{
    return QObject::eventFilter(o,e);
}

AddLoveListTips::AddLoveListTips(QWidget *p):baseWidget(p)
{
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timerWork()));
    m_timer->setInterval(3000);
    m_timer->setSingleShot(true);


    setStyleSheet("QWidget{background:rgb(254,249,180)}"

                  "QLabel{background:rgb(254,249,180);color: rgb(55,55,55);font-size:13px;}");
    setFixedHeight(34);

    QLabel *label=new QLabel(this);
    m_closebtn=new myPushButton(this);
    m_tipslab=new QLabel(this);

    label->setFixedSize(16,16);
    m_closebtn->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_addtips (1).png);}"
                              "QPushButton::hover{border-image:url(:/image/middlewidget/btn_addtips (2).png);}"
                              "QPushButton::pressed{border-image:url(:/image/middlewidget/btn_addtips (3).png);}");

    label->setPixmap(QPixmap(":/image/middlewidget/lab_success.png"));
    m_closebtn->setFixedSize(16,16);
    QHBoxLayout *hlyout=new QHBoxLayout;
    hlyout->addWidget(label);
    hlyout->addWidget(m_tipslab);
    hlyout->addWidget(m_closebtn);
    hlyout->setContentsMargins(5,2,5,2);
    hlyout->setSpacing(5);
    setLayout(hlyout);
    connect(m_closebtn,SIGNAL(clicked(bool)),this,SLOT(hide()));
}

void AddLoveListTips::showAddTips()
{
    m_tipslab->setText("已成功添加到我的最爱列表!");
    show();
}

void AddLoveListTips::showRemoveTips()
{
    m_tipslab->setText("已成功从我的最爱列表移除!");
    show();
}

void AddLoveListTips::slot_timerWork()
{
    m_timer->stop();
    hide();
}

void AddLoveListTips::showEvent(QShowEvent *e)
{
    baseWidget::showEvent(e);
    m_timer->stop();
    m_timer->start();

}
