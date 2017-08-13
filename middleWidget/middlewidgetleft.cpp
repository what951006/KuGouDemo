#include "middlewidgetleft.h"

#include"middleLeftStackWidget0.h"
#include"middleLeftStackWidget1.h"
#include"middleLeftStackWidget2.h"
#include"middleLeftStackWidget3.h"
#include"middleLeftStackWidget4.h"
#include"mainwindow.h"

#include<QVBoxLayout>
#include<QPaintEvent>
#include<QPainter>
#include<QScrollBar>
#include<QDebug>

QColor middleWidgetLeft::color=QColor(230,230,230);
QColor middleWidgetLeft::bgcolor=QColor(255,255,255,100);

middleWidgetLeft::middleWidgetLeft(QWidget *parent)
    : baseWidget(parent),
      m_Swidget0(this),
      m_Swidget1(this),
      m_Swidget2(this),
      m_Swidget3(this),
      m_Swidget4(this)
{
    m_isDrawVerticalLine=true;
    setMinimumWidth(310);
    setMaximumWidth(380);
    setMinimumHeight(570);
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);

    initLayout();
    initAnimation();
}
void middleWidgetLeft::initAnimation()
{
    m_pix=QPixmap(":/image/middlewidget/indicator.png");
    m_index=-1;
    m_preindex=-1;
    m_preItem=0;
    m_x=0;
    m_isAnima=true;
    m_animation=new QPropertyAnimation(this,"m_x");
    m_animation->setDuration(200);
    m_stackwid.setCurrentIndex(0);
    connect(m_animation,SIGNAL(finished()),this,SLOT(slot_finished()));

    m_Widanimation=new QPropertyAnimation(this,"geometry");
    m_Widanimation->setTargetObject(&m_stackwid);
    m_Widanimation->setDuration(200);
}



void middleWidgetLeft::setBackgroundtransparent()
{
    color=QColor(55,55,55,55);
    m_Swidget0.updateBGColor();
    setDrawVerticalLine(false);
    update();
}

