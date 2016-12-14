#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>


class myDataBase
{
public:
    explicit myDataBase(){}

   static void connectListinfo();
   static void connectSongInfo();
   /*insert
   */
   static void addSong(const QString& listname,const QString&  songname,const QString&  url,const QString&  duration); //添加歌曲
   static void addPlayList(const QString&  plistName);         //添加列表
   /*delete
   */
   static void deleteSong(const QString&  listname,int index);  //删除歌曲
   static void emptyList(const QString&  listname); //清空列表
   static void removePlayList(const QString& );//移除table
    /*update
    */
   static void renameList(const QString&  oldname,const QString&  newname);  //重命名
   static void swapList(const QString&,int index,const QString &,int index2);
   static void takeAndInsert(const QString&from,const QString &to);
   /*select
   */
   static QVector<QStringList> readListSongInfo(const QString&  listinfo);   //获取歌曲
   static QStringList loadPlayList();// 加载列表

private:

};

#endif // MYDATABASE_H
