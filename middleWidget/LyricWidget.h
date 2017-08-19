#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H
/**
   @brief：this class is used to lyric widget which can be scrolled automatically
   @date:2017年7月30日 11:10:42
   @author:yantao
   @details:QQ:842420717
*/

#include <QWidget>
#include <Qmap>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QOpenGLWidget>
#include "baseWidget.h"

struct ItemInfo{
    int index;
    int alpha;
    int Y;
    bool hightLight;
    QString strText;
};

class LyricWidget : public baseWidget
{
    Q_OBJECT

public:
    LyricWidget(QWidget *parent = 0);
    ~LyricWidget();

//get current Row lrc by index

    QString GetLrcByIndex(int index);

//get start pos by index
    qint64 GetPosByindex(int index);

//get the lrc by time
    QString GetLrcByTime(qint64 time);

//get index by time
    int GetIndexByTime(qint64 time);

//get mask length
    void GetMaskLen(int nFontSize);


    void analyzeLrcContent(QByteArray&,const QString &filePath);

    void clearLrc();

    void getPosInfo(int &keyTime,int &interval, float &precent, QString &str);
protected:
    virtual void paintEvent(QPaintEvent*event);

private slots:
    void slot_timer();

    void positionChanged(qint64 length);
private:
//Draw single row text
    void DrawItem(QPainter&Painter,ItemInfo &index);

//
    QMap<qint64,QString> m_lineMap;
    QList<QMap<int,QString>> m_word_list;
    QList<QMap<int ,int>> m_interval_list; //interval,间隔时间
    QList<QMap<int ,float>> m_percent_list; //float,比例
//

    QString m_strCurLrc;
    QTimer m_timer;
    QFont m_normalFont;
    qint64 m_nCurPos;
    qint64 m_nCurStartPos;
    float m_nMaskLen;
    int m_nCurIndex;
    int m_nSroIndex;
    int m_nOffset;
    int m_nFontPixSize;
    QColor m_HLColor;   //hightlight
    QColor m_NlColor;   //normal
};

#endif // LYRICWIDGET_H
