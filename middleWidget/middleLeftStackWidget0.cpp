#include "middleLeftStackWidget0.h"
#include<QLabel>
#include<QScrollBar>
#include<QVBoxLayout>
#include<QDebug>
#include<QDrag>
#include<QDropEvent>
#include<QMimeData>

#include"mainwindow.h"
#include"bottomWidgets.h"
#include"trayiconmenu.h"
#include"mynetwork.h"
#include"mainWindowContentWidget.h"
#include"myTablePlayListFinal.h"
#include"mytablewidget.h"
#include"middleWidgetRight.h"
#include"myDataBase.h"
#include"playingWidget.h"
#include"lyriclabel.h"
#include"myMediaList.h"
#include"middleconvienttwobutton.h"
#include"middleWidgets.h"
#include"FFmpegPlayer.h"
#include"Global_ValueGather.h"

middleLeftStackWidget0::middleLeftStackWidget0(QWidget*parent):myScrollArea(parent)
{
    midstack0Pointer=this;
    setMinimumWidth(310);
    setMaximumWidth(380);

    init();
    initAddTips();
    initConvientWidget();
    initConnection();
}
void middleLeftStackWidget0::slot_setlabelpic(QByteArray byt,QString songname)
{
    if(!m_nowplayfinaltable)
        return;
    QString playsongname=m_nowplayfinaltable->currentSongName();
    if(songname==playsongname.replace("&"," "))
    {
        QPixmap pix;
        pix.loadFromData(byt);
        m_nowplayfinaltable->setCurrentSongAlbumPic(pix);
    }
}

void middleLeftStackWidget0::initAddTips()
{
     m_addTips=new AddLoveListTips(this);
     m_addTips->hide();
}

void middleLeftStackWidget0::initConvientWidget()
{
    m_convtwowid=new middleConvientTwoButton(this);
    m_convtwowid->hide();
    connect(m_convtwowid->m_btnlocate,SIGNAL(clicked(bool)),this,SLOT(scrolltoCurrentPlayList()));

    m_searchwid=new middleListSearch(this);
    m_searchwid->hide();
    connect(m_convtwowid->m_btnsearch,SIGNAL(clicked(bool)),m_searchwid,SLOT(show()));
    connect(m_searchwid->m_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(slot_searchSong(QString)));

    m_convientSTBtn=new myShowTableButton(this);
    connect(m_convientSTBtn,SIGNAL(clicked(bool)),m_convientSTBtn,SLOT(hide()));
    m_convientSTBtn->setTipsStyle(true);
    m_convientSTBtn->hide();
}

void middleLeftStackWidget0::initConnection()
{
    connect(this->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(slot_verScrBarChange(int)));
}
void middleLeftStackWidget0::scrolltoCurrentPlayList()
{
    if(m_nowplayfinaltable==NULL)
        return;
   int pindex= vlyout1->indexOf(m_nowplayfinaltable);

    if(!m_nowplayfinaltable->m_table->isHidden()&&m_nowplayfinaltable->m_table->rowCount()!=0)//if it is visiable
    {
        int height=0;
        for(int i=0;i<pindex;i++)
        {
            height+=m_Vector.value(i)->height();
        }
        int index=m_nowplayfinaltable->m_table->currentSongIndex();
        this->verticalScrollBar()->setValue(height+index*32);

    }
    else if(m_nowplayfinaltable->m_table->isHidden()&&m_nowplayfinaltable->m_table->rowCount()!=0)
    {
        m_nowplayfinaltable->m_Btntable->clicked();
        int index=m_nowplayfinaltable->m_table->currentSongIndex();
        this->verticalScrollBar()->setValue(index*32);
    }
}

void middleLeftStackWidget0::showAddtips()
{
    m_addTips->showAddTips();
}

void middleLeftStackWidget0::showRemovetips()
{
    m_addTips->showRemoveTips();
}

void middleLeftStackWidget0::setInitMainWindow(mainWindow *p)
{
    m_mainWindow=p;
    setPlayListConnection();
}

