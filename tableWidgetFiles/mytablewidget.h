#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QObject>
#include <QWidget>
#include<QTableWidget>
#include<QScrollArea>
#include<qpropertyanimation.h>
#include<qdebug.h>
#include<QMenu>
#include<QDesktopWidget>
#include<QApplication>

#include"pushButtonOnTable.h"
#include"playingWidget.h"
#include"myTableAddWidget.h"

class middleLeftStackWidget0;
class myTablePlayListFinal;

class myTableWidget : public QTableWidget
{
    Q_OBJECT
    friend class playingWidget;

public:
    explicit myTableWidget(QWidget *parent = Q_NULLPTR);
     void init();
     void initMenu();
     void initPlayingWidget();

     void setinitMyTableFinal(myTablePlayListFinal*f){m_finalWidget=f;}       //it must to be initialized
     void setinitMiddleStackWidget0(middleLeftStackWidget0*p){m_middleftStack0=p;}  //need to initialization
     const QString getHoverDuration(){return m_text;}

     void setItem(int row, int column, QTableWidgetItem *item){QTableWidget::setItem(row,column,item);emit sig_RowCountChange();}
     void removeRow(int i){QTableWidget::removeRow(i);emit sig_RowCountChange();}
     int currentSongIndex();

     void setAutoLayoutSize();
     void updatePlayingWidgetPos();
     void removeSong(int row,bool setAutoLayout=true);

     playingWidget *m_playingWid;
protected:
    virtual  void resizeEvent(QResizeEvent*);
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void enterEvent(QEvent*e){QTableWidget::enterEvent(e);setCursor(Qt::ArrowCursor);}
    virtual void leaveEvent(QEvent *);
    virtual void wheelEvent(QWheelEvent *);
    virtual bool eventFilter(QObject *, QEvent *);

    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

public Q_SLOTS:

     void slot_mvclicked();
     void slot_playingWidgetLoveBtnClicked();
     void slot_btnloveclicked();

     void slot_rowCountChanged();
     void slot_actplay();

     void slot_cellClicked(int,int);
     void slot_removeHoverRow();
     void slot_removeRow();
     void slot_cellEnter(int,int);
     void slot_animation(QVariant&);
     void slot_doublick(int r,int,bool isMv=0);
     void slot_menuRequest(QPoint);//
     void slot_moveToPList();
     void slot_playingWidgetDelBtnClicked();

signals:
     void sig_requestMv(QString);
     void sig_setLoveState(bool);

     void sig_playMv(int);
     void sig_play(int);
     void sig_delIndex(int);

     void sig_addSongFolder();
     void sig_addSong();

     void sig_RowCountChange();
private:
     int m_prebgItem;
     int m_loveNowRow;//其余列表在喜欢列表的index处

     myTableAddWidget *m_addWid;

     QMenu *m_menu;
     QMenu *m_Addtoplistmenu;
     QString m_text;

     bool m_enter;
     bool m_isGiven;

     pushButtonCrossWidget *m_crossWid;
     pushButtonGroupWidget *m_groupWid;


     myTablePlayListFinal*m_finalWidget;
     middleLeftStackWidget0 *m_middleftStack0;
};


#endif // MYTABLEWIDGET_H
