#include "mySlider.h"
#include<QDebug>
mySlider::mySlider(Qt::Orientation orientation,QWidget*parent):QSlider(orientation,parent)
{
   /* setStyleSheet("QSlider::groove:horizontal{border-radius:2px;height:3px;}"
                  "QSlider::sub-page:horizontal{background:rgb(255, 255, 160);}"
                  "QSlider::add-page:horizontal{background:rgb(128,179,209);}"
                  "QSlider::handle:horizontal{background-color: rgb(255, 255, 160);width:10px;border-radius:5px;margin:-3px 0px -3px 0px;}");*/

    setMaximum(100);
    setMinimum(0);
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);
}
void mySlider::mousePressEvent(QMouseEvent *ev)
{
    if(orientation()==Qt::Vertical)//vertical
    {
        if(ev->button()==Qt::LeftButton)
        {
            int dur = maximum()- minimum();
            int pos = maximum() - dur * ((double)ev->y() / height());
            if(pos != sliderPosition())
             {
               setValue(pos);
             }
        }
    }
    else//horizontal
    {
        if(ev->button()==Qt::LeftButton)
        {
            int dur = maximum()- minimum();
            int pos = minimum() + dur * ((double)ev->x() / width());
            if(pos != sliderPosition())
             {
               setValue(pos);
             }
        }
    }
    QSlider::mousePressEvent(ev);
}
