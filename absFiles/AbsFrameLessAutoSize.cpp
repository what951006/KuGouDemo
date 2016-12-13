#include "AbsFrameLessAutoSize.h"
#include<qdebug.h>
AbsFrameLessAutoSize::AbsFrameLessAutoSize(QWidget *parent)
    : QWidget(parent)
{

    m_state.MousePressed=false;
    m_state.IsPressBorder=false;
    setMinimumSize(400,550);
    m_border=4;

    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint); //setting windows tool bar icon invisiable
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void AbsFrameLessAutoSize::setBorder(int border)
{
    m_border=border;
}

void AbsFrameLessAutoSize::mouseMoveRect(const QPoint& p)
{
    if(!m_state.IsPressBorder)
    {
        if( p.x()>width()-m_border&&p.y()<height()-m_border&&p.y()>m_border)//right side
        {
            setCursor(Qt::SizeHorCursor);
            m_curPos= CursorPos::Right;
        }
        else if(p.x()<m_border&&p.y()<height()-m_border&&p.y()>m_border)//left side;
        {
            setCursor(Qt::SizeHorCursor);
            m_curPos= CursorPos::Left;
        }
        else if(p.y()>height()-m_border&&p.x()>m_border&&p.x()<width()-m_border)//bottom side;
        {
            setCursor(Qt::SizeVerCursor);
            m_curPos= CursorPos::Bottom;
        }
        else if(p.y()<m_border&&p.x()>m_border&&p.x()<width()-m_border)
        {
            setCursor(Qt::SizeVerCursor);
            m_curPos=CursorPos::Top;
        }
//corner
        else if(p.y()<m_border&&p.x()>width()-m_border)
        {
            setCursor(Qt::SizeBDiagCursor);
            m_curPos=CursorPos::TopRight;
        }

        else if(p.y()<m_border&&p.x()<m_border)
        {
            setCursor(Qt::SizeFDiagCursor);
            m_curPos=CursorPos::TopLeft;
        }

        else if(p.x()>m_border&&p.y()>height()-m_border)
        {
            setCursor(Qt::SizeFDiagCursor);
            m_curPos=CursorPos::BottomRight;
        }

        else if(p.x()<m_border&&p.y()>height()-m_border)
        {
            setCursor(Qt::SizeBDiagCursor);
            m_curPos=CursorPos::BottomLeft;
        }
////
        else
        {
            m_curPos=CursorPos::Default;
            setCursor(Qt::ArrowCursor);
        }
    }
    else
    {
        switch (m_curPos) {
        case CursorPos::Right:
        {
           int setW=QCursor::pos().x()-x();
           if(minimumWidth()<=setW&&setW<=maximumWidth())
             setGeometry(x(),y(),setW,height());
            break;
        }
        case CursorPos::Left:
        {
            int setW=x()+width()-QCursor::pos().x();
            int setX=QCursor::pos().x();
            if(minimumWidth()<=setW&&setW<=maximumWidth())
              setGeometry(setX,y(),setW,height());
             break;
        }
        case CursorPos::Bottom:
        {
            int setH=QCursor::pos().y()-y();
            if(minimumHeight()<=setH&&setH<=maximumHeight())
            setGeometry(x(),y(),width(),setH);
            break;
        }
        case CursorPos::Top:
        {
           int setH=y()-QCursor::pos().y()+height();
           if(minimumHeight()<=setH&&setH<=maximumHeight())
             setGeometry(x(),QCursor::pos().y(),width(),setH);
             break;
        }
        case CursorPos::TopRight:
        {
            int setH=y()+height()-QCursor::pos().y();
            int setW=QCursor::pos().x()-x();
            int setY=QCursor::pos().y();
            if(setH>=maximumHeight())
            {
                setY=m_state.WindowPos.y()+m_state.PressedSize.height()-height();
                setH=maximumHeight();
            }
            if(setH<=minimumHeight())
            {
                setY=m_state.WindowPos.y()+m_state.PressedSize.height()-height();
                setH=minimumHeight();
            }
            setGeometry(m_state.WindowPos.x(),setY,setW,setH);
            break;
        }
        case CursorPos::TopLeft:
        {
            int setY=QCursor::pos().y();
            int setX=QCursor::pos().x();

            int setW=pos().x()+width()-setX;
            int setH=pos().y()+height()-setY;
            int totalW= m_state.WindowPos.x()+m_state.PressedSize.width();
            int totalH=m_state.WindowPos.y()+m_state.PressedSize.height();

            if(totalW-setX>=maximumWidth())
            {
                setX=totalW-maximumWidth();
                setW=maximumWidth();
            }
            if(totalW-setX<=minimumWidth())
            {
                setX=totalW-minimumWidth();
                setW=minimumWidth();
            }
            if(totalH-setY>=maximumHeight())
            {
                setY=totalH-maximumHeight();
                setH=maximumHeight();
            }
            if(totalH-setY<=minimumHeight())
            {
                setY=totalH-minimumHeight();
                setH=minimumHeight();
            }
            setGeometry(setX,setY,setW,setH);
            break;
        }
        case CursorPos::BottomRight:
        {
           int setW=QCursor::pos().x()-x();
           int setH=QCursor::pos().y()-y();
           setGeometry(m_state.WindowPos.x(),m_state.WindowPos.y(),setW,setH);
             break;
        }
        case CursorPos::BottomLeft:
        {
            int setW=x()+width()-QCursor::pos().x();
            int setH=QCursor::pos().y()-m_state.WindowPos.y();
            int setX=QCursor::pos().x();
            int totalW= m_state.WindowPos.x()+m_state.PressedSize.width();
            if(totalW-setX>=maximumWidth())
            {
                setX=totalW-maximumWidth();
                setW=maximumWidth();
            }
            if(totalW-setX<=minimumWidth())
            {
                setX=totalW-minimumWidth();
                setW=minimumWidth();
            }
            setGeometry(setX,m_state.WindowPos.y(),setW,setH);
            break;
        }
        case CursorPos::Default:
        default:
            break;
        }
    }
}
void AbsFrameLessAutoSize::mousePressEvent(QMouseEvent *event)
{
    m_state.PressedSize=this->size();
    m_state.IsPressBorder=false;
      setFocus();
      if (event->button() == Qt::LeftButton)
     {
          m_state.WindowPos = this->pos(); //save the prssed position
          if(QRect(m_border+1,m_border+1,width()-(m_border+1)*2,height()-(m_border+1)*2).contains(QPoint(event->globalX()-x(),event->globalY()-y())))
          {
              m_state.MousePos = event->globalPos();
              m_state.MousePressed = true;
          }
          else
              m_state.IsPressBorder=true;
      }
}
void AbsFrameLessAutoSize::mouseMoveEvent(QMouseEvent *event)
{
    mouseMoveRect(mapFromGlobal(event->globalPos()));
    if (m_state.MousePressed)
        {
            this->move(m_state.WindowPos + (event->globalPos() - m_state.MousePos));
        }
}
void AbsFrameLessAutoSize::mouseReleaseEvent(QMouseEvent *event)
{
      m_state.IsPressBorder=false;
    if (event->button() == Qt::LeftButton)
    {
        this->m_state.MousePressed = false;
    }
    setCursor(Qt::PointingHandCursor);
}
