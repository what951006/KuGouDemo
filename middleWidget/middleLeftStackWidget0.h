#ifndef MIDDLELEFTSTACKWIDGET0_H
#define MIDDLELEFTSTACKWIDGET0_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>
#include <QThread>


#include "baseWidget.h"
#include "myshowtablebutton.h"
#include "middleconvienttwobutton.h"
#include "mainWindowContentWidget.h"
#include "mynetwork.h"
#include "myTablePlayListFinal.h"





class myScrollArea:public QScrollArea
{
    Q_OBJECT
public:
    explicit myScrollArea(QWidget *parent = Q_NULLPTR);
protected:
    void mousePressEvent(QMouseEvent *){setFocus();}
private:
};
//////////////////////////////////////////////////////////////////
class middleLeftStackWidget0: public myScrollArea
{
    Q_OBJECT
public:
   explicit middleLeftStackWidget0(QWidget *parent);
    ~middleLeftStackWidget0(){}
    void init();

    void initAddTips();

    void initConvientWidget();

    void initConnection();

    void updateBGColor();

    bool isEnableMoveList(myTablePlayListFinal*list);

    void showAddtips(){m_addTips.showAddTips();}

    void showRemovetips(){m_addTips.showRemoveTips();}

    void setAutoLayout();

    void setOriginalStatus();

    void setSwapList( myTablePlayListFinal*begin, myTablePlayListFinal*after);

    void setListTakeAndInsert( myTablePlayListFinal*start, myTablePlayListFinal*after); //add to butt of myTablePlayListFinal*after

    void addMusicToDefaultList(const ItemResult& ,bool bPlay);

    void addMusicToList(const ItemResult&result,myTablePlayListFinal*);

    baseWidget *GetWidget(){return &m_wid;}

    middleListSearch* middleListSearchs(){return &m_searchwid;}

    QVector<myTablePlayListFinal*> & myTablePlayListFinalVector(){return m_Vector;}

    myShowTableButton* convientShowTableBtn(){return &m_convientSTBtn;}

    myTablePlayListFinal *getLoveList(){return &m_lovedList;}

    myTablePlayListFinal *getDefaultList(){return &m_defaultList;}
public Q_SLOTS:
    void slot_playMVIndex(int);

    void slot_showMvWidget(const QString&);

    void slot_localSearch(const QString&);

    void scrolltoCurrentPlayList();

    void slot_verScrBarChange(int);

    void slot_setlabelpic(const QString&strPath,const QString &songname);

    void slot_removePlayList();

    void addPlayList(const QString&);

    void slot_addPlayListWithRename();

    void slot_playIndex(int index);

    void slot_endOfMedia();

    void slot_btnnextSong();

    void slot_btnpreSong();
protected:
    virtual void resizeEvent(QResizeEvent *);
private:
    baseWidget m_wid;
    myShowTableButton m_convientSTBtn;
    QVector<myTablePlayListFinal*> m_Vector;
    middleListSearch m_searchwid;
    middleConvientTwoButton m_convtwowid;
    AddLoveListTips m_addTips;
    QVBoxLayout m_vlyout;
    myTablePlayListFinal m_defaultList;
    myTablePlayListFinal m_lovedList;
};

#endif // MIDDLELEFTSTACKWIDGET0_H
