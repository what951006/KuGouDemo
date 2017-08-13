#include "middleWidgets.h"

#include"middlewidgetleft.h"
#include"middleWidgetRight.h"
#include"mainwindow.h"
#include"myPushButton.h"
#include"QHBoxLayout"
#include<QPainter>
middleWidgets::middleWidgets(QWidget *parent)
    : baseWidget(parent),
      m_leftWid(this),
      m_rightWid(this),
      m_btnhidelist(this)
{
    m_rightWid.setInitMiddleWidget(this);//have to init this

    setStyleSheet("baseWidget{background:white;}");
    initLayout();


}
void middleWidgets::initLayout()
{
    QHBoxLayout *hyout=new QHBoxLayout;

    m_btnhidelist.setFixedSize(13,55);
    m_btnhidelist.hide();
    setlistHideStyle();

    hyout->addWidget(&m_leftWid);
    hyout->addWidget(&m_btnhidelist);
    hyout->addWidget(&m_rightWid);
    hyout->setSpacing(0);
    hyout->setContentsMargins(0,0,0,0);
    setLayout(hyout);
    connect(&m_btnhidelist,SIGNAL(clicked(bool)),this,SLOT(slot_btnclicked()));
}


void middleWidgets::setlistShowStyle()
{
    m_btnhidelist.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_listshow (1).png)}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_listshow (2).png)}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_listshow (3).png)}");
}

void middleWidgets::setlistHideStyle()
{
    m_btnhidelist.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_listhide (1).png)}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_listhide (2).png)}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_listhide (3).png)}");
}

void middleWidgets::setBtnShowHide(bool isshow)
{
    if(isshow)
        m_btnhidelist.show();
    else
        m_btnhidelist.hide();

}

void middleWidgets::mouseMoveEvent(QMouseEvent *e)
{
    baseWidget::mouseMoveEvent(e);
}

void middleWidgets::paintEvent(QPaintEvent *e)
{
    baseWidget::paintEvent(e);
  /*  QPainter p(this);
    p.setPen(QColor(230,230,230));
    p.drawLine(QPoint(0,0),QPoint(width(),0));
    p.drawLine(QPoint(0,height()-1),QPoint(width(),height()-1));*/
}

void middleWidgets::slot_btnclicked()
{
    if(m_leftWid.isHidden())//如果是隐藏的
    {
        m_leftWid.show();
         setlistHideStyle();

    }
    else
    {
        m_leftWid.hide();
       setlistShowStyle();
    }
}
