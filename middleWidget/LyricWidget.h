#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H
/**
   @brief：this class is used for lyric widget which scroll automatically
   @date:2017年7月30日 11:10:42
   @author:yantao
*/

#include <QWidget>
#include <Qmap>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QFile>


class LyricWidget : public QWidget
{
    Q_OBJECT

public:
    LyricWidget(QWidget *parent = 0);
    ~LyricWidget();
/**
   @brief:set signle row height
*/
    void SetItemHeight(int Row){m_nRowHeight=Row;}
/**
   @brief:get current Row lrc by index
*/
    QString GetLrcByIndex(int index);
/**
  @brief:get the lrc by time
*/
    QString GetLrcByTime(qint64 time);
/**
  @brief:get index by time
*/
    int GetIndexByTime(qint64 time);
//-----------------original func------------
public:
    Lyric(){}
    void analyzeLrcContent(const QByteArray&,const QString filePath=NULL);
    inline const  QString getLineAt(int index){ return index<0||index>=m_lrcMap.size()? "":m_lrcMap.values().at(index);}
    int getCount(){return m_lrcMap.keys().count();}
    int getIndex(qint64 pos);
    void getItemPrecent(qint64 pos,int &interval,float &precent,QString &string);

    inline  qint64 getPostion(int index){return index>=0&&index <m_lrcMap.values().size()?m_lrcMap.keys().at(index):0;}
    void changeLrcFileTime(int time,bool add=true);
    QString m_filedir;
    QMap<qint64,QMap<qint64,qint64>> m_lrcintervalmap;
    QMap<qint64,QMap<qint64,QString>> m_lrcwordsmap;
//-----------------

protected:
    virtual void paintEvent(QPaintEvent*event);
private slots:
    void slot_timer();

    void slot_replyLrc(const QByteArray &byt,const QString& songname);

    void positionChanged(qint64 length);
private:
/**
      @brief:Draw single row text
*/
    void DrawItem(QPainter&Painter,int Y,const QString &strText,Qt::Alignment align=Qt::AlignCenter);


    QFont m_normalFont;
    QMap<int,QString> m_lrcMap;
    int m_nRowHeight;
    int m_nCurIndex;
    int m_nOffset;
    int m_nSroIndex;
};

#endif // LYRICWIDGET_H
