#ifndef LYRIC_H
#define LYRIC_H

#include <QVector>
#include<QMap>
class Lyric
{
    public:
        Lyric(){}
        void analyzeLrcContent(const QByteArray&,const QString filePath=NULL);
        inline const  QString getLineAt(int index){ return index<0||index>=m_lrcmap.size()? "":m_lrcmap.values().at(index);}
        int getCount(){return m_lrcmap.keys().count();}
        int getIndex(qint64 pos);
        void getItemPrecent(qint64 pos,int &interval,float &precent,QString &string);

        inline  qint64 getPostion(int index){return index>=0&&index <m_lrcmap.values().size()?m_lrcmap.keys().at(index):0;}
        void changeLrcFileTime(int time,bool add=true);
        QString m_filedir;
        QMap<qint64,QMap<qint64,qint64>> m_lrcintervalmap;
        QMap<qint64,QMap<qint64,QString>> m_lrcwordsmap;
        QMap<qint64,QString> m_lrcmap;
    private:
        double offset;
};

#endif // LYRIC_H
