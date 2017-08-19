#include "myTablePlayListFinal.h"
#include<QVBoxLayout>
#include<QScrollBar>
#include<QFileDialog>
#include<iterator>
#include<QToolTip>
#include<QMessageBox>
#include<QEventLoop>
#include<QTime>
#include<qthread.h>
#include<QMediaPlayer>
#include<QPainter>
#include<QMimeData>

#include"FFmpegPlayer.h"
#include"middleLeftStackWidget0.h"
#include"myMediaList.h"
#include"mytablewidget.h"
#include"mainwindow.h"
#include"middlewidgetleft.h"

static QColor  BGcolor=QColor(230,230,230);
myTablePlayListFinal * myTablePlayListFinal::s_pCurList=NULL;

myTablePlayListFinal::myTablePlayListFinal(QWidget*parent):
    baseWidget(parent),
    m_PlayListname("新建列表")
{
    m_table.setTableFinal(this);
    m_Btntable.setTableFinal(this);
    m_playList.setTableFinal(this);

    setMouseTracking(true);
    setAcceptDrops(true);

    connect(&m_Btntable,SIGNAL(sig_emptyList()),this,SLOT(slot_emptyList()));//清空列表
    connect(&m_Btntable,SIGNAL(sig_addSong()),this,SLOT(slot_addSong()));//添加歌曲
    connect(&m_table,SIGNAL(sig_RowCountChange()),&m_Btntable,SLOT(slot_updateSongCount()));//歌曲列表改变信息
    connect(&m_table,SIGNAL(sig_RowCountChange()),&m_Btntable,SLOT(slot_updateSongCount()));//歌曲列表改变信息
    connect(&m_Btntable,SIGNAL(clicked(bool)),this,SLOT(slot_showHideTable()));
    connect(&m_table,SIGNAL(sig_delIndex(int)),&m_playList,SLOT(slot_removeSong(int)));//从列表中删除
    connect(&m_table,SIGNAL(sig_addSong()),this,SLOT(slot_addSong()));//添加歌曲
    connect(&m_table,SIGNAL(sig_addSongFolder()),this,SLOT(slot_addSongFolder()));

    QVBoxLayout *vlyout1=new QVBoxLayout;
    setMinimumSize(310,40);
    setMaximumWidth(380);

    vlyout1->addWidget(&m_Btntable);
    vlyout1->addWidget(&m_table);
    vlyout1->setContentsMargins(0,0,0,0);
    vlyout1->setSpacing(0);
    setLayout(vlyout1);
}
void myTablePlayListFinal::addToPlayList(const QString &name,const QString &url,const QString &dur, const QString &strHash)
{
    int rowcount= m_table.rowCount();
    m_table.insertRow(rowcount);
    m_table.setItem(rowcount,0,new QTableWidgetItem(""));
    m_table.setItem(rowcount,1, new QTableWidgetItem(name));
    m_table.setItem(rowcount,2, new QTableWidgetItem(QString(dur+"    ")));
    m_table.item(rowcount,2)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

    m_playList.addPlayList(url,strHash);
    myDataBase::addSong(ShowButtonName(),name,url,dur);
}

void myTablePlayListFinal::wheelEvent(QWheelEvent *e)
{
    int i=e->delta()/5;
    int value=m_midleft0->verticalScrollBar()->value();
    m_midleft0->verticalScrollBar()->setValue(value-i);
}

void myTablePlayListFinal::paintEvent(QPaintEvent *e)
{
    baseWidget::paintEvent(e);
    QPainter p(this);
    p.setPen(BGcolor);
    p.drawLine(0,height()-1,width()-1,height()-1);
}

void myTablePlayListFinal::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void myTablePlayListFinal::dropEvent(QDropEvent *event)
{
    QList<QUrl> files = event->mimeData()->urls();
    QMediaPlayer player;
    QEventLoop lp;
       for(int i=0;i<files.count();i++)
       {
           if(files.value(i).toString().contains(".mp3"))
           {
                   QFileInfo info(files.value(i).toLocalFile());
                   QString m_name=info.completeBaseName();
                   if(!m_playList.GetList().contains(files.value(i).toString()))
                   {
                       QString filePath=files.value(i).toLocalFile();
                       player.setMedia(QUrl(filePath));
////prevent the loop dont stop
                       QTimer timer;
                       connect(&timer,&QTimer::timeout,[&](){
                                   lp.quit();
                               });
                       timer.setSingleShot(true);
                       timer.start(2000);
////
                       connect(&player,SIGNAL(durationChanged(qint64)),&lp,SLOT(quit()));
                       lp.exec();
                       qint64 musicTime= player.duration();
                       QTime total_time(0, (musicTime/60000)%60, (musicTime/1000)%60);
                       QString duration=total_time.toString("mm:ss");


                       addToPlayList(m_name,files.value(i).toLocalFile(),duration);
                       setAutoLayout();
                   }
           }
      }

}


