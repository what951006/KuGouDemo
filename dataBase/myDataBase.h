#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>


namespace myDataBase {
    void connectListinfo();
    void connectSongInfo();
   /*insert
   */
    void addSong(const QString& listname,const QString&  songname,const QString&  url,const QString&  duration); //添加歌曲
    void addPlayList(const QString&  plistName);         //添加列表
   /*delete
   */
    void deleteSong(const QString&  listname,int index);  //删除歌曲
    void emptyList(const QString&  listname); //清空列表
    void removePlayList(const QString& );//移除table
    /*update
    */
    void renameList(const QString&  oldname,const QString&  newname);  //重命名
    void swapList(const QString&,int index,const QString &,int index2);
    void takeAndInsert(const QString&from,const QString &to);
   /*select
   */
    QVector<QStringList> readListSongInfo(const QString&  listinfo);   //获取歌曲
    QStringList loadPlayList();// 加载列表
}

#endif // MYDATABASE_H
