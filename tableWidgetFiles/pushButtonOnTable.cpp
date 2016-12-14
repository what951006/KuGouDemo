#include "pushButtonOnTable.h"
#include<QVBoxLayout>
pushButtonCrossWidget::pushButtonCrossWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlyout=new QVBoxLayout;
    m_btn=new myPushButton(this);
    m_btn->setFixedSize(16,16);
    m_btn->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_cross (1).png);}"
                         "QPushButton:hover{border-image:url(:/image/middlewidget/btn_cross (2).png);}"
                         "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_cross (3).png);}");
    vlyout->addWidget(m_btn,0,Qt::AlignCenter);
    vlyout->setContentsMargins(0,0,0,0);
    setLayout(vlyout);
}
//////////////////////////////////////////////////////////////////////////////////////
pushButtonGroupWidget::pushButtonGroupWidget(QWidget *parent) : QWidget(parent)
{
    m_islove=false;

    QHBoxLayout *hlyout=new QHBoxLayout;
    m_btnLove=new myPushButton(this);
    m_btnLove->setFixedSize(16,16);
    m_btnDel=new myPushButton(this);
    m_btnDel->setFixedSize(16,16);
    m_btnMore=new myPushButton(this);
    m_btnMore->setFixedSize(16,16);
    m_btnMovie=new myPushButton(this);
    m_btnMovie->setFixedSize(16,16);

    m_btnMovie->setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/middlewidget/btn_mv (2).png);}"
                                  "QPushButton:hover{border:NULL;background-image:url(:/image/middlewidget/btn_mv (1).png);}"
                                  "QPushButton:pressed{border:NULL;background-image:url(:/image/middlewidget/btn_mv (3).png);}");

    m_btnLove->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_love (1).png);}"
                             "QPushButton:hover{border-image:url(:/image/middlewidget/btn_love (2).png);}"
                             "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_love (3).png);}");

    m_btnDel->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_del (1).png);}"
                            "QPushButton:hover{border-image:url(:/image/middlewidget/btn_del (2).png);}"
                            "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_del (3).png);}");

    m_btnMore->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_more (1).png);}"
                             "QPushButton:hover{border-image:url(:/image/middlewidget/btn_more (2).png);}"
                             "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_more (3).png);}");

    hlyout->addWidget(m_btnMovie,0,Qt::AlignCenter);
    hlyout->addWidget(m_btnLove,0,Qt::AlignCenter);
    hlyout->addWidget(m_btnDel,0,Qt::AlignCenter);
    hlyout->addWidget(m_btnMore,0,Qt::AlignCenter);
    hlyout->addSpacing(14);
    hlyout->setSpacing(5);
    hlyout->setContentsMargins(0,0,0,0);
    setLayout(hlyout);

    connect(m_btnLove,SIGNAL(clicked(bool)),this,SLOT(slot_btnloveclicked()));
}
void pushButtonGroupWidget::setLoved()
{
    m_islove=true;
    m_btnLove->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_islove (1).png);}"
                                         "QPushButton:hover{border-image:url(:/image/middlewidget/btn_islove (2).png);}"
                                         "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_islove (3).png);}");
}

void pushButtonGroupWidget::slot_btnloveclicked()
{
    if(m_islove)
    {
        m_btnLove->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_love (1).png);}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_love (2).png);}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_love (3).png);}");
    }
    else
    {
        m_btnLove->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_islove (1).png);}"
                                             "QPushButton:hover{border-image:url(:/image/middlewidget/btn_islove (2).png);}"
                                             "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_islove (3).png);}");
    }
    m_islove=!m_islove;
}












