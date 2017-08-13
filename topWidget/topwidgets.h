#ifndef TOPWIDGETS_H
#define TOPWIDGETS_H

#include <QObject>
#include <QWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QSpacerItem>

#include"baseWidget.h"
#include"myPushButton.h"
#include"topSearchTipsWidget.h"

class TopWidgets : public baseWidget
{

    Q_OBJECT
public:
    explicit TopWidgets(QWidget *parent = 0);
    void initWidget();
    void searchFromLineEdit(const QString);

    myPushButton m_btnexit;
    myPushButton m_btnmini;
    myPushButton m_btnminiframe;
    myPushButton m_btnmobile;
    myPushButton m_btnsetting;
    myPushButton m_btnskin;
    myPushButton m_btnmessage;
    myPushButton m_btnplaycard;
    myPushButton m_btntool;
    myPushButton m_btngame;

    QLineEdit m_lineEdit;
    myPushButton m_btnlog;
    myPushButton m_btnregister;
public slots:
    void slot_serTipsClick(QListWidgetItem*);

protected:
    bool eventFilter(QObject *, QEvent *);
signals:
};

#endif // TOPWIDGETS_H
