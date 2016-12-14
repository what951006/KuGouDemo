#ifndef MIDDLELEFTSTACKWIDGET4_H
#define MIDDLELEFTSTACKWIDGET4_H

#include <QObject>
#include <QWidget>
#include"baseWidget.h"
#include"buttonstackedwidget.h"
class middleLeftStackWidget4:public baseWidget
{
    Q_OBJECT
public:
   explicit middleLeftStackWidget4(QWidget *parent);
    buttonStackedWidget *m_wid;
    void init();
};

#endif // MIDDLELEFTSTACKWIDGET4_H
