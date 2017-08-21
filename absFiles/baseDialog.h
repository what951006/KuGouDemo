#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QObject>
#include<QDialog>
#include"basewindow.h"

class baseDialog : public QDialog
{
    Q_OBJECT
public:
   explicit baseDialog(QWidget*p=0);

   explicit baseDialog(QWidget *p,bool isDrawCentralWidget);

    Widget m_mainwid;
protected:
    void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    bool    m_MousePressed;
    bool   m_isPressBorder;
    QPoint  m_MousePos;
    QPoint  m_WindowPos;



};

#endif // BASEDIALOG_H
