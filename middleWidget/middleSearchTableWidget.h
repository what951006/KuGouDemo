#ifndef MIDDLESEARCHTABLEWIDGET_H
#define MIDDLESEARCHTABLEWIDGET_H

#include <QObject>
#include <QWidget>
#include<QCheckBox>
#include<QTableWidget>
#include"baseWidget.h"
#include"myPushButton.h"

class middleSearchWidget;

class tableCellWidget:public baseWidget
{
    Q_OBJECT
public:
   explicit tableCellWidget(QWidget*p=0);
    ~tableCellWidget()
    {
        m_btntableplay->deleteLater();
        m_btntableadd->deleteLater();
        m_btntabledownload->deleteLater();
    }
   void init();
   myPushButton *m_btntableplay;
   myPushButton *m_btntableadd;
   myPushButton *m_btntabledownload;
};

class middleSearchTableWidget:public QTableWidget
{
    Q_OBJECT
public:
   explicit middleSearchTableWidget(QWidget*p=0);
   void init();
   void setInitSearchTableWidget(middleSearchWidget *p){m_searchwidget=p;}
   void setItem(int row, int column, QTableWidgetItem *item);//add checkbox for column 0
   void removeRow(int row);
signals:
   void sig_doubleclicked(const QStringList&,const QStringList&,const QStringList&);
protected:
   virtual void leaveEvent(QEvent *);

public slots:
   void slot_tableContainerBtnClicked();
   void slot_playbtnclicked();
   void slot_addbtnclicked();
   void slot_downloadbtnclicked();

   void slot_cellEnter(int row, int c);

   void slot_itemclick(int,int);

   void slot_doubleClicked(QTableWidgetItem*);
private:
    int m_prebgItem;
    middleSearchWidget *m_searchwidget;

};

#endif // MIDDLESEARCHTABLEWIDGET_H
