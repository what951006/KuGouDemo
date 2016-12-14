#include "myDataBase.h"
#include<QSqlError>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<qsqlrecord.h>

////////////////////////////////////////////////加载列表
  QStringList myDataBase::loadPlayList()//只要运行一次就okay了
{
    connectListinfo();
    QSqlQuery query1(QString("SELECT * FROM listinfo order by id"));
          QStringList list;
          while (query1.next()) {
              QString songurl = query1.value("listname").toString();
              list<<songurl;
          }
          return list;
}
 void myDataBase::removePlayList(const QString&  name)
{
    connectListinfo();
    connectSongInfo();
    QSqlQuery query;

    query.exec(QString("delete from listinfo where listname='%1'").arg(name));
    query.exec(QString("delete from songinfo where playlistname= '%1'").arg(name));
 }

 void myDataBase::connectListinfo()
 {
     QSqlDatabase datebase;
     if(QSqlDatabase::contains("qt_sql_default_connection"))
             datebase = QSqlDatabase::database("qt_sql_default_connection");
           else
             datebase = QSqlDatabase::addDatabase("QSQLITE");
     datebase.setDatabaseName("SongDateBase.db");

   datebase.open();
   datebase.exec(QString("CREATE TABLE IF NOT EXISTS listinfo(id integer primary key,listname TEXT)"));//create table

 }

 void myDataBase::connectSongInfo() //just connect once;
 {
     QSqlDatabase datebase;
     if(QSqlDatabase::contains("qt_sql_default_connection"))
             datebase = QSqlDatabase::database("qt_sql_default_connection");
           else
             datebase = QSqlDatabase::addDatabase("QSQLITE");
    datebase.setDatabaseName("SongDateBase.db");
    datebase.open();
    datebase.exec(QString("create table if not exists songinfo(playlistname TEXT,id INT ,songname TEXT,songurl TEXT,duration TEXT)"));
 }

 QVector<QStringList> myDataBase::readListSongInfo(const QString&  listname)
 {
    connectSongInfo();
    QStringList list1;
    QStringList list2;
    QStringList list3;

    QSqlQuery sql_query;
    QString select_sql=QString("select * from songinfo where playlistname=? order by id");
    sql_query.prepare(select_sql);
    sql_query.addBindValue(listname);

    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            QString songname = sql_query.value("songname").toString();
            QString songurl = sql_query.value("songurl").toString();
            QString songdur=sql_query.value("duration").toString();

            list1<<songname;
            list2<<songurl;
            list3<<songdur;
        }
    }

    QVector<QStringList> vec;
    vec<<list1<<list2<<list3;
    return vec;
 }

 void myDataBase::renameList(const QString&  oldname, const QString&  newname)
 {
     connectListinfo();
     connectSongInfo();
     QSqlQuery query;
     query.exec(QString("update listinfo set listname= '%1' where listname= '%2'").arg(newname).arg(oldname));
     query.exec(QString("update songinfo set playlistname= '%1' where playlistname= '%2'").arg(newname).arg(oldname));

 }

 void myDataBase::swapList(const QString &str1, int index1, const QString &str2, int index2)
 {
    connectListinfo();
    QSqlQuery query;
    query.exec(QString("update listinfo set listname= '%1' where id= %2").arg(str2).arg(index1));
    query.exec(QString("update listinfo set listname= '%1' where id= %2").arg(str1).arg(index2));
 }

 void myDataBase::takeAndInsert(const QString &from, const QString &to)
 {
     connectListinfo();
     QStringList list= loadPlayList();
     list.removeOne(from);
     int index= list.indexOf(to);
     list.insert(index+1,from);

     QSqlQuery query;
     query.exec("delete from listinfo where id!=-1"); //清空表

     foreach (QString listname, list)
        addPlayList(listname);
 }

 void myDataBase::emptyList(const QString&  listname)
 {
     connectListinfo();
     connectSongInfo();
     QSqlQuery query;
     query.exec(QString("delete from songinfo where playlistname='%1'").arg(listname));

 }

 void myDataBase::addSong(const QString&  listname, const QString&  songname, const QString&  url, const QString&  duration)
 {
         connectSongInfo();
         QSqlQuery query;
         query.prepare(QString("select * from songinfo where playlistname=? and id=(select max(id) from songinfo where playlistname=?)"));
         query.addBindValue(listname);
         query.addBindValue(listname);
         query.exec();

      int  index=0;
      while(query.next())//选出最大值
      {
          index= query.value("id").toInt();
          index++;
      }

      query.prepare(QString("INSERT INTO songinfo(playlistname,id,songname,songurl,duration)"
                            "VALUES (:playlistname,:id,:songname,:songurl,:duration)"));
      query.bindValue(":playlistname", listname);
      query.bindValue(":id", index);
      query.bindValue(":songname", songname);
      query.bindValue(":songurl", url);
      query.bindValue(":duration", duration);
      query.exec();

 }

 void myDataBase::deleteSong(const QString&  listname, int index)
 {
     if(index<0)
       return;
     connectSongInfo();
     QSqlQuery query;
    query.exec(QString("delete from songinfo where playlistname= '%1' and id= %2").arg(listname).arg(index));
    query.exec(QString("update songinfo set id = id-1 where playlistname = '%1' and id > %2").arg(listname).arg(index));

 }

 void myDataBase::addPlayList(const QString&  plistname)//时实添加就Okay了
{
    connectListinfo();
    QSqlQuery query;
    query.prepare(QString("INSERT INTO listinfo(listname) VALUES (:listname)"));
    query.bindValue(":listname", plistname);
    query.exec();
}



