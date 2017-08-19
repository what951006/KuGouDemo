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
/*static functions*/
    static myTablePlayListFinal* getCurrentList(){return s_pCurList;}

    static void setCurrentList(myTablePlayListFinal*pList){s_pCurList=pList;}

/*pass the pointer*/
    void setMiddleStackWidget0(middleLeftStackWidget0*p){m_midleft0=p;m_table.setMiddleStackWidget0(p);}

/*other functions*/
    void setShowButtonName(QString playListName="新建列表"){ m_Btntable.m_playlistName->setText(playListName);m_PlayListname=playListName;}

    void setCurrentSongDuration(const QString &dur){m_table.m_playingWid.m_labduration.setText(dur);}

    void setPlayMode(PlayMode mode){m_playList.setPlayMode(mode);}

    void setCurrentSongAlbumPic(const QString &strPath);

    void setAutoLayout();


    playingWidget *getPlayingWidget(){return &m_table.m_playingWid;}

    QString ShowButtonName(){return m_PlayListname;}


   const QString currentAuthor(){return m_table.m_playingWid.m_btnAuthor.text();}

   const QString currentMusicName(){return m_table.m_playingWid.songName();}

   const QString currentSongDuration();


    int currentSongDurationToInt();

    void stopCurrentSong();

    void getlistfromDateBase();

    void addToPlayList(const QString &name,const QString &url,const QString &dur, const QString &strHash="");

    void updateCount(){m_Btntable.slot_updateSongCount();}

    void updateConvientButton();

    void updateBGcolor();

////////////////////////////////playList operation
    myMediaList *mediaList(){return &m_playList;}

    int getIndexByUrl(const QString &strUrl){return m_playList.GetList().indexOf(strUrl);}

    QString getUrlByIndex(int index){return m_playList.GetList().value(index);}

    bool isContainUrl(const QString&strUrl){return m_playList.GetList().contains(strUrl);}
//////////////////////////////////////


    myShowTableButton m_Btntable;
    myTableWidget m_table;
    myMediaList m_playList;
    QString m_PlayListname;
    middleLeftStackWidget0 *m_midleft0;
public slots:
    void slot_emptyList();

    void slot_showHideTable();

    void slot_addSong();

    void slot_addSongFolder();

    void slot_addSongFromSearchTable(const QStringList& name,const QStringList &url,const QStringList &dur);

 //   void slot_playSongFromSearchTable(const QStringList &name,const QStringList &url,const QStringList &dur);
protected:
    static myTablePlayListFinal *s_pCurList;

    virtual void wheelEvent(QWheelEvent *);

    virtual void paintEvent(QPaintEvent *);

    virtual void dragEnterEvent(QDragEnterEvent *);

    virtual void dropEvent(QDropEvent *);

};

#endif //MYTABLEPLAYLISTFINAL_H
