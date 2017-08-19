#include "mynetwork.h"
#include<QFile>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QEventLoop>
#include<QDebug>

//play method
//http://m.kugou.com/app/i/getSongInfo.php?cmd=playInfo&hash=
//http://www.kugou.com/yy/index.php?r=play/getdata&hash
//4C285C68EBEFAD7D8602D2D14D48F725
//AFF3B6219C15D030F957B82FF50AA91E

#define USE_NETCLOUD 1

#define BG_URL "http://artistpicserver.kuwo.cn/pic.web?type=big_artist_pic&pictype=url&content=list&&id=0&from=pc&json=1&version=1&width=1920&height=1080&name=%1"
#define SONG_URL "http://itwusun.com/search/wy/%1?&f=json&size=50&p=%2&sign=itwusun"

#define URL_KGPLAY "http://m.kugou.com/app/i/getSongInfo.php?cmd=playInfo&hash=%1"
#define KGLrcPart0 "http://songsearch.kugou.com/song_search_v2?keyword=%1&page=1&pagesize=40&filter=0&bitrate=0&isfuzzy=0&inputtype=2&platform=PcFilter&userid=312986171&clientver=8100&iscorrection=3"
#define KGLrcPart1 "http://lyrics.kugou.com/search?ver=1&man=no&client=pc&keyword=%1&duration=%2&hash=%3"//&hash=9c6fd9b90800f7a37f6821c07bc0f906 9C6FD9B90800F7A37F6821C07BC0F906 b3c9045aa086236dc78a59357bdf73ac
#define KGLrcPart2 "http://lyrics.kugou.com/download?ver=1&client=pc&id=%1&accesskey=%2&fmt=krc"

#define ITWUSUN "http://api.itwusun.com/music/search/wy/%1?format=json&keyword=%2&sign=a5cc0a8797539d3a1a4f7aeca5b695b9"
/*
 *  a new API =====>    http://s.music.163.com/search/get/?type=1&s=  歌曲名/歌手名  &limit=5000
 *  a new API too-----> http://api.itwusun.com/music/search/wy/2?format=json&keyword=陈奕迅&sign=a5cc0a8797539d3a1a4f7aeca5b695b9
*/

/*{"SongName":"告白气球",
"OwnerCount":1874763,
"MvType":2,
"SQFailProcess":4,
"Source":"",
"Bitrate":128,
"HQExtName":"mp3",
"SQFileSize":25058814,
"ResFileSize":43915053,
"SuperExtName":"",
"MvTrac":3,
"SQDuration":215,
"ExtName":"mp3",
"Auxiliary":"",
"SQPkgPrice":1,
"Scid":22084042,
"FailProcess":4,
"HQPkgPrice":1,
"HQBitrate":320,
"HiFiQuality":3,
"Grp":{},
"AlbumPrivilege":8,
"AlbumID":"1645030",
"SuperFileHash":"",
"ASQPrivilege":10,
"M4aSize":887266,
"AlbumName":"周杰伦的床边故事",
"Privilege":8,
"ResBitrate":1632,
"FileHash":"3C3D93A5615FB42486CAB22024945264",
"SQPayType":3,
"HQPrice":200,
"SourceID":0,
"FileName":"周杰伦 - 告白气球",
"ID":"9dba44e08f30315c565a0fb773289dea",
"SuperFileSize":0,
"QualityLevel":3,
"SQFileHash":"B2C0A23919EEE8B47831FFAA2604107F",
"HQPrivilege":10,
"SuperBitrate":0,
"SuperDuration":0,
"ResFileHash":"05CCCE134338BD3B2AAB176218FCB0F9",
"PublishAge":255,
"HQFileHash":"4C285C68EBEFAD7D8602D2D14D48F725",
"A320Privilege":10,
"HQPayType":3,
"TopicUrl":"",
"PayType":3,
"PkgPrice":1,
"SongLabel":"",
"Accompany":1,
"HQFileSize":8624568,
"FileSize":3449934,
"SQPrice":200,
"ResDuration":215,
"SQExtName":"flac",
"Price":200,
"AudioCdn":100,
"SingerName":"周杰伦",
"SQBitrate":931,
"MvHash":"AFF3B6219C15D030F957B82FF50AA91E",
"SQPrivilege":10,
"HQDuration":215,
"OtherName":"",
"HasAlbum":1,
"HQFailProcess":4,
"Duration":216}


check this out===>http://trackercdn.kugou.com/i/v2/?cmd=24
&hash=3c3d93a5615fb42486cab22024945264
&key=0b8d4d681d6cd4403ea2a47907935ae0
&pid=1
&vipToken=
&appid=1001
&version=8100
&token=
&vipType=0
&userid=312986171
&behavior=play
&module=locallist
&album_id=1645030
&cdnBackup=1
&mid=716e60e8f13d81666d8515da60a4313b

*/

