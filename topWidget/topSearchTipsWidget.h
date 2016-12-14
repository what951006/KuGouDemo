#ifndef TOPSEARCHTIPSWIDGET_H
#define TOPSEARCHTIPSWIDGET_H

#include <QObject>
#include <QWidget>
#include "QListWidget.h"
#include<QTableWidgetItem>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
class topSearchTipsWidget:public QListWidget
{
    Q_OBJECT
public:
    explicit topSearchTipsWidget(QWidget*p=0);

public slots:
    void slot_textchanged(QString);
protected:
    void mouseMoveEvent(QMouseEvent*);
    void leaveEvent(QEvent *);
    void focusInEvent(QFocusEvent *);

private:
    int prerow;
    QNetworkAccessManager *m_manger;
};

#endif // TOPSEARCHTIPSWIDGET_H
