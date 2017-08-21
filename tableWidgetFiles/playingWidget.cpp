#include "playingWidget.h"

#include"mytablewidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QDebug>
playingWidget::playingWidget(QWidget *parent ):
    baseWidget(parent),
    m_labduration("00:00/00:00",this)
{
    setStyleSheet("baseWidget{background-color:rgb(180,180,180,125);}"
                  "QLabel{color:rgb(78,78,78);font-size:12px;font-family:黑体;}");


    init();
}
void playingWidget::init()
{
     m_item=NULL;
     setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    QHBoxLayout *hlyout=new QHBoxLayout;
    setFixedHeight(52);
///////////////////////////////////////////////first
    QHBoxLayout *hlyout2=new QHBoxLayout;

    m_btnCross.setFixedSize(16,16);
    m_btnCross.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_cross (1).png);}"
                         "QPushButton:hover{border-image:url(:/image/middlewidget/btn_cross (2).png);}"
                         "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_cross (3).png);}");
/////////////////////////////////////////////second
    m_btnLab.setFixedSize(52,52);
    m_btnLab.setIcon(QIcon(":/image/middlewidget/lab_Author.png"));
    m_btnLab.setIconSize(QSize(52,52));

    hlyout2->addSpacing(2);
    hlyout2->addWidget(&m_btnCross);
    hlyout2->addSpacing(2);
    hlyout2->addWidget(&m_btnLab);
    hlyout2->setSpacing(0);
    hlyout2->setContentsMargins(0,0,0,0);
/////////////////////////////////////////////third
    QVBoxLayout *vlyout=new QVBoxLayout;
    QHBoxLayout *hlyout1=new QHBoxLayout;

    m_btnSongName.setNormalColor(QColor(78,78,78));
    m_btnSongName.setHoverColor(QColor(48,48,48));

    m_btnAuthor.setNormalColor(QColor(78,78,78));
    m_btnAuthor.setHoverColor(QColor(48,48,48));


    m_labduration.setFixedSize(80,12);
    m_labduration.setAlignment(Qt::AlignTop|Qt::AlignLeft);

    m_btndownload.setFixedSize(16,16);
    m_btnMovie.setFixedSize(16,16);
    m_btnLove.setFixedSize(16,16);
    m_btnDel.setFixedSize(16,16);
    m_btnMore.setFixedSize(16,16);

    m_btnMovie.setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/middlewidget/btn_mv (2).png);}"
                              "QPushButton:hover{border:NULL;background-image:url(:/image/middlewidget/btn_mv (1).png);}"
                              "QPushButton:pressed{border:NULL;background-image:url(:/image/middlewidget/btn_mv (3).png);}");

    m_btnSongName.setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:14px;font-family:黑体;}"
                             "QPushButton:hover{text-decoration: underline; color: rgb(100,176,250);}");

    m_btnAuthor.setStyleSheet("QPushButton{color:rgb(68,68,68);font-size:14px;font-family:黑体;}"
                               "QPushButton:hover{text-decoration: underline; color: rgb(100,176,250);}");

    m_labduration.setStyleSheet("color:rgb(78,78,78);font-size:12px;font-family:黑体;");

    m_btndownload.setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/middlewidget/btn_downloadmini (1).png);}"
                                 "QPushButton:hover{border:NULL;background-image:url(:/image/middlewidget/btn_downloadmini (2).png);}"
                                 "QPushButton:pressed{border:NULL;background-image:url(:/image/middlewidget/btn_downloadmini (3).png);}");

    m_btnLove.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_love (1).png);}"
                             "QPushButton:hover{border-image:url(:/image/middlewidget/btn_love (2).png);}"
                             "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_love (3).png);}");

    m_btnDel.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_del (1).png);}"
                            "QPushButton:hover{border-image:url(:/image/middlewidget/btn_del (2).png);}"
                            "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_del (3).png);}");

    m_btnMore.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_more (1).png);}"
                             "QPushButton:hover{border-image:url(:/image/middlewidget/btn_more (2).png);}"
                             "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_more (3).png);}");

    hlyout1->addWidget(&m_labduration);
    hlyout1->addStretch(10);
    hlyout1->addWidget(&m_btndownload);
    hlyout1->addWidget(&m_btnMovie);
    hlyout1->addWidget(&m_btnLove);
    hlyout1->addWidget(&m_btnDel);
    hlyout1->addWidget(&m_btnMore);
    hlyout1->setSpacing(11);
    hlyout1->setContentsMargins(0,0,0,0);

    QHBoxLayout *hlyout3=new QHBoxLayout;
    hlyout3->addWidget(&m_btnAuthor);
    hlyout3->addWidget(new QLabel("-"));
    hlyout3->addWidget(&m_btnSongName);

    hlyout3->addStretch(10);
    hlyout3->setSpacing(2);
    hlyout3->setContentsMargins(0,0,0,0);

    vlyout->addLayout(hlyout3);
    vlyout->addLayout(hlyout1);

    vlyout->setSpacing(0);
    vlyout->setContentsMargins(0,5,18,8);
/////////////////////////////////////////////////////
    hlyout->addLayout(hlyout2);
    hlyout->addSpacing(6);
    hlyout->addLayout(vlyout);
    hlyout->setContentsMargins(0,0,0,0);
    setLayout(hlyout);

    connect(&m_btnAuthor,SIGNAL(clicked(bool)),this,SLOT(slot_readySearch()));
    connect(&m_btnSongName,SIGNAL(clicked(bool)),this,SLOT(slot_readySearch()));

}

void playingWidget::setLoveState(bool bLoved)
{
    if(bLoved)
    {
        m_btnLove.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_islove (1).png);}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_islove (2).png);}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_islove (3).png);}");
        m_islove=true;
    }
    else
    {
        m_btnLove.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_love (1).png);}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_love (2).png);}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_love (3).png);}");
        m_islove=false;
    }
}


void playingWidget::setOriginalState()
{
    setSongName("");
    setCurrentSongItem(NULL);
}

void playingWidget::setSongName(const QString &str)
{
   QString author= str.split("-").value(0);
   QString songname= str.split("-").value(1);
    m_btnAuthor.setText(author);
    m_btnSongName.setText(songname);
    m_btnAuthor.setMaximumWidth(100);
    m_btnSongName.setMaximumWidth(150);
    m_btnAuthor.adjustSize();
    m_btnSongName.adjustSize();
    m_songName=str;
}

void playingWidget::slot_readySearch()
{
    playingWidgetBtn*btn= (playingWidgetBtn*)sender();
    emit sig_clicked(btn->text());
}

