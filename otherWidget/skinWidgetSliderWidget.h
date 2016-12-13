#ifndef SKINWIDGETSLIDERWIDGET_H
#define SKINWIDGETSLIDERWIDGET_H

#include <QObject>
#include <QWidget>
#include"baseWidget.h"
#include"mySlider.h"

class skinWidgetSliderWidget : public baseWidget
{
public:
   explicit skinWidgetSliderWidget(QWidget*p=0);
    mySlider *m_slider;
protected:
    void focusOutEvent(QFocusEvent *){if(!hasFocus()&&!m_slider->hasFocus())hide();}
    void paintEvent(QPaintEvent *);
    void leaveEvent(QEvent *);
private:


};

#endif // SKINWIDGETSLIDERWIDGET_H
