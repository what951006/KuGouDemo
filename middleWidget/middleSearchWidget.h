#ifndef MIDDLESEARCHWIDGET_H
#define MIDDLESEARCHWIDGET_H

#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include <QTimeLine>
#include <QLineEdit>

#include"myPushButton.h"
#include"baseWidget.h"
#include"middleSearchTableWidget.h"
#include"mynetwork.h"


class loadingWidget:public baseWidget
{
  Q_OBJECT
public:
   explicit loadingWidget(QString pixurl,int tinypixcount,QString text,QWidget*p);
protected:
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *e);

public slots:
    void slot_valuechange(int i) {_index=i; update(); }
private:
    QVector<QPixmap> m_listpix;
    QString m_text;
    int _index;
    QTimeLine *animationtimeline;
};

class middleSearchWidget:public baseWidget
{
    Q_OBJECT
public:
   explicit middleSearchWidget(QWidget*p=0);
    ~middleSearchWidget(){}
    void init();
    void initMaskWidget();

    void setSearchName(const QString&name){m_labtext->setText(name);}
    void showLoadingWidget(){m_maskwid->show();}
    void setRequestisFinished(bool finished){m_isRequestFinished=finished;}

    middleSearchTableWidget* serchTable(){return m_table;}
    QCheckBox *m_checkbox;
    QList<QString> m_songlist;

protected:
    virtual void resizeEvent(QResizeEvent *);

public slots:
    void slot_addRequestSong(const QByteArray&);

    void slot_autoRequestNextPage(int);

    void slot_requestSong(const QByteArray&);

    void slot_btnplayclicked();
    void slot_btnaddclicked();
    void slot_btndownloadclicked();

    void slot_checkBoxClicked();
private slots:
    void slot_menuWork();
signals:
    void sig_requestSongNextPage();

    void sig_btnPlayClickInfo(const QStringList &songname,const QStringList &songurl,const QStringList &dur);

private:
     middleSearchTableWidget*m_table;
     myPushButton *m_btnplay;
     myPushButton *m_btnadd;
     myPushButton *m_btndownload;

     loadingWidget *m_maskwid;
     QLabel *m_labtext;
     QLabel *m_labelSinger;
     QLabel *m_labelAlbum;
     bool m_isRequestFinished;

};

#endif // MIDDLESEARCHWIDGET_H
