#ifndef MYSHOWTABLEBUTTON_H
#define MYSHOWTABLEBUTTON_H

#include <QObject>
#include <QWidget>
#include <qlabel.h>
#include <QMenu>
#include <QAction>
#include <QLineEdit>

#include"myPushButton.h"
#include"myshowtablebutton.h"

class myTablePlayListFinal;
class myTableWidget;

class myShowTableButton:public QPushButton
{
    Q_OBJECT
public:
   explicit myShowTableButton(QWidget*parent=0);
    void init();
    void initMenu();

    void setTableFinal(myTablePlayListFinal*p);

    void setTableShowIndicator();
    void setTableHideIndicator();
    void setEnabledMenuItem(bool isSetting=false); //menu!!
    void setTipsStyle(bool isset=true);

    QLabel *m_playlistName;
public slots:
    void slot_updateSongCount();
    void slot_menuRequest(){m_menu->exec(QCursor::pos());}
    void slot_returnPressed();
    void slot_ReName();
    void slot_reNameDB(const QString&);

signals:
    void sig_emptyList();
    void sig_addPlayList();
    void sig_addSong();
    void sig_delPlayList();
    void sig_reName(const QString&);
protected:
    virtual void dragLeaveEvent(QDragLeaveEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void dragMoveEvent(QDragMoveEvent*);
    virtual void dragEnterEvent(QDragEnterEvent*);
    virtual void dropEvent(QDropEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);

    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual bool eventFilter(QObject *, QEvent *);
    virtual void paintEvent(QPaintEvent *);
private:
    QLabel *m_indicator;
    QMenu *m_menu;
    myPushButton *m_btnmenu;
    QLineEdit *m_lineEdit;
    bool m_isTipsStyle;
    myTablePlayListFinal *m_finalWid;

    bool m_isdrawTop;
    bool m_isdrawMove;
    QPoint m_presspos;
};

#endif // MYSHOWTABLEBUTTON_H
