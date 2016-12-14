#include "middleLeftStackWidget3.h"
#include"qlabel.h"
#include<QVBoxLayout>
#include"myPushButton.h"
middleLeftStackWidget3::middleLeftStackWidget3(QWidget*parent):baseWidget(parent)
{

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    initLayout();
}
void middleLeftStackWidget3::initLayout()
{
    QVBoxLayout *vLyout=new QVBoxLayout;

    QFont font;
    font.setFamily("黑体");
    font.setPixelSize(18);

    QLabel *label2=new QLabel("请选择连接手机的方式",this);
    label2->adjustSize();
    label2->setAlignment(Qt::AlignHCenter);
    label2->setStyleSheet("color:rgb(68,68,68);");
    label2->setFont(font);

    myPushButton *btn=new myPushButton(this);
    btn->setFixedSize(206,100);
    btn->setStyleSheet("QPushButton{border-image: url(:/image/middlewidget/wire_connect (1).png);}"
                       "QPushButton:hover{border-image: url(:/image/middlewidget/wire_connect (2).png);}"
                       "QPushButton:pressed{border-image: url(:/image/middlewidget/wire_connect (3).png);}");

    myPushButton *btn2=new myPushButton(this);
    btn2->setFixedSize(206,100);
    btn2->setStyleSheet("QPushButton{border-image: url(:/image/middlewidget/net_connect (1).png);}"
                       "QPushButton:hover{border-image: url(:/image/middlewidget/net_connect (2).png);}"
                       "QPushButton:pressed{border-image: url(:/image/middlewidget/net_connect (3).png);}");


    vLyout->addSpacerItem(new QSpacerItem(10,65,QSizePolicy::Expanding,QSizePolicy::Expanding));
    vLyout->addWidget(label2,0,Qt::AlignHCenter);
    vLyout->addSpacing(50);
    vLyout->addWidget(btn,0,Qt::AlignHCenter);
    vLyout->addSpacing(30);
    vLyout->addWidget(btn2,0,Qt::AlignHCenter);

    vLyout->addSpacerItem(new QSpacerItem(10,170,QSizePolicy::Expanding,QSizePolicy::Expanding));
    vLyout->setContentsMargins(0,0,0,0);
    setLayout(vLyout);

}