void myTablePlayListFinal::slot_emptyList()//清空列表
{
    int i=0;
    int count=m_table.rowCount();
    int mb= QMessageBox::warning(NULL,"warning","确定要清空？",QMessageBox::Ok|QMessageBox::Cancel);
    if(mb==QMessageBox::Cancel)
        return;

    myDataBase::emptyList(ShowButtonName());

   if(s_pCurList == this)//如果正在播放的
     {
        stopCurrentSong();
     }
    while(i<count)
    {
        int row=m_table.rowCount()-1;
        m_table.slot_cellEnter(-1,0);
        emit m_table.sig_delIndex(row);
        m_table.removeRow(row);
        i++;
    }
    setAutoLayout();
}

void myTablePlayListFinal::setAutoLayout()
{
    m_table.setAutoLayoutSize();
  //  updateConvientButton();
}

void myTablePlayListFinal::updateBGcolor()
{
    BGcolor=middleWidgetLeft::color;
    update();
}

void myTablePlayListFinal::setCurrentSongAlbumPic(const QString &strPath)
{
    m_table.m_playingWid.m_btnLab.setIcon(QPixmap(strPath).scaled(52,52));
}

const QString myTablePlayListFinal::currentSongDuration()
{
    int index= m_table.currentSongIndex();
    QTableWidgetItem *it=m_table.item(index,2);
    if(it!=Q_NULLPTR)
    {
        QString text=it->text();
        return text.isEmpty()? m_table.getHoverDuration().simplified():text.simplified();
    }
    else
        return m_table.getHoverDuration().simplified();
}

int myTablePlayListFinal::currentSongDurationToInt()
{
   QTime time= QTime::fromString( currentSongDuration(),"mm:ss");
    return time.minute()*60*1000+time.second()*1000;
}

void myTablePlayListFinal::stopCurrentSong()
{
    mainWindow::GetInstance()->player()->pause();
    mainWindow::GetInstance()->player()->setMedia(NULL);
    m_table.m_playingWid.setCurrentSongItem(NULL);
    m_midleft0->setOriginalStatus();
}

void myTablePlayListFinal::updateConvientButton()
{
#if 0
    int y= m_Btntable->mapFrom(m_midleft0,QPoint(0,0)).y();
    if(y<=0)
    {
        m_midleft0->convientShowTableBtn()->hide();
    }
    else
    {
        m_midleft0->convientShowTableBtn()->setFinalWidget(this);
        m_midleft0->convientShowTableBtn()->show();

        foreach (myTablePlayListFinal*f, m_midleft0->myTablePlayListFinalVector())
        disconnect(m_midleft0->convientShowTableBtn(),SIGNAL(clicked(bool)),f->m_Btntable,SLOT(click()));
        connect(m_midleft0->convientShowTableBtn(),SIGNAL(clicked(bool)),m_Btntable,SLOT(click()));
    }
#else
    int y=0;
    QVector<myTablePlayListFinal*> &vector=m_midleft0->myTablePlayListFinalVector();
    for(int i=0;i<vector.size();++i)
    {
        myTablePlayListFinal*f=vector.value(i);
        if(f==this)
            break;
        y+=f->height();
    }
    if(m_midleft0->verticalScrollBar()->value()<=y)
    {
         m_midleft0->convientShowTableBtn()->hide();
    }
    else
    {
      //  m_midleft0->convientShowTableBtn()->setFinalWidget(this);
        m_midleft0->convientShowTableBtn()->show();
        if(this== m_midleft0->myTablePlayListFinalVector().first()||this== m_midleft0->myTablePlayListFinalVector().last())
            m_midleft0->convientShowTableBtn()->setEnabledMenuItem(false);
        else
            m_midleft0->convientShowTableBtn()->setEnabledMenuItem(true);
        foreach (myTablePlayListFinal*f, m_midleft0->myTablePlayListFinalVector())
        {
            disconnect(m_midleft0->convientShowTableBtn(),SIGNAL(clicked(bool)),&f->m_Btntable,SLOT(click()));
        }
        connect(m_midleft0->convientShowTableBtn(),SIGNAL(clicked(bool)),&m_Btntable,SLOT(click()));
    }
#endif
}


void myTablePlayListFinal::slot_showHideTable()
{
  //  updateConvientButton();
    if(!m_table.isHidden())
    {
        m_table.hide();
        foreach(myTablePlayListFinal *f,m_midleft0->myTablePlayListFinalVector())
                f->m_table.hide();
        m_midleft0->convientShowTableBtn()->hide();

    }
    else
    {
       m_table.show();
           foreach(myTablePlayListFinal *f,m_midleft0->myTablePlayListFinalVector())
           {
               if(f!=this)//如果显示的话
               {
                   f->m_table.hide();
               }
           }
    }

}

