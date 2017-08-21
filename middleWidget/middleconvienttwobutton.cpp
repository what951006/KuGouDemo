#include "middleconvienttwobutton.h"
#include"myPushButton.h"

#include<QHBoxLayout>

middleConvientTwoButton::middleConvientTwoButton(QWidget*p)
    :baseWidget(p)
    ,m_btnsearch(this)
    ,m_btnlocate(this)
{


     m_btnsearch.setGeometry(5,5,24,24);
     m_btnlocate.setGeometry(35,5,24,24);

     setFixedSize(65,34);
     setStyleSheet("baseWidget{background: rgb(220,220,220,150);border:1px; border-radius: 17px;}");

     m_btnsearch.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_search (1).png)}"
                                "QPushButton:hover{border-image:url(:/image/middlewidget/btn_search (2).png)}"
                                "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_search (3).png)}");

     m_btnlocate.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_location (1).png)}"
                                "QPushButton:hover{border-image:url(:/image/middlewidget/btn_location (2).png)}"
                                "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_location (3).png)}");

     m_timer.setSingleShot(true);
     connect(&m_timer,SIGNAL(timeout()),this,SLOT(slot_timerEvent()));
}

/////////////////////////////////////////////////////////////////////////////////////////
middleListSearch::middleListSearch(QWidget *p)
    :baseWidget(p)
    ,m_lineEdit(this)
    ,m_btnclose(this)
{
    setFixedHeight(32);
    setStyleSheet("QWidget{background: rgb(180,180,180,180);}");


    m_lineEdit.setPlaceholderText("查找所有列表歌曲");
    m_lineEdit.setTextMargins(10,0,0,0);

    m_lineEdit.setMinimumWidth(260);
    m_lineEdit.setFixedHeight(22);
    m_btnclose.setFixedSize(20,20);

    m_lineEdit.setStyleSheet("QLineEdit{border-radius:11px;background:rgb(255,255,255,180);}");
    m_btnclose.setStyleSheet("QPushButton{background:transparent; border-image:url(:/image/topwidget/btn_close (1).png);}"
                              "QPushButton::hover{border-image:url(:/image/topwidget/btn_close (2).png);}"
                              "QPushButton::pressed{border-image:url(:/image/topwidget/btn_close (3).png);}");


    QHBoxLayout *hlyout=new QHBoxLayout;
    hlyout->addWidget(&m_lineEdit);
    hlyout->addWidget(&m_btnclose);
    hlyout->setContentsMargins(10,5,10,5);
    setLayout(hlyout);
    connect(&m_btnclose,SIGNAL(clicked(bool)),this,SLOT(hide()));
    connect(&m_btnclose,SIGNAL(clicked(bool)),this,SLOT(setClear()));
}

