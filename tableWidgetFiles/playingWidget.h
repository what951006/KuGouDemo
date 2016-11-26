#ifndef PLAYINGWIDGET_H
#define PLAYINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTableWidgetItem>
#include <QLabel>

#include"baseWidget.h"
#include"myPushButton.h"

class playingWidget:public baseWidget
{
    Q_OBJECT
    friend class myTableWidget;
public:
    explicit playingWidget(QWidget *parent = 0);
    void init();
    void setLoveState();
    void setUnloveState();
    bool isLoved(){return m_islove;}
    int currentSongIndex();
    void setCurrentSongItem(QTableWidgetItem*);


    void setOriginalState();
    void setSongName(const QString&);
    const QString songName(){return m_songName;}

    playingWidgetBtn *m_btnSongName;
    playingWidgetBtn *m_btnAuthor;

    myPushButton *m_btnCross;
    myPushButton *m_btnLab;

    QLabel *m_labduration;
    myPushButton *m_btndownload;
    myPushButton *m_btnMovie;
    myPushButton *m_btnLove;
    myPushButton *m_btnDel;
    myPushButton *m_btnMore;

    QTableWidgetItem *m_item;
public slots:
    void slot_readySearch();
signals:
    void sig_clicked(const QString &);
private:
    QString m_songName;
    bool m_islove;
};

#endif // PLAYINGWIDGET_H