void myTablePlayListFinal::getlistfromDateBase()//从数据库中获取列表
{
   QVector<QStringList> vec= myDataBase::readListSongInfo(ShowButtonName());
   QStringList listname=vec.at(0);
   QStringList listurl=vec.at(1);
   QStringList listduration=vec.at(2);
   for(int i=0;i<listname.count();i++)
   {
        m_table.insertRow(i);
        m_table.setItem(i,0,new QTableWidgetItem(""));
        m_table.setItem(i,1,new QTableWidgetItem(listname.value(i)));
        m_table.setItem(i,2,new QTableWidgetItem(listduration.value(i)+"    "));
        m_table.item(i,2)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

        m_playList.addPlayList(listurl.value(i));
   }
   m_Btntable. slot_updateSongCount();
}

void myTablePlayListFinal::slot_addSong()
{
    static QString  initialName ="F:/MP3s764东西/MUSIC";
    QStringList files=QFileDialog::getOpenFileNames(this, tr("选择文件"), initialName, tr("*.mp3"));
    if(files.isEmpty())
        return;

    QMediaPlayer player;
    QEventLoop lp;
       for(int i=0;i<files.count();i++)
       {

                   QFileInfo info(files[i]);
                   QString m_name=info.completeBaseName();
                   if(!m_playList.GetList().contains(files.value(i)))
                   {
                       QString filePath=files.value(i);
                       player.setMedia(QUrl(filePath));
////prevent the loop dont stop
                       QTimer timer;
                       connect(&timer,&QTimer::timeout,[&](){
                                   lp.quit();
                               });
                       timer.setSingleShot(true);
                       timer.start(2000);
////
                       connect(&player,SIGNAL(durationChanged(qint64)),&lp,SLOT(quit()));
                       lp.exec();
                       qint64 musicTime= player.duration();
                       QTime total_time(0, (musicTime/60000)%60, (musicTime/1000)%60);
                       QString duration=total_time.toString("mm:ss");


                       addToPlayList(m_name,files.at(i),duration);
                       setAutoLayout();
                   }
        }
}
void myTablePlayListFinal::slot_addSongFolder()
{

    QString dirrr = QFileDialog::getExistingDirectory(this, tr("打开本地歌曲文件夹"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    QDir dir(dirrr);
        if(!dir.exists())
        {
            return;
        }
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();
        int file_count = list.count();

        if(file_count <= 0)
        {
            return;
        }

        QStringList files;
        for(int i=0; i<file_count;i++)
        {
            QFileInfo file_info = list.at(i);
            QString suffix = file_info.suffix();
            if(QString::compare(suffix, QString("mp3"), Qt::CaseInsensitive) == 0)
            {
                QString absolute_file_path = file_info.absoluteFilePath();
                files.append(absolute_file_path);
            }
        }

        if(files.isEmpty())
            return;

        QMediaPlayer player;
        QEventLoop lp;
           for(int i=0;i<files.count();i++)
           {

                       QFileInfo info(files[i]);
                       QString m_name=info.completeBaseName();
                       if(!m_playList.GetList().contains(files.value(i)))
                       {
                           QString filePath=files.value(i);
                           player.setMedia(QUrl(filePath));

                           connect(&player,SIGNAL(durationChanged(qint64)),&lp,SLOT(quit()));
                           lp.exec();
                           qint64 musicTime= player.duration();
                           QTime total_time(0, (musicTime/60000)%60, (musicTime/1000)%60);
                           QString format = "mm:ss";

                           QString duration=total_time.toString(format);
                           addToPlayList(m_name,files.at(i),duration);
                           setAutoLayout();
                       }
            }
}
/*void myTablePlayListFinal::slot_playSongFromSearchTable(const QStringList &namelist,const QStringList &urllist,const QStringList &dur)
{
    for(int i=0;i<namelist.count();i++)
    {
                QString m_name=namelist.value(i);
                if(!m_playList.GetList().contains(urllist.value(i)))
                {
                    addToPlayList(m_name,urllist.value(i),dur.value(i));
                }
     }

    if( m_midleft0->myTablePlayListFinalVector().at(0)->m_table.isHidden())//如果第一列表隐藏
        m_midleft0->myTablePlayListFinalVector().at(0)->m_Btntable.clicked();
    else
        setAutoLayout();

    QString songurl= urllist.value(0);
    int index= songUrlList().indexOf(QUrl(songurl));
    m_table.slot_doublick(index,0);

    m_midleft0->scrolltoCurrentPlayList();
}*/

void myTablePlayListFinal::slot_addSongFromSearchTable(const QStringList &namelist,const QStringList& urllist,const QStringList &dur)
{
    for(int i=0;i<namelist.count();i++)
    {
                QString m_name=namelist.value(i);
                if(!m_playList.GetList().contains(urllist.value(i)))
                {
                    addToPlayList(m_name,urllist.value(i),dur.value(i));
                }
     }
   if(m_table.isHidden())//如果第一列表隐藏
      m_Btntable.clicked();
   else
       setAutoLayout();
}