MyNetWork::MyNetWork(QObject *parent) : QObject(parent)
{

    m_pageindex=1;
    m_songname="";
}

MyNetWork::~MyNetWork()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyNetWork::requestalbum(const QString &name,const QString &savelocal)
{
    QString songname=name;
    QByteArray songencod(songname.replace("&"," ").toUtf8().toPercentEncoding());
    QNetworkRequest requestalbum1;
    QNetworkAccessManager mangeralbum1;
    requestalbum1.setUrl(QUrl("http://music.163.com/api/search/pc"));
    requestalbum1.setRawHeader("Cookie","os=pc");
    requestalbum1.setRawHeader("Host","music.163.com");
    requestalbum1.setRawHeader("MUSIC_U","5339640232");
    requestalbum1.setRawHeader("Referer","http://music.163.com/");
    requestalbum1.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply *reply1= mangeralbum1.post(requestalbum1,"offset=0&total=true&limit=100&type=1&s="+songencod);

///loop1
    QEventLoop loop1;
    connect(&mangeralbum1,SIGNAL(finished(QNetworkReply*)),&loop1,SLOT(quit()));
    loop1.exec();
////
    QByteArray byt1=reply1->readAll();
    if(reply1->error()==QNetworkReply::NoError)
    {
        QJsonDocument doc=QJsonDocument::fromJson(byt1);
        QJsonObject obj0=doc.object();
        QJsonObject obj1=obj0.value("result").toObject();
        QJsonArray arry=obj1.value("songs").toArray();
        QJsonObject obj3= arry.at(1).toObject();
        QJsonObject obj4=obj3.value("album").toObject();
        QString picurl=obj4.value("picUrl").toString();

        QNetworkRequest requestalbum2;
        QNetworkAccessManager mangeralbum2;
        requestalbum2.setUrl(picurl);
        QNetworkReply *reply2= mangeralbum2.get(requestalbum2);
///loop2
        QEventLoop loop2;
        connect(&mangeralbum2,SIGNAL(finished(QNetworkReply*)),&loop2,SLOT(quit()));
        loop2.exec();
///
        if(reply2->error()==QNetworkReply::NoError)
        {
            QByteArray byt=reply2->readAll();
    ///save
            QPixmap pix;
            pix.loadFromData(byt);
            pix.save(savelocal);
    ///
            emit setpic(savelocal,name);
            reply2->deleteLater();
        }
        else
        {
            reply1->deleteLater();
            reply2->deleteLater();
            return;
        }
    }
    else
    {
        reply1->deleteLater();
        return;
    }
    reply1->deleteLater();

}


