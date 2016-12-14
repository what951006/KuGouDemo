#ifndef MIDDLECONVIENTTWOBUTTON_H
#define MIDDLECONVIENTTWOBUTTON_H

#include <QObject>
#include <QWidget>
#include<QlineEdit>
#include<myPushButton.h>

#include"baseWidget.h"

class myPushButton;

class middleConvientTwoButton : public baseWidget
{
    Q_OBJECT
public:
  explicit   middleConvientTwoButton(QWidget*p=0);

   myPushButton *m_btnsearch;
   myPushButton *m_btnlocate;
protected:
   void showEvent(QShowEvent *);
   void enterEvent(QEvent *);
   void leaveEvent(QEvent *);
public slots:
   void slot_timerEvent();
private:
   QTimer *m_timer;
};

class middleListSearch:public baseWidget
{
    Q_OBJECT
public:
    explicit middleListSearch(QWidget*p=0);
    QLineEdit *m_lineEdit;
    myPushButton *m_btnclose;
private:
public slots:
    void setClear();

};



#endif // MIDDLECONVIENTTWOBUTTON_H
