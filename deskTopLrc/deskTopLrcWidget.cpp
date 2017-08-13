#include "deskTopLrcWidget.h"
#include<QLinearGradient>
#include<QPainter>
#include<QMessageBox>
#include<QDebug>
#include<QHBoxLayout>
#include<QVBoxLayout>

#include"Global_ValueGather.h"
#include"FFmpegPlayer.h"

deskTopLrcWidget::deskTopLrcWidget(QWidget *parent)
    : AbsFrameLessAutoSize(parent)
{
    desktoplrcPointer=this;
    m_precent=0;
    m_border=8;
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::X11BypassWindowManagerHint|Qt::WindowStaysOnTopHint); //setting windows tool bar icon invisiable
    setAttribute(Qt::WA_TranslucentBackground,true);

    initWidgetMISC();
}

deskTopLrcWidget::~deskTopLrcWidget(){}

void deskTopLrcWidget::initWidgetMISC()
{
    m_maskGradient.setColorAt(0.1,QColor(150,250,250));
    m_maskGradient.setColorAt(0.5, Qt::white);
    m_maskGradient.setColorAt(0.9, QColor(150,250,250));

    m_normalGradient.setColorAt(0.1, QColor(0, 75, 155));
    m_normalGradient.setColorAt(0.5, QColor(1, 110, 186));
    m_normalGradient.setColorAt(0.9, QColor(2, 168, 224));


    m_isOriginalState=false;
    m_isDrawBrush=false;

    setMinimumSize(1000,120);
    setMaximumHeight(250);
    setOriginalStatus();

    m_topWid=new QWidget(this);
    m_topWid->setFixedHeight(20);

    m_closeBtn=new myPushButton(this);
    m_closeBtn->setFixedSize(20,20);
    m_closeBtn->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_close (1).png);background:rgb(0,0,0,75);}"
                              "QPushButton::hover{border-image:url(:/image/topwidget/btn_close (2).png);}"
                              "QPushButton::pressed{border-image:url(:/image/topwidget/btn_close (3).png);}");

    QHBoxLayout *hl=new QHBoxLayout;
    hl->addWidget(m_closeBtn);
    hl->setContentsMargins(0,0,0,0);
    m_topWid->setLayout(hl);

    m_closeBtn->hide();
    m_closeBtn->setGeometry(width()-m_closeBtn->width()-2,0,20,20);
    connect(m_closeBtn,SIGNAL(clicked(bool)),this,SLOT(hide()));

}

void deskTopLrcWidget::setCurrentAndNextLrc(const QString &cur, const QString &next)
{
    if(cur==m_Text1)// to judge the current lrc is on the top
    {
        m_Text1=cur;
        m_Text2=next;
        m_isDrawTop=true;
    }
    else
    {
        m_Text1=next;
        m_Text2=cur;
        m_isDrawTop=false;
    }
    m_isOriginalState=false;
    update();

}



QString deskTopLrcWidget::getCurrentLrc() const
{
    if(m_isDrawTop)
     return m_Text1;
    return m_Text2;
}

 void deskTopLrcWidget::setOriginalStatus()
{
    m_Text1="";
    m_Text2="";
    m_isOriginalState=true;
    update();
 }


void deskTopLrcWidget::slot_currentLrcChange(const QString&cur, const QString &next)
{
    m_lrcMaskWidth=-1000;
    setCurrentAndNextLrc(cur,next);
}


void deskTopLrcWidget::slot_precentChange(const QString &word, float &precent, int &interval)
{
    QString text;
    if(m_isDrawTop)
        text=m_Text1;
    else
        text=m_Text2;

    QFontMetrics metrics(font());
    if(m_precent==precent)
    {
        qreal count = interval / 25;
        float lrcMaskMiniStep = metrics.width(word) / count;
        m_lrcMaskWidth +=lrcMaskMiniStep;
    }
    else
        m_lrcMaskWidth=metrics.width(text)*precent;
    m_isOriginalState=false;
    m_precent=precent;
    update();
}