///////////////////////////////////////////////////////////////////////////////////////
void MyNetWork::requestSong(const QString &strReq)//请求歌曲
{
    /*QString songname=str;
    QByteArray byt=songname.replace("&"," ").toUtf8().toPercentEncoding();
    QNetworkRequest requestsong;
    QNetworkAccessManager mangersong;
#if USE_NETCLOUD
    requestsong.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    requestsong.setRawHeader("Referer","http://music.163.com");
    requestsong.setRawHeader("Origin","http://music.163.com");
    requestsong.setRawHeader("Host","music.163.com");
    requestsong.setRawHeader("Connection","Keep-Alive");
//    requestsong.setRawHeader("Accept-Encoding","gzip,deflate");



    requestsong.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    requestsong.setUrl(QUrl("http://music.163.com/api/search/pc"));
    QNetworkReply *reply1= mangersong.post(requestsong,"offset=0&limit=40&type=1&s="+byt);


#else
    QString Url = ITWUSUN.arg(1).arg(songname);
    requestsong.setUrl(Url);
    QNetworkReply *reply1= mangersong.get(requestsong);
#endif


///loop1
    QEventLoop loop1;
    connect(reply1,SIGNAL(finished()),&loop1,SLOT(quit()));
    loop1.exec();
///

   if(reply1->error()==QNetworkReply::NoError)
   {
        QByteArray arry=reply1->readAll();

        emit sig_reqSongfinished(arry);
   }
   else
   {
       reply1->deleteLater();
       return;
   }
   reply1->deleteLater();

   m_pageindex=1;
   m_songname=str;*/
/**
  @brief:kuGou API search，hash
*/

    ItemResult Item={0};
     emit sig_reqSongStatus(Item,SearchStatus::Started);

    QString strTemp(KGLrcPart0);
    strTemp=strTemp.arg(strReq);

    QNetworkRequest request;
    QNetworkAccessManager manger;
    request.setUrl(strTemp);
    QNetworkReply *reply=manger.get(request);

    QEventLoop loop;
    connect(&manger,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    loop.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytTemp= reply->readAll();
        reply->deleteLater();
        QJsonDocument doc0=QJsonDocument::fromJson(bytTemp);
        QJsonObject objTemp=doc0.object();
        objTemp= objTemp.value("data").toObject();
        QJsonArray array0= objTemp.value("lists").toArray();
        for(int i=0;i<array0.size();++i)
        {
            objTemp= array0.at(i).toObject();

            Item.strFullName=objTemp.value("FileName").toString();
            Item.strHash= objTemp.value("FileHash").toString();
            Item.strMusicName= objTemp.value("SongName").toString();
            Item.strSinger=objTemp.value("SingerName").toString();
            Item.strAlbum=objTemp.value("AlbumName").toString();
            int ndur=objTemp.value("Duration").toInt();
            QTime time(0, ndur/60, ndur%60);
            Item.strDur= time.toString("mm:ss");

            strTemp=URL_KGPLAY;
            strTemp=strTemp.arg(Item.strHash);
            request.setUrl(strTemp);

            QNetworkReply *reply=manger.get(request);
            loop.exec();
            if(reply->error()==QNetworkReply::NoError)
            {
                 bytTemp= reply->readAll();
                 QJsonDocument doc0=QJsonDocument::fromJson(bytTemp);
                 objTemp=doc0.object();
                 Item.strUrl="";
                 Item.strUrl= objTemp.value("url").toString();
                 emit sig_reqSongStatus(Item,SearchStatus::Searching);
            }
            reply->deleteLater();
        }
    }
    emit sig_reqSongStatus(Item,SearchStatus::Finished);
    reply->deleteLater();

/* QJsonDocument doc0=QJsonDocument::fromJson(byt0);
    QJsonObject obj0=doc0.object();
    QJsonObject obj01= obj0.value("data").toObject();
    QJsonArray array0= obj01.value("lists").toArray();
    QJsonObject obj02= array0.at(0).toObject();
    QString hash= obj02.value("FileHash").toString();*/

}



