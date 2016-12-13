#ifndef MIDDLELEFTSTACKWIDGET0_H
#define MIDDLELEFTSTACKWIDGET0_H

#include <QObject>
#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>
#include <QThread>

#include"myshowtablebutton.h"
#include"baseWidget.h"

class mainWindow;
class myTablePlayListFinal;
class myTableWidget;
class AddLoveListTips;
class myDateBase;
class playingWidget;
class myMediaPlayCore;
class myMediaList;
class middleConvientTwoButton;
class middleListSearch;
class myPlayer;


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

    void showAddtips();
    void showRemovetips();
    void setInitMainWindow(mainWindow*);
    void setAutoLayout();
    void setOriginalStatus();
    void setPlayListConnection();
    void setSwapList( myTablePlayListFinal*begin, myTablePlayListFinal*after);
    void setListTakeAndInsert( myTablePlayListFinal*start, myTablePlayListFinal*after); //add to butt of myTablePlayListFinal*after

    inline mainWindow* mainWindows(){return m_mainWindow;}
    inline middleListSearch* middleListSearchs(){return m_searchwid;}
    inline myTablePlayListFinal* nowPlayFinalTable(){return m_nowplayfinaltable;}
    inline QVector<myTablePlayListFinal*> & myTablePlayListFinalVector(){return m_Vector;}
    inline myShowTableButton* convientShowTableBtn(){return m_convientSTBtn;}

    baseWidget *m_wid;
public Q_SLOTS:
    void slot_playMVIndex(int);
    void slot_showMvWidget(const QString&);
    void slot_searchSong(const QString&);
    void scrolltoCurrentPlayList();
    void slot_verScrBarChange(int);
    void slot_setlabelpic(QByteArray,QString songname);
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
    myShowTableButton*m_convientSTBtn;
    QVector<myTablePlayListFinal*> m_Vector;
    myTablePlayListFinal *m_nowplayfinaltable;
    myTablePlayListFinal *m_table;
    middleListSearch *m_searchwid;
    middleConvientTwoButton *m_convtwowid;
    AddLoveListTips *m_addTips;
    QVBoxLayout *vlyout1;
    mainWindow *m_mainWindow;
};

#endif // MIDDLELEFTSTACKWIDGET0_H