void middleWidgetLeft::setBackgroundnormal()
{
    color=QColor(230,230,230);
    m_Swidget0.updateBGColor();
    setDrawVerticalLine(true);
    update();
}
void middleWidgetLeft::setWidgetOpacity(int value)
{
     middleWidgetLeft::bgcolor=QColor(255,255,255,value);
     update();
}
void middleWidgetLeft::initLayout()
{
    QVBoxLayout *vlyout=new QVBoxLayout;
    m_stackwid.setFrameShadow(QFrame::Plain);
    m_stackwid.setFrameShape(QFrame::NoFrame);

    m_stackwid.addWidget(&m_Swidget0);
    m_stackwid.addWidget(&m_Swidget1);
    m_stackwid.addWidget(&m_Swidget2);
    m_stackwid.addWidget(&m_Swidget3);
    m_stackwid.addWidget(&m_Swidget4);

    m_stackwid.setContentsMargins(0,0,0,0);


    m_btn[0]=new stackButton(":/image/middlewidget/btn_music (1).png",":/image/middlewidget/btn_music (2).png",":/image/middlewidget/btn_music (3).png",this);
    m_btn[1]=new stackButton(":/image/middlewidget/btn_cloud (1).png",":/image/middlewidget/btn_cloud (2).png",":/image/middlewidget/btn_cloud (3).png",this);
    m_btn[2]=new stackButton(":/image/middlewidget/btn_radio (1).png",":/image/middlewidget/btn_radio (2).png",":/image/middlewidget/btn_radio (3).png",this);
    m_btn[3]=new stackButton(":/image/middlewidget/btn_phone (1).png",":/image/middlewidget/btn_phone (2).png",":/image/middlewidget/btn_phone (3).png",this);
    m_btn[4]=new stackButton(":/image/middlewidget/btn_download (1).png",":/image/middlewidget/btn_download (2).png",":/image/middlewidget/btn_download (3).png",this);

    m_btn[0]->setFixedHeight(40);
    m_btn[1]->setFixedHeight(40);
    m_btn[2]->setFixedHeight(40);
    m_btn[3]->setFixedHeight(40);
    m_btn[4]->setFixedHeight(40);

    QHBoxLayout *hlyout=new QHBoxLayout;
    hlyout->addWidget(m_btn[0]);
    hlyout->addWidget(m_btn[1]);
    hlyout->addWidget(m_btn[2]);
    hlyout->addWidget(m_btn[3]);
    hlyout->addWidget(m_btn[4]);
    hlyout->setContentsMargins(0,0,0,0);
    hlyout->setSpacing(0);

    vlyout->addLayout(hlyout);
    vlyout->addWidget(&m_stackwid);
    vlyout->setSpacing(0);
    vlyout->setContentsMargins(0,0,0,0);
    setLayout(vlyout);

    for(int i=0;i<5;i++)
    {
        m_btn[i]->setObjectName(QString::number(i));
        connect(m_btn[i],SIGNAL(pressed()),this,SLOT(slot_btn()));
    }

    connect(&m_stackwid,SIGNAL(currentChanged(int)),this,SLOT(slot_changeButtonSelected(int)));

    m_stackwid.setCurrentIndex(0);//选中第一个stackwidget
    m_btn[0]->setselected(true); //设置
}
void middleWidgetLeft::slot_changeButtonSelected(int index)
{
    m_isAnima=true;
    m_index=index;
    m_animation->setStartValue(m_preindex*m_btn[0]->width());
    m_animation->setEndValue(index*m_btn[0]->width());
    m_animation->start();

    for(int i=0;i<5;i++)
    {
        if(i==index)
        {
           m_btn[index]->setselected(true);
        }
        else
        {
          m_btn[i]->setselected(false);
        }
    }

    if(index>m_preItem)
    {
        m_Widanimation->setTargetObject(&m_stackwid);
        m_Widanimation->setStartValue(QRect(m_stackwid.width(),40,m_stackwid.width(),m_stackwid.height()));
        m_Widanimation->setEndValue(QRect(0,40,m_stackwid.width(),m_stackwid.height()));
        m_Widanimation->start();
    }
    if(index<m_preItem)
    {
        m_Widanimation->setTargetObject(&m_stackwid);
        m_Widanimation->setStartValue(QRect(-m_stackwid.width(),40,m_stackwid.width(),m_stackwid.height()));
        m_Widanimation->setEndValue(QRect(0,40,m_stackwid.width(),m_stackwid.height()));
        m_Widanimation->start();
    }
    m_preItem=index;
}
void middleWidgetLeft::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(Qt::transparent);
    p.setBrush(bgcolor);//刷透明区域
    p.drawRect(-1,-1,width(),height()+1);
    p.setPen(color);

   if(m_isAnima)
    {
       if(m_isDrawVerticalLine)
       p.drawLine(width()-1,0,width()-1,height());//vertical line

        p.drawLine(0,m_btn[0]->y()+m_btn[0]->height()-1,m_x+(m_btn[0]->width()-m_pix.width())/2-1,m_btn[0]->y()+m_btn[0]->height()-1);//first line
        p.drawLine(m_x+(m_btn[0]->width()-m_pix.width())/2+m_pix.width(),m_btn[0]->y()+m_btn[0]->height()-1,width(),m_btn[0]->y()+m_btn[0]->height()-1);//second line
        p.drawPixmap(m_x+(m_btn[0]->width()-m_pix.width())/2,m_btn[0]->y()+m_btn[0]->height()-m_pix.height()+1,m_pix);
    }
    else
    {
       if(m_isDrawVerticalLine)
       p.drawLine(width()-1,0,width()-1,height());

       p.drawLine(0,m_btn[0]->y()+m_btn[0]->height()-1,m_index*m_btn[0]->width()+(m_btn[0]->width()-m_pix.width())/2-1,m_btn[0]->y()+m_btn[0]->height()-1);
       p.drawLine(m_index*m_btn[0]->width()+(m_btn[0]->width()-m_pix.width())/2+m_pix.width(),m_btn[0]->y()+m_btn[0]->height()-1,width(),m_btn[0]->y()+m_btn[0]->height()-1);
       p.drawPixmap(m_index*m_btn[0]->width()+(m_btn[0]->width()-m_pix.width())/2,m_btn[0]->y()+m_btn[0]->height()-m_pix.height()+1,m_pix);
    }
}
void middleWidgetLeft::slot_btn()
{
    m_preindex=m_stackwid.currentIndex();
    m_stackwid.setCurrentIndex(sender()->objectName().toInt());
}
void middleWidgetLeft::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}