void MyNetWork::requestSongNextPage()
{
    m_pageindex++;
    QByteArray byt=m_songname.replace("&"," ").toUtf8().toPercentEncoding();
    QNetworkRequest requestsong;
    QNetworkAccessManager mangersong;
#if USE_NETCLOUD
    requestsong.setUrl(QUrl("http://music.163.com/api/search/pc"));
    requestsong.setRawHeader("Origin","http://music.163.com");
    requestsong.setRawHeader("Host","music.163.com");
    requestsong.setRawHeader("MUSIC_U","5339640232");
    requestsong.setRawHeader("Referer","http://music.163.com/");
    requestsong.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QByteArray bytarray="offset=0&limit=100&type=1&s="+byt;

    QNetworkReply *reply1= mangersong.post(requestsong,bytarray);
#else
    QString Url = ITWUSUN.arg(m_pageindex).arg(m_songname);
    requestsong.setUrl(Url);
    QNetworkReply *reply1= mangersong.get(requestsong);
#endif
///loop1
    QEventLoop loop1;
    connect(reply1,SIGNAL(finished()),&loop1,SLOT(quit()));
    loop1.exec();
///
   if(reply1->error()==QNetworkReply::NoError)
   {
        QByteArray arry=reply1->readAll();
        emit sig_reqSongNextPagefinished(arry);
   }
   reply1->deleteLater();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MyNetWork::requestlrc(const QString &lrcname,qint64 nTotalTime,const QString &lrcloaction)//请求歌词
{
    if(0==nTotalTime)
        return;
    QString songname=lrcname;
    songname.replace("&"," ");

    QString strTemp(KGLrcPart0);
    strTemp=strTemp.arg(songname);

    QNetworkRequest requestlrc0;
    QNetworkAccessManager mangerlrc0;
    requestlrc0.setUrl(strTemp);
    QNetworkReply *reply0=mangerlrc0.get(requestlrc0);

    QEventLoop loop0;
    connect(&mangerlrc0,SIGNAL(finished(QNetworkReply*)),&loop0,SLOT(quit()));
    loop0.exec();

    if(reply0->error()!=QNetworkReply::NoError)
    {
        reply0->deleteLater();
        return;
    }
    QByteArray byt0= reply0->readAll();
    QJsonDocument doc0=QJsonDocument::fromJson(byt0);
    QJsonObject obj0=doc0.object();
    QJsonObject obj01= obj0.value("data").toObject();
    QJsonArray array0= obj01.value("lists").toArray();
    QJsonObject obj02= array0.at(0).toObject();
    QString hash= obj02.value("FileHash").toString();


    strTemp=KGLrcPart1;
    QNetworkRequest requestlrc1;
    QNetworkAccessManager mangerlrc1;
    requestlrc1.setUrl(strTemp.arg(songname).arg(nTotalTime).arg(hash));
    QNetworkReply *reply1= mangerlrc1.get(requestlrc1);
///loop1
    QEventLoop loop1;
    connect(&mangerlrc1,SIGNAL(finished(QNetworkReply*)),&loop1,SLOT(quit()));
    loop1.exec();
////
    if(reply1->error()!=QNetworkReply::NoError)
    {
        reply0->deleteLater();
        reply1->deleteLater();
        return;
    }
    QByteArray byt=reply1->readAll();
    QJsonDocument doc=QJsonDocument::fromJson(byt);
    QJsonObject obj=doc.object();
    QJsonArray arry=obj.value("candidates").toArray();
    QJsonObject obj1= arry.at(0).toObject();
    QString accesskey= obj1.value("accesskey").toString();
    QString id=obj1.value("id").toString();


    strTemp=KGLrcPart2;
    QNetworkRequest requestlrc2;
    QNetworkAccessManager mangerlrc2;
    requestlrc2.setUrl(strTemp.arg(id).arg(accesskey));
    QNetworkReply *reply2= mangerlrc2.get(requestlrc2);
///loop2
    QEventLoop loop2;
    connect(&mangerlrc2,SIGNAL(finished(QNetworkReply*)),&loop2,SLOT(quit()));
    loop2.exec();
///
    if(reply2->error()==QNetworkReply::NoError)
    {
            QByteArray byt=reply2->readAll();
            QJsonDocument doc=QJsonDocument::fromJson(byt);
            QJsonObject obj=doc.object();
            QByteArray utf8byt=obj.value("content").toString().toUtf8();
            QByteArray bytfrom64=QByteArray::fromBase64(utf8byt);
            if(bytfrom64.size()!=0)
            {
                emit dolrcworkfinished(bytfrom64,lrcname);//发送做完的信号
                ////////////////////////////用于保存
                QFile file(lrcloaction);
                file.resize(0);
                if(file.open(QIODevice::WriteOnly))//如果打开失败
                {
                    file.write(bytfrom64); //write the kugou source krc
                    file.close();
                }
            }
    }
    reply0->deleteLater();
    reply1->deleteLater();
    reply2->deleteLater();
}

const QImage &MyNetWork::BgWhiteChange(QImage &image , int brightness)
    {
        uchar *line =image.scanLine(0);
            uchar *pixel = line;
            for (int y = 0; y < image.height(); ++y)
            {
                pixel = line;
                for (int x = 0; x < image.width(); ++x)
                {
                    *pixel = qBound(0, *pixel + brightness, 255);
                    *(pixel + 1) = qBound(0, *(pixel + 1) + brightness, 255);
                    *(pixel + 2) = qBound(0, *(pixel + 2) + brightness, 255);
                    pixel += 4;
                }
                line += image.bytesPerLine();
            }
            return image;
}

void MyNetWork::requestMv(const QString &mvname)
{
    QString strTemp(ITWUSUN);
    QByteArray byt=QString(mvname).replace("&"," ").toUtf8().toPercentEncoding();
    QNetworkRequest request;
    QNetworkAccessManager manger;
    request.setUrl(strTemp.arg(1).arg(QString(byt)));
    QNetworkReply *reply1= manger.get(request);
///loop1
    QEventLoop loop1;
    connect(reply1,SIGNAL(finished()),&loop1,SLOT(quit()));
    loop1.exec();
///

   if(reply1->error()==QNetworkReply::NoError)
   {
        QByteArray arry=reply1->readAll();

        QJsonDocument doc=QJsonDocument::fromJson(arry);
        QJsonArray array=doc.array();
        QJsonObject obj=array.at(0).toObject();
        QString url= obj.value("MvUrl").toString();//添加mp3Url
        if(!url.isEmpty())
         emit sig_requestMvfinished(url);
   }
   else
   {
       reply1->deleteLater();
       return;
   }
   reply1->deleteLater();
}

void MyNetWork::requestBgPic(const QString &author)
{
    QString strTemp(BG_URL);
    strTemp=strTemp.arg(author);

    QNetworkAccessManager manger;
    QNetworkReply *reply;
    reply=manger.get(QNetworkRequest(strTemp));

    QEventLoop loop;
    connect(&manger,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    loop.exec();
    QByteArray bytTemp=reply->readAll();
    reply->deleteLater();

    QJsonDocument doc=QJsonDocument::fromJson(bytTemp);
    QJsonObject obj=doc.object();
    QJsonArray array=obj.value("array").toArray();

    QStringList picList;
    for(int i=0;i<array.count();i++)
    {
       obj= array.at(i).toObject();
       strTemp=obj.value("bkurl").toString();
       if(!strTemp.isEmpty())//如果不为空
       {
           reply=manger.get(QNetworkRequest(strTemp));
           connect(&manger,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
           loop.exec();

           bytTemp=reply->readAll();

           QImage image;
           image.loadFromData(bytTemp);
           image=BgWhiteChange(image,-50);
           strTemp=QString("D:/ExcellentAlbum/%1/%2.jpg").arg(author).arg(i);
           image.save(strTemp);


           picList<<strTemp;
           reply->deleteLater();
       }
    }
    if(!picList.isEmpty())
        emit sig_setBgpix(picList,author);
}
