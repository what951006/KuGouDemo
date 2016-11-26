#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QObject>
#include <QWidget>
#include<QMouseEvent>
#include<QSlider>
class mySlider : public QSlider
{
    Q_OBJECT
public:
    mySlider(Qt::Orientation orientation,QWidget*parent=0);
protected:
  //  bool eventFilter(QObject *, QEvent *);
    void mousePressEvent(QMouseEvent*);
};

#endif // MYSLIDER_H