void middleLeftStackWidget0::slot_endOfMedia()//下一曲
{
    if(!m_nowplayfinaltable)
        return;

        int index= m_nowplayfinaltable->playList()->nextMediaIndex();
        m_nowplayfinaltable->m_table->slot_doublick(index,0);
}
void middleLeftStackWidget0::slot_playIndex(int index)//设置播放的index所有的歌曲都是通过这个方法来播放的
{
    m_nowplayfinaltable=(myTablePlayListFinal*)sender()->parent();
    QUrl url= m_nowplayfinaltable->playList()->mediaUrl(index);
    if(!url.isEmpty())
    {
        m_mainWindow->player()->setMedia(url.toString());
        m_mainWindow->player()->play();
    }
}

void middleLeftStackWidget0::init()
{
    m_nowplayfinaltable=NULL;

    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_wid=new baseWidget(this);
    setWidget(m_wid);
    QGridLayout *glyout=new QGridLayout;
    glyout->addWidget(m_wid);
    glyout->setContentsMargins(0,0,0,0);
    setLayout(glyout);

    vlyout1=new QVBoxLayout;
    myTablePlayListFinal *table0=new myTablePlayListFinal(this);
    table0->setMiddleStackWidget0(this);
    table0->setShowButtonName("默认列表");
    table0->getlistfromDateBase();//添加歌曲
    table0->m_table->show();
    table0->m_Btntable->setEnabledMenuItem();

    myTablePlayListFinal *table1=new myTablePlayListFinal(this);
    table1->setMiddleStackWidget0(this);
    table1->setShowButtonName("我的最爱");
    table1->getlistfromDateBase();
    table1->m_table->hide();
    table1->m_Btntable->setEnabledMenuItem();

    m_Vector<<table0<<table1;
    vlyout1->addWidget(table0);
    vlyout1->addWidget(table1);


    vlyout1->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
    vlyout1->setContentsMargins(0,0,0,0);
    vlyout1->setSpacing(0);
    m_wid->setLayout(vlyout1);

    QStringList list=myDataBase::loadPlayList();
    for(int i=0;i<list.count();i++)//加载播放列表
    {
        addPlayList(list.value(i));
    }

    myDataBase::connectListinfo();
    myDataBase::connectSongInfo();
    setAutoLayout();

//    setOriginalStatus();
}

void middleLeftStackWidget0::setAutoLayout()
{
  int height=0;
  foreach(myTablePlayListFinal* p,m_Vector)
  {
      height+= p->height();
  }
  m_wid->setMinimumHeight(height);
}
void middleLeftStackWidget0::slot_addPlayListWithRename()//新建一个表
{
    myTablePlayListFinal *table=new myTablePlayListFinal(this);
    table->setMiddleStackWidget0(this);

    QStringList strlist;
    for(int i=0;i<m_Vector.count();i++)
    {
        strlist<<m_Vector.at(i)->ShowButtonName();
    }
    for(int i=1;i<=m_Vector.count();i++)
    {
        if(!strlist.contains(QString("新建列表%1").arg(i)))//如果包含
        {
            table->setShowButtonName(QString("新建列表%1").arg(i));
            break;
        }
    }
    myDataBase::addPlayList(table->ShowButtonName());  //db

    table->m_table->hide();
    vlyout1->insertWidget(vlyout1->count()-2,table);
    m_Vector.insert(m_Vector.count()-1,table);
    table->m_Btntable->click();
    setPlayListConnection();
    setAutoLayout();
    table->m_Btntable->slot_ReName();
}

void middleLeftStackWidget0::addPlayList(const QString &plname)//添加一个列表
{
    m_table=new myTablePlayListFinal(this);
    m_table->setMiddleStackWidget0(this);//pass the pointer
    m_table->m_table->hide();
    vlyout1->insertWidget(vlyout1->count()-2,m_table);
    m_Vector.insert(m_Vector.count()-1,m_table);
    m_table->setShowButtonName(plname);
    m_table->getlistfromDateBase(); //根据目标名 添加歌曲
    m_table->updateCount();//更新一下
    setAutoLayout();
}

void middleLeftStackWidget0::slot_removePlayList()//移除播放列表
{
   myTablePlayListFinal* f= (myTablePlayListFinal*)sender()->parent();
   vlyout1->removeWidget(f);
   if(f==m_nowplayfinaltable)//如果遇到正在播放
   {
       f->stopCurrentSong();
   }
   myDataBase::removePlayList(f->ShowButtonName());//dbremoved
   m_Vector.removeOne(f);//从这里vector移出
   f->deleteLater();
   setAutoLayout();
}

