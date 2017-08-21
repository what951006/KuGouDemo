#ifndef MIANWINDOWCONTENTWIDGET_H
#define MIANWINDOWCONTENTWIDGET_H

#include <QObject>
#include <QWidget>
#include<QLabel>
#include<QToolTip>

#include"baseWidget.h"
#include"myPushButton.h"
#include"mySlider.h"
#include"myMediaList.h"


class playModeWidget:public baseWidget
{
    Q_OBJECT
public:
   explicit playModeWidget(QWidget*parent=0);
   void init();
   void setCurrentMode(PlayMode);

   QVector<playModeButton*> m_vector;
   playModeButton m_btnOrder;
   playModeButton m_btnRandom;
   playModeButton m_btnOneCircle;
private slots:
   void slot_setCurrentMode();
signals:
   void sig_CurrentModeChange(PlayMode);
protected:
    void mousePressEvent(QMouseEvent *){}
    void mouseMoveEvent(QMouseEvent *){}
    void focusOutEvent(QFocusEvent*){this->hide();}
private:

};

class volSliderWidget:public baseWidget
{
    Q_OBJECT
public:
    volSliderWidget(QWidget*parent=0);
    mySlider *m_slider;
public slots:
    void slot_sliderMove(int va){QToolTip::showText(QCursor::pos(),QString::number(va));}
protected:
    void mousePressEvent(QMouseEvent*){}
    void mouseMoveEvent(QMouseEvent*){}
    void mouseReleaseEvent(QMouseEvent*){}

    bool eventFilter(QObject *, QEvent *);
private:

};

class AddLoveListTips:public baseWidget
{
    Q_OBJECT
public:
    explicit AddLoveListTips(QWidget*p=0);
    void showAddTips();
    void showRemoveTips();
public slots:
    void slot_timerWork();
private:
    myPushButton m_closebtn;
    QLabel m_tipslab;
    QTimer m_timer;
protected:
    void showEvent(QShowEvent*);

};
#endif // MIANWINDOWCONTENTWIDGET_H
