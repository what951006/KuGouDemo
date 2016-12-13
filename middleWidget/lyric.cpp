#include "lyric.h"
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <QTime>

#include"decodekrc.h"

int Lyric::getIndex(qint64 pos)
{
    //binary search
    //time complexity O(logn)
    qint64 lt=0;
    qint64 rt= m_lrcmap.values().count();
    qint64 mid=0;
    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (m_lrcmap.keys().value(mid) > pos)
            rt = mid;
        else
            lt = mid;
    }
    return lt;
}

void Lyric::getItemPrecent(qint64 pos, int &interval, float &precent, QString &string)
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


void Lyric::changeLrcFileTime(int time, bool isadd)
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
void Lyric::analyzeLrcContent(const QByteArray &KlcData,const QString filedir)
{
    m_lrcmap.clear();
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
        m_lrcmap.insert(time.toInt(),lrctotalstr);
    }
    m_filedir=filedir;
}