void middleLeftStackWidget0::slot_btnnextSong()//下一曲
{
    if(m_nowplayfinaltable==NULL)
        return;
    int index= m_nowplayfinaltable->playList()->nextMediaIndex();
    m_nowplayfinaltable->m_table->slot_doublick(index,0);
}

void middleLeftStackWidget0::slot_btnpreSong()//上一曲
{
    if(m_nowplayfinaltable==NULL)
        return;
    int index= m_nowplayfinaltable->playList()->preMediaIndex();
    m_nowplayfinaltable->m_table->slot_doublick(index,0);
}

void middleLeftStackWidget0::resizeEvent(QResizeEvent *e)
{
    myScrollArea::resizeEvent(e);
    m_addTips->setGeometry(0,height()-m_addTips->height(),width(),m_addTips->height());
    m_convtwowid->setGeometry(width()*7/10,height()*8/10,m_convtwowid->width(),m_convtwowid->height());
    m_searchwid->setGeometry(0,height()-m_searchwid->height(),width(),m_searchwid->height());
    m_convientSTBtn->setGeometry(0,0,width(),40);
}

void middleLeftStackWidget0::setOriginalStatus()
{
    m_nowplayfinaltable=NULL;
    foreach (myTablePlayListFinal*f, m_Vector) {
          f->m_table->m_playingWid->setCurrentSongItem(NULL);
    }
    m_mainWindow->setOriginalStatus();
}

void middleLeftStackWidget0::updateBGColor()
{
    m_Vector.value(0)->updateBGcolor();
}

bool middleLeftStackWidget0::isEnableMoveList(myTablePlayListFinal *list)
{
    if(list==m_Vector.first()||list==m_Vector.last())
        return 0;
    return 1;
}

void middleLeftStackWidget0::setPlayListConnection()
{
    for(int i=0;i<m_Vector.count();i++)
    {
        disconnect(m_Vector.at(i)->m_Btntable,SIGNAL(sig_addPlayList()),this,SLOT(slot_addPlayListWithRename()));//添加播放列表
        connect(m_Vector.at(i)->m_Btntable,SIGNAL(sig_addPlayList()),this,SLOT(slot_addPlayListWithRename()));

        disconnect(m_Vector.at(i)->m_Btntable,SIGNAL(sig_delPlayList()),this,SLOT(slot_removePlayList()));//移除播放列表
        connect(m_Vector.at(i)->m_Btntable,SIGNAL(sig_delPlayList()),this,SLOT(slot_removePlayList()));

        disconnect(m_Vector.at(i)->m_table,SIGNAL(sig_play(int)),this,SLOT(slot_playIndex(int)));//播放triggered
        connect(m_Vector.at(i)->m_table,SIGNAL(sig_play(int)),this,SLOT(slot_playIndex(int)));

        disconnect(m_Vector.at(i)->m_table,SIGNAL(sig_playMv(int)),this,SLOT(slot_playMVIndex(int)));//播放MV!!
        connect(m_Vector.at(i)->m_table,SIGNAL(sig_playMv(int)),this,SLOT(slot_playMVIndex(int)));

       disconnect(m_Vector.at(i)->m_table->m_playingWid,SIGNAL(sig_clicked(QString)),m_mainWindow->middleWidget()->m_rightWid,SLOT(slot_search(QString)));//clickand 搜索
       connect(m_Vector.at(i)->m_table->m_playingWid,SIGNAL(sig_clicked(QString)),m_mainWindow->middleWidget()->m_rightWid,SLOT(slot_search(QString)));//搜索

       disconnect(m_Vector.at(i)->m_table->m_playingWid->m_btnLab,SIGNAL(clicked(bool)),m_mainWindow->middleWidget()->m_rightWid,SLOT(slot_setLrcShowStack()));
       connect(m_Vector.at(i)->m_table->m_playingWid->m_btnLab,SIGNAL(clicked(bool)),m_mainWindow->middleWidget()->m_rightWid,SLOT(slot_setLrcShowStack()));

       disconnect(m_Vector.at(i)->m_table,SIGNAL(sig_requestMv(QString)),m_mainWindow,SIGNAL(sig_requestMv(QString)));
       connect(m_Vector.at(i)->m_table,SIGNAL(sig_requestMv(QString)),m_mainWindow,SIGNAL(sig_requestMv(QString)));
    }
}

