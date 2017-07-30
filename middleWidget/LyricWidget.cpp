#include "LyricWidget.h"
#include "decodekrc.h"
#include <time.h>
QTimer *g_timer;
#define SCROLL_TIME 200
#define SCROLL_TIMER_TIME 10


LyricWidget::LyricWidget(QWidget *parent)
    : QWidget(parent)
{
    m_nRowHeight=50;
    m_normalFont.setFamily("宋体");
    m_normalFont.setPixelSize(12);
    m_nCurIndex=0;
    m_nOffset=0;

    g_timer=new QTimer;
    connect(g_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));
  //  g_timer->start(10);

//------------test---------------
 /*   m_lrcMap.insert(10,"明天你好");
    m_lrcMap.insert(120,"明天东奔西走你好");
    m_lrcMap.insert(130,"明fvc天你好");
    m_lrcMap.insert(140,"明adsf天你好");
    m_lrcMap.insert(150,"明天你fqwehb好");
    m_lrcMap.insert(160,"明xcv天你好");
    m_lrcMap.insert(170,"明天asdfzcvasdf你好");*/

/*  QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,[&](){
        g_timer->start(10);
    });
    timer->start(2000);
*/
//------------test----------------
/*  qDebug()<<GetLrcByTime(1);
    qDebug()<<GetLrcByTime(130);
    qDebug()<<GetLrcByTime(101);
    qDebug()<<GetLrcByTime(1);
    qDebug()<<GetLrcByTime(1);
*/

}

LyricWidget::~LyricWidget()
{

}
void LyricWidget::slot_timer()
{
    m_nOffset-= m_nRowHeight/(SCROLL_TIME/SCROLL_TIMER_TIME);

    if(abs(m_nOffset)>=m_nRowHeight)
    {
        m_nOffset=0;
        g_timer->stop();
        m_nSroIndex=m_nCurIndex;
    }
    update();
}
void LyricWidget::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    QString strTemp;
    int nRowCount=height()/m_nRowHeight;
    for(int i=0;i<nRowCount;++i)
    {
        strTemp= GetLrcByIndex((m_nSroIndex-nRowCount/2)+i);

        DrawItem(painter,i*m_nRowHeight+m_nOffset,strTemp);
    }

}

void LyricWidget::DrawItem(QPainter&Painter,int Y,const QString &strText,Qt::Alignment align)
{
    if(Qt::AlignCenter==align)
    {
        QFontMetrics metric(m_normalFont);
        int X= (width()-metric.width(strText))/2;
        Y+=(m_nRowHeight-metric.height())/2;
        Painter.setPen(QColor(255,255,255));
        Painter.drawText(X,Y,strText);
    }
}
inline QString LyricWidget::GetLrcByIndex(int index)
{
    if(index<0)
        return "";
    else if(index>m_lrcMap.size())
        return "";
        return m_lrcMap.values().value(index);
}

QString LyricWidget::GetLrcByTime(qint64 time)
{
    int index=GetIndexByTime(time);
    return m_lrcMap.values().value(index);
}
int LyricWidget::GetIndexByTime(qint64 time)
{
    //binary search
    //time complexity O(logn)
    qint64 lt=0;
    qint64 rt= m_lrcMap.values().count();
    qint64 mid=0;
    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (m_lrcMap.keys().value(mid) > time)
            rt = mid;
        else
            lt = mid;
    }
    return lt;
}

void LyricWidget::positionChanged(qint64 length)
{

    int index = GetIndexByTime(length/1000); //using binary search for the current lyric index;
    if (index != m_nCurIndex && 0==m_nOffset)
    {
        qDebug()<<"here"<<index<<length;
     //   m_itemOffset=-1; // have to set this value ,cause when update event was done , the last lyric mask will leave on window!!!
        m_nSroIndex=index-1; //变成上一行~
        m_nCurIndex=index;
        g_timer->start(SCROLL_TIMER_TIME);
    //    emit sig_currentLrcChange(m_lyric->getLineAt(index),m_lyric->getLineAt(index+1));
    }
    // slot_timerWork();
}


void LyricWidget::slot_replyLrc(const QByteArray& byt,const QString& songname)
{
 /*   QString playsongname= m_mainwindow->middleStack0()->nowPlayFinalTable()->currentSongName();
    if(songname==playsongname.replace("&"," "))
    {
        analyzeLrc(byt,QString("D://ExcellentLrc/")+songname+".lrc");
    }*/
}
//-------------------------------------original func------------------------------------------
int LyricWidget::getIndex(qint64 pos)
{
    //binary search
    //time complexity O(logn)
    qint64 lt=0;
    qint64 rt= m_lrcMap.values().count();
    qint64 mid=0;
    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (m_lrcMap.keys().value(mid) > pos)
            rt = mid;
        else
            lt = mid;
    }
    return lt;
}

