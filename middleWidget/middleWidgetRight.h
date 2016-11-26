#ifndef MIDDLEWIDGETRIGHT_H
#define MIDDLEWIDGETRIGHT_H

#include <QObject>
#include <QWidget>
#include<QStackedWidget>
#include<QLineEdit>

//#include"WebWidget.h"
#include"baseWidget.h"
class mainWindow;
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
   QVector<myPushButton*>  &listButtons(){return m_listbtn;}
   void setInitMiddleWidget(middleWidgets*p){m_middlewidget=p;}
   void setInitMainWindow(mainWindow*p);
   void setDrawLine(bool draw=true){m_isdrawline=draw;}

   QStackedWidget *m_stackWid;
   LyricLabel * m_lrcwid;
   middleSearchWidget*m_searchwid;
   static QColor bgcolor;
public slots:
   void slot_setSearchStack();
   void slot_search(const QString&);
   void slot_setLrcShowStack();
   void slot_btnClick();
   void slot_curStackChange(int);
protected:
   void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *);
signals:
    void sig_requestSong(const QString&);

private:
   bool m_isdrawline;

   QVector<myPushButton*> m_listbtn;
   middleWidgets *m_middlewidget;
   mainWindow *m_mainWindow;
   baseWidget*m_wid;


/*   WebWidget *m_web0;
    WebWidget *m_web1;
    WebWidget *m_web2;
    WebWidget *m_web3;
    WebWidget *m_web4;*/
};

#endif // MIDDLEWIDGETRIGHT_H