void middleLeftStackWidget0::setSwapList( myTablePlayListFinal *begin,  myTablePlayListFinal *after)
{
   if(!begin||!after||begin==after)
       return;
    int beginindex=m_Vector.indexOf(begin);
    int afterindex=m_Vector.indexOf(after);

    m_Vector.replace(afterindex,begin);
    m_Vector.replace(beginindex,after);
/**/
    int loindex= vlyout1->indexOf(begin);
    vlyout1->removeWidget(begin);
    int loindex2=vlyout1->indexOf(after);
    vlyout1->removeWidget(after);

    vlyout1->insertWidget(loindex2,begin);
    vlyout1->insertWidget(loindex,after);
/**/
    myDataBase::swapList(begin->ShowButtonName(),beginindex,after->ShowButtonName(),afterindex);
}

void middleLeftStackWidget0::setListTakeAndInsert(myTablePlayListFinal *from, myTablePlayListFinal *after)
{
    if(!from||!after||from==after)
        return;

    m_Vector.removeOne(from);
    int afterindex= m_Vector.indexOf(after);
    m_Vector.insert(afterindex+1,from);
    /*
     * vector was successfully to be inserted
    */
    vlyout1->removeWidget(from);
    int afterindex2=vlyout1->indexOf(after);
    vlyout1->insertWidget(afterindex2+1,from);
    /*
     * Layout was successfully to be changed
    */
      myDataBase::takeAndInsert(from->ShowButtonName(),after->ShowButtonName());
    /*
     * DateBase was  successfully to be updated
    */
}

void middleLeftStackWidget0::slot_playMVIndex(int index)
{
    m_nowplayfinaltable=(myTablePlayListFinal*)sender()->parent();
    m_nowplayfinaltable->playList()->setCurIndex(index); //用于MV播放完了~知道自己的位置
}

void middleLeftStackWidget0::slot_showMvWidget(const QString & url)
{
    m_mainWindow->player()->setMedia(url,true);
}

void middleLeftStackWidget0::slot_searchSong(const QString &words)
{

    foreach(myTablePlayListFinal*f,m_Vector)
    {
        int row= f->m_table->currentSongIndex();
        int count=f->m_table->rowCount();
        for(int i=0;i<count;i++)
        {
            f->m_table->setRowHeight(i,0);
            if(f->m_table->item(i,1)->text().contains(words))
            {
                f->m_table->show();
                if(row!=i)
                   f->m_table->setRowHeight(i,32);
                else
                     f->m_table->setRowHeight(i,52);
            }
            if(words=="")
            {
                 if(row!=i)
                    f->m_table->setRowHeight(i,32);
                 else
                    f->m_table->setRowHeight(i,52);
            }
             f->m_table->updatePlayingWidgetPos(); //更新一下位置
        }
        f->setAutoLayout();
    }
}
void middleLeftStackWidget0::slot_verScrBarChange(int value)
{
    if(value!=0)
        m_convtwowid->show();
    myTablePlayListFinal*final=NULL;
    foreach (myTablePlayListFinal*f,m_Vector) {
       if(!f->m_table->isHidden())
        final=f;
    }
   /* if(final)
    final->updateConvientButton();*/

}
/////////////////////////////////////////////////////////////////////////////////////
myScrollArea::myScrollArea(QWidget *parent):QScrollArea(parent)
{

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setFrameShadow(QFrame::Plain);//设置平的样式
    setFrameShape(QFrame::NoFrame);//设置无边样式

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width:10px; padding-right: 2px;}"
                                      "QScrollBar::handle{background:rgb(180,180,180,150);border-radius:4px;}"
                                      "QScrollBar::handle:hover{background: rgb(150,150,150,150);}"
                                      "QScrollBar::add-line:vertical{border:1px rgb(230,230,230,150);height: 1px;}"
                                      "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230,150);height: 1px;}"
                                      "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");


//////////////////////////////////////////////用于最外边


}
