#ifndef MIDDLEWIDGETRIGHT_H
#define MIDDLEWIDGETRIGHT_H

#include <QObject>
#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>

#include "MvWidget.h"
#include "baseWidget.h"
#include "LyricWidget.h"
#include "myPushButton.h"
#include "middleSearchWidget.h"

class middleSearchWidget;
class LyricLabel;
class myPushButton;
class middleWidgets;


class middleWidgetRight:public baseWidget
{
    Q_OBJECT

public:
   explicit middleWidgetRight(QWidget*parent=0);
   void init();
   void setInitMiddleWidget(middleWidgets*p){m_middlewidget=p;}
   void setDrawLine(bool draw=true){m_isdrawline=draw;}

   LyricWidget* getLrcWidget(){return &m_lrcwid;}
   middleSearchWidget * getSearWidget(){return &m_searchwid;}
   static QColor bgcolor;
public slots:
   void slot_setSearchStack();
   void slot_search(const QString&);
   void slot_setLrcShowStack();
   void slot_btnClick();
   void slot_curStackChange(int);
   void slot_imageMV(QImage);
protected:
   void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *);
signals:
    void sig_requestSong(const QString&);

private:
   bool m_isdrawline;

   QStackedWidget m_stackWid;
   LyricWidget m_lrcwid;
   middleSearchWidget m_searchwid;
   myPushButton m_btnArray[6];
   MvWidget m_MvWid;
   baseWidget m_wid;

   middleWidgets *m_middlewidget;
/*   WebWidget *m_web0;
    WebWidget *m_web1;
    WebWidget *m_web2;
    WebWidget *m_web3;
    WebWidget *m_web4;*/
};

#endif // MIDDLEWIDGETRIGHT_H
