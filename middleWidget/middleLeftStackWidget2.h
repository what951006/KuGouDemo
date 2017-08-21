#ifndef MIDDLELEFTSTACKWIDGET2_H
#define MIDDLELEFTSTACKWIDGET2_H

#include <QObject>
#include <QWidget>
#include"baseWidget.h"
#include"buttonstackedwidget.h"
#include<QVBoxLayout>
class middleLeftStackWidget2:public baseWidget
{
    Q_OBJECT
public:
   explicit middleLeftStackWidget2(QWidget *parent);
    void initLayout();
    buttonStackedWidget m_Wid;
};

#endif // MIDDLELEFTSTACKWIDGET2_H
