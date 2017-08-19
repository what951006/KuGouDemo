#ifndef MYNETWORK_H
#define MYNETWORK_H

#include<QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<Qimage>
#include<QPixmap>

struct ItemResult
{
   QString strFullName;
   QString strUrl;
   QString strMusicName;
   QString strSinger;
   QString strAlbum;
   QString strHash;
   QString strDur;
};
enum SearchStatus{Started=0x00,Searching,Finished};


class MyNetWork : public QObject
{
    Q_OBJECT

public:
    explicit MyNetWork(QObject *parent = 0);
    ~MyNetWork();
    const QImage &BgWhiteChange(QImage& image , int brightness);
signals:
    void sig_requestMvfinished(const QString&);
    void sig_reqSongStatus(const ItemResult&,SearchStatus);
    void sig_reqSongNextPagefinished(const QByteArray&);

    void dolrcworkfinished(const QByteArray&,const QString&);
    void setpic(const QString&strPath,const QString&strName);
    void sig_setBgpix(const QStringList&,const QString& author);
public slots:
    void requestMv(const QString&);
    void requestalbum(const QString &name,const QString &savelocal);
    void requestSong(const QString&);
    void requestSongNextPage();
    void requestlrc(const QString &,qint64 totaltime,const QString &saveloaction);
    void requestBgPic(const QString &author);
private:

    int m_pageindex;
    QString m_songname;
};

#endif // MYNETWORK_H
