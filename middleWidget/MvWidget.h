#ifndef MVWIDGET_H
#define MVWIDGET_H

#include <QWidget>
#include"baseWidget.h"
class MvWidget : public baseWidget
{
public:
    MvWidget(QWidget*w=0);
    void setImage(QImage img){m_img=img;update();}
protected:
    virtual void paintEvent(QPaintEvent*);
private:
    QImage m_img;
};

#endif // MVWIDGET_H
