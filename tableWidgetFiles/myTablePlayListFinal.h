#ifndef MYTABLEPLAYLISTFINAL_H
#define MYTABLEPLAYLISTFINAL_H

#include <QObject>
#include <QWidget>
#include<QScrollArea>
#include<QDebug>
#include<QWheelEvent>

#include"mytablewidget.h"
#include"playingWidget.h"
#include"myshowtablebutton.h"
#include"myMediaList.h"
#include"dataBase/myDataBase.h"
class middleLeftStackWidget0;
class middleWidgetLeft;

class myTablePlayListFinal:public baseWidget
{
    Q_OBJECT
public:
   explicit myTablePlayListFinal(QWidget*parent=0);
    ~myTablePlayListFinal(){}

    void setShowButtonName(QString playListName="新建列表"){ m_Btntable->m_playlistName->setText(playListName);m_PlayListname=playListName;}
    void setMiddleStackWidget0(middleLeftStackWidget0*p){m_midleft0=p;m_table->setinitMiddleStackWidget0(p);}
    void setCurrentSongDuration(const QString &dur){m_table->m_playingWid->m_labduration->setText(dur);}
    void setPlayMode(PlayMode mode){m_playList->setPlayMode(mode);}
    void setCurrentSongAlbumPic(QPixmap);
    void setAutoLayout();

   inline middleLeftStackWidget0* midLeft0(){return m_midleft0;}
   inline playingWidget *getPlayingWidget(){return m_table->m_playingWid;}
   inline QString ShowButtonName(){return m_PlayListname;}
   inline QString currentSongName(){return m_table->m_playingWid->songName();}
   inline myMediaList *playList(){return m_playList;}

   const QString currentAuthor(){return m_table->m_playingWid->m_btnAuthor->text();}
   const QString currentMusicName(){return m_table->m_playingWid->songName();}
   const QString currentSongDuration();

   const QList<QUrl> &songUrlList();
    int currentSongDurationToInt();
    void stopCurrentSong();
    void getlistfromDateBase();
    void addToPlayList(const QString &name,const QString &url,const QString &dur);
    void updateCount(){m_Btntable->slot_updateSongCount();}
    void updateConvientButton();
    void updateBGcolor();

    myTableWidget *m_table;
    myShowTableButton *m_Btntable;
protected:
    virtual void wheelEvent(QWheelEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dropEvent(QDropEvent *);
public slots:
    void slot_emptyList();

    void slot_showHideTable();

    void slot_addSong();
    void slot_addSongFolder();

    void slot_addSongFromSearchTable(const QStringList& name,const QStringList &url,const QStringList &dur);

    void slot_playSongFromSearchTable(const QStringList &name,const QStringList &url,const QStringList &dur);
private:
    myMediaList *m_playList;
    QString m_PlayListname;
    middleLeftStackWidget0 *m_midleft0;
};

#endif //MYTABLEPLAYLISTFINAL_H
