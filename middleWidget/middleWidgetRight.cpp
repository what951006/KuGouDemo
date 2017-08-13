#include "middleWidgetRight.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include<QPainter>
#include<middlewidgetleft.h>

#include"myPushButton.h"
#include"middleSearchWidget.h"
#include"middleWidgets.h"
#include"mainwindow.h"
#include"middleWidgets.h"

#include"Global_ValueGather.h"
QColor middleWidgetRight::bgcolor= QColor(230,230,230);//初始化

middleWidgetRight::middleWidgetRight(QWidget*parent)
    :baseWidget(parent),
     m_stackWid(this),
     m_lrcwid(this),
     m_searchwid(this),
     m_MvWid(this),
     m_wid(this)
{
    m_isdrawline=true;
    init();
    emit m_btnArray[0].click();
}
void middleWidgetRight::init()
{
    setMinimumWidth(690);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *hlyout=new QHBoxLayout;
    QVBoxLayout *vlyout=new QVBoxLayout;

#if 0
    m_stackWid->addWidget(m_web0=new WebWidget(this));
    m_stackWid->addWidget(m_web1=new WebWidget(this));
    m_stackWid->addWidget(m_web2=new WebWidget(this));
    m_stackWid->addWidget(m_web3=new WebWidget(this));
    m_stackWid->addWidget(m_web4=new WebWidget(this));
    m_stackWid->addWidget(m_lrcwid=new LyricLabel(false,this));
    m_stackWid->addWidget(m_searchwid=new middleSearchWidget(this));

    m_web0->m_web->load(QUrl("http://www2.kugou.kugou.com/yueku/v9/html/home.html"));
    m_web0->m_web->settings()->setUserStyleSheetUrl(QUrl("qrc:/css/QWebViewStyle.css"));
    m_web0->m_web->show();

    m_web1->m_web->load(QUrl("http://www2.kugou.kugou.com/fm2/app/publicV2/html/recommend/index.html"));
    m_web1->m_web->settings()->setUserStyleSheetUrl(QUrl("qrc:/css/QWebViewStyle.css"));
    m_web1->m_web->show();

        m_web2->m_web->load(QUrl("http://www2.kugou.kugou.com/yueku/v9/html/default_special.html?ver=8071"));
        m_web2->m_web->settings()->setUserStyleSheetUrl(QUrl("qrc:/css/QWebViewStyle.css"));
        m_web2->m_web->show();

        m_web3->m_web->load(QUrl("http://www2.kugou.kugou.com/mv/v9/html/mvradio.html"));
        m_web3->m_web->settings()->setUserStyleSheetUrl(QUrl("qrc:/css/QWebViewStyle.css"));
        m_web3->m_web->show();

        m_web4->m_web->load(QUrl("http://fanxing.kugou.com/mini/index.html?ver=8071"));
        m_web4->m_web->settings()->setUserStyleSheetUrl(QUrl("qrc:/css/QWebViewStyle.css"));
        m_web4->m_web->show();

 #else
        m_stackWid.addWidget(new baseWidget(this));
        m_stackWid.addWidget(new baseWidget(this));
        m_stackWid.addWidget(new baseWidget(this));
        m_stackWid.addWidget(new baseWidget(this));
        m_stackWid.addWidget(&m_MvWid);
        m_stackWid.addWidget(&m_lrcwid);
        m_stackWid.addWidget(&m_searchwid);
#endif


    m_btnArray[0].setText("乐库");
    m_btnArray[1].setText("电台");
    m_btnArray[2].setText("歌单");
    m_btnArray[3].setText("直播");
    m_btnArray[4].setText("MV");
    m_btnArray[5].setText("歌词");


    m_btnArray[0].setFixedSize(54,40);
    m_btnArray[1].setFixedSize(54,40);
    m_btnArray[2].setFixedSize(54,40);
    m_btnArray[3].setFixedSize(54,40);
    m_btnArray[4].setFixedSize(54,40);
    m_btnArray[5].setFixedSize(54,40);

    m_btnArray[0].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                         "QPushButton:hover{color:rgb(40,143,231);}");
    m_btnArray[1].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                          "QPushButton:hover{color:rgb(40,143,231);}");
    m_btnArray[2].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                          "QPushButton:hover{color:rgb(40,143,231);}");
    m_btnArray[3].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                          "QPushButton:hover{color:rgb(40,143,231);}");
    m_btnArray[4].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                          "QPushButton:hover{color:rgb(40,143,231);}");
    m_btnArray[5].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                          "QPushButton:hover{color:rgb(40,143,231);}");

    hlyout->addStretch(60);
    hlyout->addWidget(&m_btnArray[0],0,Qt::AlignHCenter);
    hlyout->addWidget(&m_btnArray[1],0,Qt::AlignHCenter);
    hlyout->addWidget(&m_btnArray[2],0,Qt::AlignHCenter);
    hlyout->addWidget(&m_btnArray[3],0,Qt::AlignHCenter);
    hlyout->addWidget(&m_btnArray[4],0,Qt::AlignHCenter);
    hlyout->addWidget(&m_btnArray[5],0,Qt::AlignHCenter);

    hlyout->addStretch(60);
    hlyout->setSpacing(25);
    hlyout->setContentsMargins(0,0,0,0);
    for(int i=0;i<6;i++)
    {
       m_btnArray[i].setObjectName(QString::number(i));
       connect(&m_btnArray[i],SIGNAL(clicked()),this,SLOT(slot_btnClick()));
    }


    m_wid.setLayout(hlyout);
    vlyout->addWidget(&m_wid);
    vlyout->addWidget(&m_stackWid);
    vlyout->setSpacing(0);
    vlyout->setContentsMargins(0,0,0,0);
    setLayout(vlyout);
    connect(&m_stackWid,SIGNAL(currentChanged(int)),this,SLOT(slot_curStackChange(int)));
}



