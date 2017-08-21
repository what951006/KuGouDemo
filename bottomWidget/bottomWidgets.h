#ifndef BOTTOMWIDGETS_H
#define BOTTOMWIDGETS_H

#include <QObject>
#include <QWidget>
#include<QLabel>

#include"baseWidget.h"
#include"myPushButton.h"
#include"mySlider.h"
#include"mainWindowContentWidget.h"
#include"TextMovingWidget.h"
#include"myMediaList.h"
//////////////////////////////////////////////////////////
class mainWindow;

class bottomWidgets : public baseWidget
{
    Q_OBJECT
public:
    explicit bottomWidgets(QWidget *parent = 0);

    void init();

    void setPlayModeBtnStyle(PlayMode);

    void setPauseStyle();

    void setPlayStyle();

    void setBufferStaus();

    void setCurrentSongName(const QString&str){ m_labnowPlayname.setText(str);}

    void setPositon(const QString&strPos){m_labposition.setText(strPos);}

    void setSliderRange(int min,int max){m_mainslider.setRange(min,max);}

    void setSliderValue(int value){m_mainslider.setValue(value);}

    myPushButton m_btnplaymode;
    myPushButton m_btnmore;
    myPushButton m_btndownload;
    myPushButton m_btnfavorite;
    myPushButton m_btnnext;
    myPushButton m_btnprevious;
    myPushButton m_btnPlay;
    mySlider m_mainslider;
    myPushButton m_btnplaylist;
    myPushButton m_btnlrc;
    myPushButton m_btnequalizer;

    volButton2 m_btnvol;
    QLabel m_labposition;
    TextMovingWidget m_labnowPlayname;
protected:
    bool eventFilter(QObject *, QEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
signals:

public slots:
    void slot_setLoveState(bool);
private:

};

#endif // bottomWidgets_H
