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
    QTimeLine animationtimeline;
};

class middleSearchWidget:public baseWidget
{
    Q_OBJECT
public:
   explicit middleSearchWidget(QWidget*p=0);
    ~middleSearchWidget(){}
    void init();

    void initMaskWidget();

    void setChecked(bool bSet){m_checkbox.setChecked(bSet);}

    void setSearchName(const QString&name){m_labtext.setText(name);}

    void showLoadingWidget(bool bShow=true){bShow?m_maskwid.show():m_maskwid.hide();}

    void setRequestisFinished(bool finished){m_isRequestFinished=finished;}


    //perhaps a Bug happend here!!if index is greater than count
    const ItemResult& GetItemByIndex(int index){return m_songlist.at(index);}

    QString GetHashByIndex(int index){return m_hashmap.value(index);}
protected:
    virtual void resizeEvent(QResizeEvent *);

public slots:
    void slot_autoRequestNextPage(int);

    void slot_requestSong(const ItemResult&,SearchStatus);

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
     middleSearchTableWidget m_table;
     loadingWidget m_maskwid;
     myPushButton m_btnplay;
     myPushButton m_btnadd;
     myPushButton m_btndownload;
     QCheckBox m_checkbox;

     QLabel m_labtext;
     QLabel m_labelSinger;
     QLabel m_labelAlbum;
     bool m_isRequestFinished;


     QList<ItemResult> m_songlist;
     QMap<int ,QString> m_hashmap;
};

#endif // MIDDLESEARCHWIDGET_H
