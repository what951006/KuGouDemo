#ifndef MYMEDIALIST_H
#define MYMEDIALIST_H
#include <QUrl>
#include <QObject>

class myTablePlayListFinal;


enum PlayMode{playInOrder=0,playRandom=1,playOneCircle=2};

class myMediaList : public QObject
{
    Q_OBJECT
public:
    explicit myMediaList(QObject *parent = 0);
 /*pass pointer*/
    void setTableFinal(myTablePlayListFinal *pParent){m_pFinal=pParent;}

 /**/
    QUrl mediaUrl(int index);//赋值给musicIndex;
    inline  void addPlayList(const QUrl& url){m_list.append(url);}

    void setPlayMode(PlayMode);
    void setCurIndex(int index){m_musicIndex=index;}
    int nextMediaIndex();
    int preMediaIndex();

    QList<QUrl> m_list;
public Q_SLOTS:
    void slot_removeSong(int index);
signals:
private:
    int indexMode;
    int m_musicIndex;

    myTablePlayListFinal *m_pFinal;
};

#endif // MYMEDIALIST_H