void deskTopLrcWidget::paintEvent(QPaintEvent *)
{
    /*
     * we divide the height into two parts in average
    */

    QPainter painter(this);
    QFontMetrics metrics(font());

    if(!m_isOriginalState)
    {
            int y=((height()-20)/2-metrics.height())/2+20;
            int bottomwidth=width()-metrics.width(m_Text2);


        /*
         *frist part
         */
            m_normalGradient.setStart(0, y);
            m_normalGradient.setFinalStop(0, y+metrics.height());


            painter.setPen(QColor(0, 0, 0, 200));
            painter.drawText(11, y+1,metrics.width(m_Text1),metrics.height(),Qt::AlignLeft,m_Text1);//左对齐

            painter.setPen(QPen(m_normalGradient,0));
            painter.drawText(10, y,metrics.width(m_Text1),metrics.height(),Qt::AlignLeft, m_Text1);

        /*
         *second part
        */
            m_normalGradient.setStart(0, y+height()/2);
            m_normalGradient.setFinalStop(0, y+(height()-20)/2+metrics.height());


            painter.setPen(QColor(0, 0, 0, 200));
            painter.drawText(bottomwidth-9, y+(height()-20)/2+1,metrics.width(m_Text2),metrics.height(),Qt::AlignLeft,m_Text2);//左对齐

            painter.setPen(QPen(m_normalGradient,0));
            painter.drawText(bottomwidth-10, y+(height()-20)/2,metrics.width(m_Text2),metrics.height(),Qt::AlignLeft, m_Text2);

            if(m_isDrawTop)
            {
                    m_maskGradient.setStart(0, y);
                    m_maskGradient.setFinalStop(0, y+metrics.height());

                    painter.setPen(QPen(m_maskGradient, 0));
                    painter.drawText(10, y, m_lrcMaskWidth, metrics.height(),Qt::AlignLeft ,m_Text1);

            }
            else
            {
                    m_maskGradient.setStart(0, y+(height()-20)/2);
                    m_maskGradient.setFinalStop(0, y+(height()-20)/2+metrics.height());


                    painter.setPen(QPen(m_maskGradient, 0));
                    painter.drawText(bottomwidth-10, y+(height()-20)/2, m_lrcMaskWidth,  metrics.height(),Qt::AlignLeft, m_Text2);
            }
    }

    else/*draw the original text here*/
    {
        int y=(height()-20-metrics.height())/2+20;
        int x=(width()-metrics.width("欢迎使用极视听"))/2;
        m_normalGradient.setStart(0, y);
        m_normalGradient.setFinalStop(0, y+metrics.height());

        painter.setPen(QColor(0, 0, 0, 200));
        painter.drawText(x+1,y+1,metrics.width("欢迎使用极视听"),metrics.height(),Qt::AlignLeft,"欢迎使用极视听");//左对齐

        painter.setPen(QPen(m_normalGradient,0));
        painter.drawText(x,y,metrics.width("欢迎使用极视听"),metrics.height(),Qt::AlignLeft,"欢迎使用极视听");//左对齐
    }

    if(m_isDrawBrush)
    {
        painter.setPen(QPen(QColor(0,0,0,75),1,Qt::DotLine));
        painter.setBrush(QColor(0,0,0,75));
        painter.drawRect(1,20,width()-2,height()-21);
    }
}

void deskTopLrcWidget::resizeEvent(QResizeEvent *)
{
    m_topWid->setGeometry(0,0,width(),20);
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(height()/4);
    setFont(font);
}

void deskTopLrcWidget::enterEvent(QEvent *)
{
    setCursor(Qt::PointingHandCursor);
    m_closeBtn->show();
    m_isDrawBrush=true;
    update();
}

void deskTopLrcWidget::leaveEvent(QEvent *)
{
   m_isDrawBrush=false;
   setCursor(Qt::ArrowCursor);
    m_closeBtn->hide();
   update();
}

void deskTopLrcWidget::mouseMoveEvent(QMouseEvent *e)
{
     AbsFrameLessAutoSize::mouseMoveEvent(e);
     m_isDrawBrush=true;
     m_closeBtn->show();
     update();
}


