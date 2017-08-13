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
    QPainter painter(this);
 // draw shadow margin

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(9, 9, this->width()-18, this->height()-18);
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
    m_bShowSinger=false;
    m_isShowSingerBG=true;
}
void Widget::setCurBGPic(const QString &strPix)
{
    m_netPic.load(strPix);
    m_bShowSinger=true;
    update();
}

void Widget::setShowSingerBG(bool is)
{
    m_isShowSingerBG=is;
}

void Widget::clearBg()
{
    m_bShowSinger=false;
    m_netPic.load("");
    update();
}


void Widget::setSkin(const QString &skin)
{
    m_skinPic.load(skin);
    m_curPixPath=skin;
    update();
}
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

//[draw background]
    if(!m_bShowSinger)
    {
        float d =(float)m_skinPic.height()/m_skinPic.width();
        int h=d*width();
        int w=height()/d;
        if(h<height())//如果图片高度小于窗口高度
        {
            painter.drawPixmap(0,0,w,height(),m_skinPic);
            return;
        }
        painter.drawPixmap(0,0,width(),h,m_skinPic);
        return;
    }
    if(m_bShowSinger && m_isShowSingerBG)
    {
        float d =(float)m_netPic.height()/m_netPic.width();
        int h=d*width();
        int w=height()/d;
        if(h<height())//如果图片高度小于窗口高度
        {
             painter.drawPixmap(0,0,w,height(),m_netPic);
          //   m_curPix=m_netPic.scaled(w,height());
             return;
        }
        painter.drawPixmap(0,0,width(),h,m_netPic);
       // m_curPix=m_netPic.scaled(width(),h);
    }
}


