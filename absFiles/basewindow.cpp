#include "basewindow.h"
#include<QGridLayout>
#include<QPushButton>
#include<QDebug>
#include<QLineEdit>

#include"Global_ValueGather.h"
baseWindow::baseWindow(QWidget *parent) : AbsFrameLessAutoSize(parent)
{
    setMinimumSize(550,440);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_mainwid=new Widget(this);
    backgroundPointer=m_mainwid; //globalvalue
    m_mainwid->setAutoFillBackground(true);
    QGridLayout *lyout=new QGridLayout;
    lyout->addWidget(m_mainwid);
    lyout->setContentsMargins(4,4,4,4);
    setLayout(lyout);


}
void baseWindow::paintEvent(QPaintEvent *e)
{
    AbsFrameLessAutoSize::paintEvent(e);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(9, 9, this->width()-18, this->height()-18);


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));


    QColor color(0, 0, 0, 50);
    for(int i=0; i<9; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(9-i, 9-i, this->width()-(9-i)*2, this->height()-(9-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

////////////////////////////////////////////////////////////////////

Widget::Widget(QWidget *parent):QWidget(parent)
{
    m_curindex=0;
    m_issetpix=false;
    m_isShowSingerBG=true;

    m_netPix=QPixmap("");
    m_localPix=QPixmap("");
    m_curPix=QPixmap("");
}
void Widget::setPixmap(const QPixmap &pix)
{
    m_localPix=pix;
    update();
}

void Widget::setShowSingerBG(bool is)
{
    m_isShowSingerBG=is;
}

void Widget::setCurrentIndex(int i)
{
    m_curindex=i;
    update();
}

void Widget::clearBg()
{
    m_issetpix=false;
    m_localPix=QPixmap("");
    update();
}


void Widget::setSkin(const QString &skin)
{
    m_netPix=QPixmap(skin);
    m_curPixPath=skin;
    update();
}

void Widget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setRenderHints(QPainter::SmoothPixmapTransform,true);//消锯齿

    if(m_curindex==5&&m_localPix.width()!=0)
        m_issetpix=true;
    else
        m_issetpix=false;

    double d =(double)m_netPix.height()/m_netPix.width();
    int h=d*width();
    int w=height()/d;
    p.drawPixmap(0,0,width(),h,m_netPix);
    m_curPix=m_netPix.scaled(width(),h);
    if(h<height())//如果图片高度小于窗口高度
    {
        p.drawPixmap(0,0,w,height(),m_netPix);
        m_curPix=m_netPix.scaled(w,height());
    }

    if(m_issetpix&&m_isShowSingerBG)
    {
        double d =(double)m_localPix.height()/m_localPix.width();
        int h=d*width();
        int w=height()/d;
        p.drawPixmap(0,0,width(),h,m_localPix);
        m_curPix=m_localPix.scaled(width(),h);
        if(h<height())//如果图片高度小于窗口高度
        {
             p.drawPixmap(0,0,w,height(),m_localPix);
             m_curPix=m_localPix.scaled(w,height());
        }
    }
}


