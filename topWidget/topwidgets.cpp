#include "topwidgets.h"
#include<QPushButton>
#include<QLabel>
#include<QHBoxLayout>
#include<QDebug>
#include<QFile>
#include<QApplication>


TopWidgets::TopWidgets(QWidget *parent) : baseWidget(parent)
{
    setStyleSheet("TopWidgets{background:transparent;}");
    setMouseTracking(true);
    initWidget();
    m_mainWindow=(mainWindow*)parent;
}
void TopWidgets::slot_serTipsClick(QListWidgetItem *it)
{
    searchFromLineEdit(it->text());
}

bool TopWidgets::eventFilter(QObject *o, QEvent *e)
{
    return QObject::eventFilter(o,e);
}

void TopWidgets::mouseMoveEvent(QMouseEvent *e)
{
    baseWidget::mouseMoveEvent(e);
   // m_mainWindow->mouseMoveEvent(e);
}

void TopWidgets::initWidget()
{
    setMinimumSize(1000,50);
    setMaximumSize(1920,50);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    QHBoxLayout *hmainyout=new QHBoxLayout;
///////////////////////////////////////////////////////////////////////////////////////////////////////////first lyout

    QHBoxLayout *hyout1=new QHBoxLayout;
    myPushButton*btn=new myPushButton(this);
    btn->setFixedSize(30,30);
    btn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/image/topwidget/label_icon.png)}");
    m_btnlog=new myPushButton("登陆",this);
    m_btnregister=new myPushButton("注册",this);
    m_btnlog->setFixedSize(26,24);
    m_btnregister->setFixedSize(26,24);
    m_btnlog->setStyleSheet("QPushButton{background:transparent;color:white;font-family:宋体;font-size:12px;}");
    m_btnregister->setStyleSheet("QPushButton{background:transparent;color:white;font-family:宋体;font-size:12px;}");
    QLabel*label2=new QLabel("丨",this);
    label2->setFixedSize(10,20);
    label2->setStyleSheet("color:white;");
    label2->adjustSize();
    hyout1->addWidget(btn);
    hyout1->addSpacing(6);
    hyout1->addWidget(m_btnlog);
    hyout1->addWidget(label2);
    hyout1->addWidget(m_btnregister);
    hyout1->setSpacing(5);
    hyout1->setContentsMargins(0,0,0,0);