void middleWidgetRight::slot_setSearchStack()//搜索界面
{
    QLineEdit *ledit=(QLineEdit*)sender();
    QString text=ledit->text().simplified();
    if(text.isEmpty())
        text=ledit->placeholderText();
    slot_search(text);
}

void middleWidgetRight::slot_search(const QString& text)
{
    m_searchwid.setSearchName(text.simplified());
    m_searchwid.m_checkbox->setCheckState(Qt::Unchecked);
    m_searchwid.setFocus();
    m_searchwid.showLoadingWidget();
    m_searchwid.setRequestisFinished(false); // request the song   we give the bool variable a false value

    for(int i=0;i<6;i++)//do not select
    {
        m_btnArray[i].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                                         "QPushButton:hover{color:rgb(40,143,231);}");
    }
    m_stackWid.setCurrentIndex(6);//the last

    emit sig_requestSong(text.simplified());
}

void middleWidgetRight::slot_setLrcShowStack()
{
    m_stackWid.setCurrentIndex(5);
}

void middleWidgetRight::resizeEvent(QResizeEvent *e)
{
    baseWidget::resizeEvent(e);
    if( m_middlewidget->m_leftWid.isHidden())
    {
        int getwidth= m_middlewidget->width()-m_middlewidget->m_leftWid.width();
        m_wid.setGeometry(m_middlewidget->m_leftWid.width(),0,getwidth,m_wid.height());
    }

}
void middleWidgetRight::slot_curStackChange(int index)
{
    for(int i=0;i<6;i++)
    {
        if(i==index)
            m_btnArray[i].setStyleSheet("QPushButton{border-image: url(:/image/middlewidget/btn_mask.png); color:rgb(40,143,231);font-size:17px;font-family:黑体;}");
        else
            m_btnArray[i].setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:17px;font-family:黑体;}"
                                           "QPushButton:hover{color:rgb(40,143,231);}");
    }
    if(index==5)//lrc widget
    {
      //  mainWindow::GetInstance()->m_mainwid->setCurrentIndex(5);
        m_middlewidget->setStyleSheet("background:transparent;");
        m_middlewidget->m_leftWid.setBackgroundtransparent();
        for(int i=0;i<6;i++)
        {
           m_btnArray[i].setStyleSheet("QPushButton{color:rgb(240,240,240);font-size:17px;font-family:黑体;}"
                                          "QPushButton:hover{color:rgb(40,143,231);}");
        }
        m_btnArray[5].setStyleSheet("QPushButton{border-image: url(:/image/middlewidget/btn_mask.png); color:rgb(40,143,231);font-size:17px;font-family:黑体;}");
        m_middlewidget->setBtnShowHide(true);//the button was used to show or hide the lefttable
        setDrawLine(false);
        update();
    }
    else
    {
       //   mainWindow::GetInstance()->m_mainwid->setCurrentIndex(index);
         m_middlewidget->setStyleSheet("background:white;");
         m_middlewidget->m_leftWid.setBackgroundnormal();

       //   mainWindow::GetInstance()->m_mainwid->setCurrentIndex(index);
         m_middlewidget->setBtnShowHide(false);
         m_middlewidget->m_leftWid.show();
         m_middlewidget->setlistHideStyle();
         setDrawLine(true);
         update();
    }
}

void middleWidgetRight::slot_imageMV(QImage img)
{
    m_MvWid.setImage(img);
}
void middleWidgetRight::slot_btnClick()
{
    int index=sender()->objectName().toInt();
    m_stackWid.setCurrentIndex(index);
}
void middleWidgetRight::paintEvent(QPaintEvent *e)
{
    baseWidget::paintEvent(e);
    if(m_isdrawline)
    {
        QPainter p(this);
        p.setPen(bgcolor);
        p.drawLine(0,39,width(),39);
    }
}
