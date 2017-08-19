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

    void setCurBGPic(const QString&);
    void setShowSingerBG(bool is=true);
    bool isShowSingerBG(){return m_isShowSingerBG;}
    void clearBg();
    const QString currentSkinPath(){return m_curPixPath;}
    inline QPixmap getRectPix(const QRect&rect){update();return  m_curPix.copy(rect);}
protected:
    virtual void paintEvent(QPaintEvent *);
 public slots:
    void setSkin(const QString &str);
private:
    QString m_curPixPath;
    QPixmap m_netPic;
    QPixmap m_skinPic;
    QPixmap m_curPix; //not the normal size

    bool m_bShowSinger;
    bool m_isShowSingerBG;
};

class baseWindow : public AbsFrameLessAutoSize
{
    Q_OBJECT
public:
    explicit baseWindow(QWidget *parent = 0);
    Widget m_mainwid;
protected:
    virtual void paintEvent(QPaintEvent *);
 private:

};

#endif // BASEWINDOW_H
