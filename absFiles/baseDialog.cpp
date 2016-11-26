#include "baseDialog.h"
#include<QGridLayout>

baseDialog::baseDialog(QWidget *parent) : QDialog(parent)
{
    m_MousePressed=false;
    setMinimumSize(550,440);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::X11BypassWindowManagerHint); //setting windows tool bar icon invisiable
    setAttribute(Qt::WA_TranslucentBackground,true);

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_mainwid=new Widget(this);
    m_mainwid->setAutoFillBackground(true);

    QGridLayout *lyout=new QGridLayout;
    lyout->addWidget(m_mainwid);
    lyout->setContentsMargins(4,4,4,4);
    setLayout(lyout);
}

baseDialog::baseDialog(QWidget *p, bool) : QDialog(p)
{
    m_MousePressed=false;
    setMinimumSize(550,440);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::X11BypassWindowManagerHint|Qt::WindowStaysOnTopHint); //setting windows tool bar icon invisiable
    setAttribute(Qt::WA_TranslucentBackground,true);

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_mainwid=NULL;
}

void baseDialog::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
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


void baseDialog::mousePressEvent(QMouseEvent *event)
{
    m_isPressBorder=false;
      setFocus();
      if (event->button() == Qt::LeftButton)
     {
          if(QRect(4,4,width()-8,height()-8).contains(event->pos()))
          {
               m_WindowPos = this->pos();
               m_MousePos = event->globalPos();
              this->m_MousePressed = true;
          }
          else
              m_isPressBorder=true;
      }
}
void baseDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_MousePressed)
        {
            this->move(m_WindowPos + (event->globalPos() - m_MousePos));
        }

}
void baseDialog::mouseReleaseEvent(QMouseEvent *event)
{
      m_isPressBorder=false;
    if (event->button() == Qt::LeftButton)
    {
        this->m_MousePressed = false;
    }
}
