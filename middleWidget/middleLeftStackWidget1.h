#ifndef MIDDLELEFTSTACKWIDGET1_H
#define MIDDLELEFTSTACKWIDGET1_H

#include <QObject>
#include <QWidget>
#include"baseWidget.h"

class middleLeftStackWidget1:public baseWidget
{

    Q_OBJECT
public:
   explicit middleLeftStackWidget1(QWidget *parent);
    void initLayout();
};

#endif // MIDDLELEFTSTACKWIDGET1_H
