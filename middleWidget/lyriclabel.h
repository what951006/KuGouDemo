#ifndef LYRICLABEL_H
#define LYRICLABEL_H

#include <QWidget>
#include <QMenu>
#include <QDebug>
#include <QThread>
#include <Qtimer>

#include"mynetwork.h"
#include"lyric.h"
#include"baseWidget.h"

class mainWindow;


class AbstractWheelWidget : public baseWidget
{
    Q_OBJECT

    public:
        AbstractWheelWidget(bool touch, QWidget *parent = 0);
        virtual ~AbstractWheelWidget();

        inline  int currentIndex() const;
        void setCurrentIndex(int index);

        virtual void paintItem(QPainter* painter, int index, QRect &rect) = 0;
        virtual void paintItemMask(QPainter* painter)= 0;

        virtual int itemHeight() const = 0;
        virtual int itemCount() const = 0;

    public slots:
        void scrollTo(int index);

    signals:
        void stopped(int index);
        void changeTo(int index);

    protected:
        virtual void paintEvent(QPaintEvent *e);
        bool event(QEvent*);

        int m_currentItem;
        int m_itemOffset; // 0-itemHeight()
        bool m_isScrolled;
        bool m_doSignal;//滚动是否触发信号,用于区别是否是人为滚动

        QRect m_currentRollrect;
        QFont m_currentMaskFont;
        float m_maskLength;
        QString m_realCurrentText;
};

class Lyric;
class QFont;

class LyricLabel : public AbstractWheelWidget
{
        Q_OBJECT
    public:
        LyricLabel(bool touch, QWidget *parent = 0);
        ~LyricLabel(){}
        void setInitMainWindow(mainWindow *p){m_mainwindow=p;}

        void setCurLrcChangeTime(int time,bool isadding=true); //改变歌词当前的时间;

        void analyzeLrc(const QByteArray&,const QString filepath=NULL);
        virtual void paintItem(QPainter* painter, int index, QRect &rect);
        virtual void paintItemMask(QPainter *painter);

       inline const QString currentLrc(){return  m_lyric->getLineAt(m_currentItem);}
       inline const QString nextLrc(){return  m_lyric->getLineAt(m_currentItem+1);}

        void clearLrc();
        int itemHeight() const;
        int itemCount() const;

         Lyric *m_lyric;
    signals:
        void changeToPosition(qint64 pos);
        void rightClicked();
        void mouseEnter();
        void sig_currentLrcChange(const QString&,const QString&); //this signal is for desktop lyric
        void sig_currentPrecentChange(const QString&,float &precent,int &interval);

    public slots:

        void slot_timerWork();
        void slot_openLocalLrcFile();

        void setForwardHalfSecond();
        void setBackHalfSecond();
        void setForwardTwoSeconds();
        void setBackTwoSeconds();
        void setSingerBGShowOrNot();
        void positionChanged(qint64 length);
        void setPostion(qint64 pos);

        void slot_replyLrc(const QByteArray &byt,const QString& songname);
    protected slots:
        void changeToEvent(int index);
        void changeFont();
        void changeHightLightColor();

    protected:
        void contextMenuEvent(QContextMenuEvent *event);//右击事件
        void enterEvent(QEvent *e);

        float m_itemPrecent;

        int m_pos;
        QMenu *m_menu;
        QFont m_lrcFont;
        QColor m_lrcHightLight;
private:
        mainWindow *m_mainwindow;
};

#endif // LYRICLABEL_H
