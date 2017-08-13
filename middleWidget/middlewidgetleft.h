#ifndef MIDDLEWIDGETLEFT_H
#define MIDDLEWIDGETLEFT_H

#include <QObject>
#include <QWidget>
#include<QStackedWidget>
#include<QPropertyAnimation>

#include "baseWidget.h"
#include "myPushButton.h"
#include "middleLeftStackWidget0.h"
#include "middleLeftStackWidget1.h"
#include "middleLeftStackWidget2.h"
#include "middleLeftStackWidget3.h"
#include "middleLeftStackWidget4.h"



class middleWidgetLeft : public baseWidget
{
    Q_OBJECT
    Q_PROPERTY(int m_x READ getValue WRITE animation)
public:
    explicit middleWidgetLeft(QWidget *parent = 0);

    middleLeftStackWidget0* GetStackWid0(){return &m_Swidget0;}


    static QColor color;//92 174 219
    static QColor bgcolor;

    void initLayout();
    void initAnimation();

    int getValue(){return m_x;}
    void animation(int i){m_x=i;update();}
    void setBackgroundtransparent();
    void setBackgroundnormal();
    void setDrawVerticalLine(bool is=true){m_isDrawVerticalLine=is;}

protected:
    void paintEvent(QPaintEvent *);

    void resizeEvent(QResizeEvent*);

private slots:
    void slot_btn();
    void setWidgetOpacity(int);
    void slot_changeButtonSelected(int i);//改变选中图片
    void slot_finished(){m_isAnima=false;}
private:

    stackButton *m_btn[5];

    QStackedWidget  m_stackwid;
    middleLeftStackWidget0 m_Swidget0;
    middleLeftStackWidget1 m_Swidget1;
    middleLeftStackWidget2 m_Swidget2;
    middleLeftStackWidget3 m_Swidget3;
    middleLeftStackWidget4 m_Swidget4;

    QPixmap m_pix;
    int m_index;
    int m_preindex;
    bool m_isDrawVerticalLine;
    bool m_isAnima;
    int m_x;
    int m_preItem;
    QPropertyAnimation *m_animation;
    QPropertyAnimation *m_Widanimation;

};


#endif // MIDDLEWIDGETLEFT_H