////////////////////////////////////////////////////////////////////////////////////////////////////////////second lyout
    QHBoxLayout *hyout2=new QHBoxLayout;
    myPushButton *m_btnreturn=new myPushButton(this);
    m_btnreturn->setFixedSize(20,20);
    m_btnreturn->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_ return(1).png);}"
                               "QPushButton::hover{border-image:url(:/image/topwidget/btn_ return(2).png);}"
                               "QPushButton::pressed{border-image:url(:/image/topwidget/btn_ return(3).png);}");
    myPushButton *m_btnrefresh=new myPushButton(this);
    m_btnrefresh->setFixedSize(20,20);
    m_btnrefresh->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_refresh(1).png);}"
                                "QPushButton::hover{border-image:url(:/image/topwidget/btn_refresh(2).png);}"
                                "QPushButton::pressed{border-image:url(:/image/topwidget/btn_refresh(3).png);}");
    m_lineEdit=new QLineEdit(this);
    m_lineEdit->setMinimumSize(260,26);
    m_lineEdit->setMaximumSize(320,26);
    m_lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_lineEdit->setStyleSheet("QLineEdit{border-radius:13px;background:rgb(255,255,255,180);}");
    m_lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    myPushButton *serbtn=new myPushButton(m_lineEdit);
    serbtn->setFixedSize(16,16);
    serbtn->setStyleSheet("QPushButton{background:transparent;border-image: url(:/image/topwidget/btn_search (1).png);}"
                          "QPushButton::hover{border-image: url(:/image/topwidget/btn_search (2).png);}"
                          "QPushButton::pressed{border-image: url(:/image/topwidget/btn_search (3).png);}");
    m_lineEdit->setPlaceholderText("周杰伦-东风破");
    QMargins margins = m_lineEdit->textMargins();
    m_lineEdit->setTextMargins(margins.left()+15, margins.top(), serbtn->width()+15, margins.bottom());


    QHBoxLayout *pSearchLayout = new QHBoxLayout(this);
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(serbtn);
    pSearchLayout->setSpacing(0);
    pSearchLayout->setContentsMargins(0,0,15,0);
    m_lineEdit->setLayout(pSearchLayout);


    hyout2->addWidget(m_btnreturn);
    hyout2->addWidget(m_btnrefresh);
    hyout2->addWidget(m_lineEdit);
    hyout2->setSpacing(20);
    hyout2->setContentsMargins(0,0,0,0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////third lyout
    QHBoxLayout *hyout3=new QHBoxLayout;
    m_btnexit=new myPushButton(this);
    m_btnmini=new myPushButton(this);
    m_btnminiframe=new myPushButton(this);
    m_btnmobile=new myPushButton(this);
    m_btnsetting=new myPushButton(this);
    m_btnskin=new myPushButton(this);
    m_btnmessage=new myPushButton(this);
    m_btnplaycard=new myPushButton(this);
    m_btntool=new myPushButton(this);
    m_btngame=new myPushButton(this);

    m_btnexit->setFixedSize(18,18);
    m_btnmini->setFixedSize(16,16);
    m_btnminiframe->setFixedSize(16,16);
    m_btnmobile->setFixedSize(16,16);
    m_btnsetting->setFixedSize(16,16);
    m_btnskin->setFixedSize(16,16);
    m_btnmessage->setFixedSize(16,16);
    m_btnplaycard->setFixedSize(16,16);
    m_btntool->setFixedSize(16,16);
    m_btngame->setFixedSize(16,16);

    m_btnexit->setObjectName("m_btnexit");
    m_btnmini->setObjectName("m_btnmini");
    m_btnminiframe->setObjectName("m_btnminiframe");
    m_btnmobile->setObjectName("m_btnmobile");
    m_btnsetting->setObjectName("m_btnsetting");
    m_btnskin->setObjectName("m_btnskin");
    m_btnmessage->setObjectName("m_btnmessage");
    m_btnplaycard->setObjectName("m_btnplaycard");
    m_btntool->setObjectName("m_btntool");
    m_btngame->setObjectName("m_btngame");
#if 1
    m_btnexit->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_close (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/topwidget/btn_close (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/topwidget/btn_close (3).png);}");
    m_btnmini->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_mini (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/topwidget/btn_mini (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/topwidget/btn_mini (3).png);}");
    m_btnminiframe->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_miniframe (1).png);}"
                                  "QPushButton::hover{border-image:url(:/image/topwidget/btn_miniframe (2).png);}"
                                  "QPushButton::pressed{border-image:url(:/image/topwidget/btn_miniframe (3).png);}");
    m_btnmobile->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_mobile (1).png);}"
                               "QPushButton::hover{border-image:url(:/image/topwidget/btn_mobile (2).png);}"
                               "QPushButton::pressed{border-image:url(:/image/topwidget/btn_mobile (3).png);}");
    m_btnsetting->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_setting (1).png);}"
                                "QPushButton::hover{border-image:url(:/image/topwidget/btn_setting (2).png);}"
                                "QPushButton::pressed{border-image:url(:/image/topwidget/btn_setting (3).png);}");
    m_btnskin->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_skin (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/topwidget/btn_skin (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/topwidget/btn_skin (3).png);}");
    m_btnmessage->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_message (1).png);}"
                                "QPushButton::hover{border-image:url(:/image/topwidget/btn_message (2).png);}"
                                "QPushButton::pressed{border-image:url(:/image/topwidget/btn_message (3).png);}");
    m_btnplaycard->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_playcard (1).png);}"
                                 "QPushButton::hover{border-image:url(:/image/topwidget/btn_playcard (2).png);}"
                                 "QPushButton::pressed{border-image:url(:/image/topwidget/btn_playcard (3).png);}");
    m_btntool->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_tool (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/topwidget/btn_tool (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/topwidget/btn_tool (3).png);}");
   m_btngame->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_game (1).png);}"
                             "QPushButton::hover{border-image:url(:/image/topwidget/btn_game (2).png);}"
                             "QPushButton::pressed{border-image:url(:/image/topwidget/btn_game (3).png);}");

#endif
    QLabel*label3=new QLabel("丨",this);
    label3->setFixedSize(6,16);
    label3->setStyleSheet("color:rgb(255,255,255,50);");
    label3->adjustSize();

    hyout3->addWidget(m_btntool);
    hyout3->addWidget(m_btnplaycard);
    hyout3->addWidget(m_btngame);
    hyout3->addWidget(m_btnmessage);

    hyout3->addWidget(m_btnskin);
    hyout3->addWidget(m_btnsetting);
    hyout3->addWidget(label3);
    hyout3->addWidget(m_btnmobile);
    hyout3->addWidget(m_btnminiframe);
    hyout3->addWidget(m_btnmini);
    hyout3->addWidget(m_btnexit);
    hyout3->setSpacing(12);


    hmainyout->addLayout(hyout1);
    hmainyout->addSpacing(190);
    hmainyout->addSpacerItem(new QSpacerItem(80,30,QSizePolicy::Maximum));//设置它最大膨胀！！！
    hmainyout->addLayout(hyout2);
    hmainyout->addSpacerItem(new QSpacerItem(110,30,QSizePolicy::Expanding));//膨胀
    //hmainyout->insertStretch(3,150);
    hmainyout->addLayout(hyout3);
    hmainyout->setSpacing(0);
    hmainyout->setContentsMargins(16,0,15,0);
    setLayout(hmainyout);

    connect(serbtn,SIGNAL(clicked(bool)),m_lineEdit,SIGNAL(returnPressed()));

}

void TopWidgets::searchFromLineEdit(const QString str)
{
    m_lineEdit->setText(str);
    emit m_lineEdit->returnPressed();
}
