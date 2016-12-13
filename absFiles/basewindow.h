#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include<QPaintEvent>
#include<QPainterPath>
#include<qmath.h>
#include<QPainter>
#include<QPainter>
#include<QStyleOption>
#include"AbsFrameLessAutoSize.h"

class Widget:public QWidget
{
    Q_OBJECT
public:
   explicit Widget(QWidget*parent=0);
   void setPixmap(const QPixmap&);
   void setShowSingerBG(bool is=true);
   bool isShowSingerBG(){return m_isShowSingerBG;}
   void setCurrentIndex(int);
   inline int CurrentIndex(){return m_curindex;}
   void clearBg();
   inline QPixmap currentSkin(){return m_curPix;}
   const QString currentSkinPath(){return m_curPixPath;}
   inline QPixmap getRectPix(const QRect&rect){update();return  m_curPix.copy(rect);}
public slots:
   void setSkin(const QString &str);
protected:
    void paintEvent(QPaintEvent *e);
private:
    QString m_curPixPath;
    QPixmap m_localPix;
    QPixmap m_netPix;
    QPixmap m_curPix; //not the normal size
    QRect m_rect;
    bool m_issetpix;
    bool m_isShowSingerBG;
    int m_curindex;
};

class baseWindow : public AbsFrameLessAutoSize
{
    Q_OBJECT
public:
    explicit baseWindow(QWidget *parent = 0);
    Widget *m_mainwid;
protected:
   virtual void paintEvent(QPaintEvent *);

private:

signals:

public slots:
};

#endif // BASEWINDOW_H
