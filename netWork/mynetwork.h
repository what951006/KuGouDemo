#ifndef MYNETWORK_H
#define MYNETWORK_H

#include<QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<Qimage>
#include<QPixmap>

class MyNetWork : public QObject
{
    Q_OBJECT

public:
    explicit MyNetWork(QObject *parent = 0);
    ~MyNetWork();
    const QImage &BgWhiteChange(QImage& image , int brightness);
signals:
    void sig_requestMvfinished(const QString&);
    void sig_reqSongfinished(const QByteArray&);
    void sig_reqSongNextPagefinished(const QByteArray&);

    void dolrcworkfinished(const QByteArray&,const QString&);
    void setpic(const QByteArray&,const QString&);
    void sig_setBgpix(const QVector<QPixmap>&,const QString& author);
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
