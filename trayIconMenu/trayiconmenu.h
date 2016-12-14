#ifndef TRAYICONMENU_H
#define TRAYICONMENU_H

#include <QObject>
#include <QWidget>
#include<QMenu>
#include"mySlider.h"
#include<QPushButton>
#include<qlabel.h>
#include<QDebug>

class  volWidget :public QWidget
{
    Q_OBJECT
public:
    volWidget(QWidget*parent);
    mySlider *m_slider_vol;
protected:

private:
};
///////////////////////////////////////
class  playWidget :public QWidget
{
    Q_OBJECT
public:
    playWidget(QWidget*parent);
    QPushButton *m_play_button;
    QPushButton *m_backward_button;
    QPushButton *m_forward_button;
    QLabel*music_name_label;
private:

};
////////////////////////////////////////
class   trayIconMenu : public QMenu
{
    Q_OBJECT
public:
    trayIconMenu(QWidget*parent=0);
    void   init_level0();
    void   init_level1();
    void   init_level2();
    void    init_level3();
    void    init_level4();
    void    init_level5();
    void    init_level6();


    void setCurrentSongName(QString);
    void setPauseStyle();
    void setPlayStyle();
    volWidget  *m_volWid;
    playWidget *m_wid6;

signals:
    void sig_OpenDeskTopLrc();
    void sig_close();
};

#endif // TRAYICONMENU_H
