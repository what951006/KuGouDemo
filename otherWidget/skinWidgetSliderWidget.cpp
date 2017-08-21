#include "skinWidgetSliderWidget.h"

#include<QPainter>
skinWidgetSliderWidget::skinWidgetSliderWidget(QWidget *p)
    :baseWidget(p)
    ,m_slider(Qt::Vertical,this)
{
    setFixedSize(72,150);
    setStyleSheet("QWidget{background:white;border:1px solid rgb(194,194,194);}");

    m_slider.setGeometry(50,5,10,140);
    m_slider.setRange(0,255);
    m_slider.setValue(100);
    m_slider.setStyleSheet("QSlider{border:NULL;background:transparent;}"
                            "QSlider::groove:vertical{background:transparent;border-radius:2px;width:3px;}"
                            "QSlider::sub-page:vertical{background:rgb(150, 150, 150);}"
                            "QSlider::add-page:vertical{background:rgb(122,122,122);}"
                            "QSlider::handle:vertical{background:rgb(122, 122, 122); height: 8px; border-radius: 4px; margin-left: -3px;  margin-right: -3px;  }");
}


void skinWidgetSliderWidget::paintEvent(QPaintEvent *e)
{
    baseWidget::paintEvent(e);
    QPainter p(this);
    p.setPen(QColor(88,88,88));
    p.drawText(5,15,"100%");
    p.drawText(5,80,"50%");
    p.drawText(5,145,"0%");
}

void skinWidgetSliderWidget::leaveEvent(QEvent *)
{
    hide();
}