void LyricWidget::getItemPrecent(qint64 pos, int &interval, float &precent, QString &string)
{
    int index=getIndex(pos);
    qint64 startvalue=getPostion(index);
    qint64 subvalue=pos-startvalue;

    QMap<qint64,qint64> map=m_lrcintervalmap.values().value(index);
    qint64 lt2=0;
    qint64 rt2= map.values().count();
    qint64 mid2=0;

    while (lt2 < rt2-1)
    {
        mid2 = (lt2+rt2)/2;
        if (map.keys().value(mid2) >subvalue)
            rt2 = mid2;
        else
            lt2 = mid2;
    }
    string=m_lrcwordsmap.values().value(index).values().value(lt2);
    interval=map.values().value(lt2);
    precent=(float)lt2/(map.values().count());
}


void LyricWidget::changeLrcFileTime(int time, bool isadd)
{
    QFile file(m_filedir);
    if(!file.exists())
        return;
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream stream(&file);
    QString all_text= stream.readAll();
    file.resize(0);

    QStringList lines = all_text.split("\n");
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");//这个是时间标签的格式[00:05.54]
                                            //正则表达式d{2}表示匹配2个数字
    foreach(QString oneline, lines)
    {
        int pos = rx.indexIn(oneline, 0);
        QString temp = oneline;
        temp.replace(rx, "");
        QString head="[%1:%2.%3]";
        QString setminutesstr="00";
        QString setsecondstr="00";
        QString setmsecondstr="00";

        while (pos != -1)
        { //表示匹配成功
            QString cap = rx.cap(0);//返回第0个表达式匹配的内容
            // 将时间标签转换为时间数值，以毫秒为单位
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();

            int totalTime=0;
            if(isadd)
                totalTime = minute * 60000 + second * 1000 + millisecond*10+time;
            else
                totalTime = minute * 60000 + second * 1000 + millisecond*10-time;
            totalTime=totalTime<0? 0:totalTime;

            int setminutes=totalTime/60000;//分钟数
            int setsecond=(totalTime-minute*60000)/1000;//秒
            int setmsecond=((totalTime)%1000)/10; //毫秒
/*
 * Change time format
*/
            setminutesstr=QString::number(setminutes);
            if(setminutes>=0&&setminutes<=9)
                setminutesstr=QString::number(0)+setminutesstr;

            setsecondstr=QString::number(setsecond);
            if(setsecond>=0&&setsecond<=9)
                setsecondstr=QString::number(0)+setsecondstr;

            setmsecondstr=QString::number(setmsecond);
            if(setmsecond>=0&&setmsecond<=9)
                setmsecondstr=QString::number(0)+setmsecondstr;
            if(setminutes<0||setsecond<0||setmsecond<0)
            {
                setminutesstr="00";
                setsecondstr="00";
                setmsecondstr="00";
            }

            pos += rx.matchedLength();
            pos = rx.indexIn(oneline, pos);//匹配全部
        }
        if(!temp.isEmpty())
          stream<<head.arg(setminutesstr).arg(setsecondstr).arg(setmsecondstr)<<temp<<("\r\n");//写入！！
    }
    file.close();
}
void LyricWidget::analyzeLrcContent(const QByteArray &KlcData,const QString filedir)
{
    m_lrcMap.clear();
    m_lrcintervalmap.clear();
    m_lrcwordsmap.clear();

    QByteArray getByt;
    QByteArray KlcByt=KlcData;
    KrcDecode(KlcByt,getByt);//here we got lrc text

    QStringList list=QString(getByt).split("\n");
    foreach (QString linestr, list)
    {
        QString temp=linestr;
        QStringList strlist= temp.split("]");

        QString getTimestr=strlist.value(0);
        QString getLrcStr=strlist.value(1);

        QString time= getTimestr.remove("[").split(',').value(0);
      //  QString maskdur=getTimestr.remove("[").split(',').value(1);
        QStringList itemList= getLrcStr.split('<');

        QString lrctotalstr;
        QMap<qint64 ,qint64> map;
        QMap<qint64,QString> wordsmap;
        foreach (QString lrcitem, itemList)
        {
            lrcitem.replace('>',',');
            QStringList tinylist= lrcitem.split(',');
            map.insert(QString(tinylist.first()).toInt(),QString(tinylist.value(1)).toInt()); //第二个就是遮罩时间
            lrctotalstr.append(tinylist.last());
            wordsmap.insert(QString(tinylist.first()).toInt(),QString(tinylist.last()).simplified());
        }
        m_lrcwordsmap.insert(time.toInt(),wordsmap);
        m_lrcintervalmap.insert(time.toInt(),map);
        m_lrcMap.insert(time.toInt(),lrctotalstr);
    }
    m_filedir=filedir;
}
