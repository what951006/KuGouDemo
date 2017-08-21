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
#include"myMediaList.h"
#include"middleconvienttwobutton.h"
#include"middleWidgets.h"
#include"FFmpegPlayer.h"

middleLeftStackWidget0::middleLeftStackWidget0(QWidget*parent)
    :myScrollArea(parent)
    ,m_wid(this)
    ,m_convientSTBtn(this)
    ,m_searchwid(this)
    ,m_convtwowid(this)
    ,m_addTips(this)
    ,m_defaultList(this)
    ,m_lovedList(this)
{
    setMinimumWidth(310);
    setMaximumWidth(380);

    init();
    initAddTips();
    initConvientWidget();
    initConnection();
}
void middleLeftStackWidget0::slot_setlabelpic(const QString&strpath,const QString &strName)
{
    myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    if(!pPlayList)
        return;
    if(strName == pPlayList->currentMusicName())
    {
        pPlayList->setCurrentSongAlbumPic(strpath);
    }
}

void middleLeftStackWidget0::initAddTips()
{
    m_addTips.hide();
}

void middleLeftStackWidget0::initConvientWidget()
{
    m_convtwowid.hide();
    connect(&m_convtwowid.m_btnlocate,SIGNAL(clicked(bool)),this,SLOT(scrolltoCurrentPlayList()));
    m_searchwid.hide();
    connect(&m_convtwowid.m_btnsearch,SIGNAL(clicked(bool)),&m_searchwid,SLOT(show()));
    connect(&m_searchwid.m_lineEdit,SIGNAL(textChanged(QString)),this,SLOT(slot_localSearch(QString)));
    connect(&m_convientSTBtn,SIGNAL(clicked(bool)),&m_convientSTBtn,SLOT(hide()));
    m_convientSTBtn.setTipsStyle(true);
    m_convientSTBtn.hide();
}

void middleLeftStackWidget0::initConnection()
{
    connect(this->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(slot_verScrBarChange(int)));
}
void middleLeftStackWidget0::scrolltoCurrentPlayList()
{
    myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    if(!pPlayList)
        return;
   int pindex= m_vlyout.indexOf(pPlayList);

    if(!pPlayList->m_table.isHidden()&&pPlayList->m_table.rowCount()!=0)//if it is visiable
    {
        int height=0;
        for(int i=0;i<pindex;i++)
        {
            height+=m_Vector.value(i)->height();
        }
        int index=pPlayList->m_table.currentSongIndex();
        this->verticalScrollBar()->setValue(height+index*32);

    }
    else if(pPlayList->m_table.isHidden()&&pPlayList->m_table.rowCount()!=0)
    {
        pPlayList->m_Btntable.clicked();
        int index=pPlayList->m_table.currentSongIndex();
        this->verticalScrollBar()->setValue(index*32);
    }
}


void middleLeftStackWidget0::slot_endOfMedia()//下一曲
{
     myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    if(!pPlayList)
        return;

    int index= pPlayList->mediaList()->nextMediaIndex();
    pPlayList->m_table.slot_doublick(index,0);
}
void middleLeftStackWidget0::slot_playIndex(int index)//设置播放的index所有的歌曲都是通过这个方法来播放的
{
    myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    QUrl url= pPlayList->mediaList()->mediaUrl(index);
    if(!url.isEmpty())
    {
        mainWindow::GetInstance()->player()->setMedia(url.toString());
        mainWindow::GetInstance()->player()->play();
    }
}

void middleLeftStackWidget0::init()
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    setWidget(&m_wid);
    QGridLayout *glyout=new QGridLayout;
    glyout->addWidget(&m_wid);
    glyout->setContentsMargins(0,0,0,0);
    setLayout(glyout);

    m_defaultList.setMiddleStackWidget0(this);
    m_defaultList.setShowButtonName("默认列表");
    m_defaultList.getlistfromDateBase();//添加歌曲
    m_defaultList.m_table.show();
    m_defaultList.m_Btntable.setEnabledMenuItem();

    m_lovedList.setMiddleStackWidget0(this);
    m_lovedList.setShowButtonName("我的最爱");
    m_lovedList.getlistfromDateBase();
    m_lovedList.m_table.hide();
    m_lovedList.m_Btntable.setEnabledMenuItem();

    m_Vector<<&m_defaultList<<&m_lovedList;
    m_vlyout.addWidget(&m_defaultList);
    m_vlyout.addWidget(&m_lovedList);


    m_vlyout.addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding,QSizePolicy::Expanding));
    m_vlyout.setContentsMargins(0,0,0,0);
    m_vlyout.setSpacing(0);
    m_wid.setLayout(&m_vlyout);

    QStringList list=myDataBase::loadPlayList();
    for(int i=0;i<list.count();i++)//加载播放列表
    {
        addPlayList(list.value(i));
    }
    setAutoLayout();
    setOriginalStatus();
}

void middleLeftStackWidget0::setAutoLayout()
{
  int height=0;
  foreach(myTablePlayListFinal* p,m_Vector)
  {
      height+= p->height();
  }
  m_wid.setMinimumHeight(height);
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

    table->m_table.hide();
    m_vlyout.insertWidget(m_vlyout.count()-2,table);
    m_Vector.insert(m_Vector.count()-1,table);
    table->m_Btntable.click();
    if(mainWindow::GetInstance())
        mainWindow::GetInstance()->UpdateListConn();
    setAutoLayout();
    table->m_Btntable.slot_ReName();
}

void middleLeftStackWidget0::addPlayList(const QString &plname)//添加一个列表
{
    myTablePlayListFinal *m_table=new myTablePlayListFinal(this);
    m_table->setMiddleStackWidget0(this);//pass the pointer
    m_table->m_table.hide();
    m_vlyout.insertWidget(m_vlyout.count()-2,m_table);
    m_Vector.insert(m_Vector.count()-1,m_table);
    m_table->setShowButtonName(plname);
    m_table->getlistfromDateBase(); //根据目标名 添加歌曲
    m_table->updateCount();//更新一下
    setAutoLayout();
}

void middleLeftStackWidget0::slot_removePlayList()//移除播放列表
{
   myTablePlayListFinal* f= (myTablePlayListFinal*)sender()->parent();
   m_vlyout.removeWidget(f);
   if(f == myTablePlayListFinal::getCurrentList())//如果遇到正在播放
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
    myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    if(!pPlayList)
        return;
    int index= pPlayList->mediaList()->nextMediaIndex();
    pPlayList->m_table.slot_doublick(index,0);
}

void middleLeftStackWidget0::slot_btnpreSong()//上一曲
{
    myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    if(!pPlayList)
        return;
    int index= pPlayList->mediaList()->preMediaIndex();
    pPlayList->m_table.slot_doublick(index,0);
}

void middleLeftStackWidget0::resizeEvent(QResizeEvent *e)
{
    myScrollArea::resizeEvent(e);
    m_addTips.setGeometry(0,height()-m_addTips.height(),width(),m_addTips.height());
    m_convtwowid.setGeometry(width()*7/10,height()*8/10,m_convtwowid.width(),m_convtwowid.height());
    m_searchwid.setGeometry(0,height()-m_searchwid.height(),width(),m_searchwid.height());
    m_convientSTBtn.setGeometry(0,0,width(),40);
}

void middleLeftStackWidget0::setOriginalStatus()
{
    myTablePlayListFinal::setCurrentList(NULL);
    foreach (myTablePlayListFinal*f, m_Vector)
    {
        f->m_table.m_playingWid.setCurrentSongItem(NULL);
    }
}

void middleLeftStackWidget0::updateBGColor()
{
    m_Vector.value(0)->updateBGcolor();
}

bool middleLeftStackWidget0::isEnableMoveList(myTablePlayListFinal *list)
{
    if(list == m_Vector.first()||list==m_Vector.last())
        return 0;
    return 1;
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
    int loindex= m_vlyout.indexOf(begin);
    m_vlyout.removeWidget(begin);
    int loindex2=m_vlyout.indexOf(after);
    m_vlyout.removeWidget(after);

    m_vlyout.insertWidget(loindex2,begin);
    m_vlyout.insertWidget(loindex,after);
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
    m_vlyout.removeWidget(from);
    int afterindex2=m_vlyout.indexOf(after);
    m_vlyout.insertWidget(afterindex2+1,from);
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
    myTablePlayListFinal*pPlayList= myTablePlayListFinal::getCurrentList();
    if(!pPlayList)
        return;
    pPlayList->mediaList()->setCurIndex(index); //用于MV播放完了~知道自己的位置
}

void middleLeftStackWidget0::slot_showMvWidget(const QString & url)
{
    mainWindow::GetInstance()->player()->setMedia(url,true);
}

void middleLeftStackWidget0::slot_localSearch(const QString &words)
{

    foreach(myTablePlayListFinal*f,m_Vector)
    {
        int row= f->m_table.currentSongIndex();
        int count=f->m_table.rowCount();
        for(int i=0;i<count;i++)
        {
            f->m_table.setRowHeight(i,0);
            if(f->m_table.item(i,1)->text().contains(words))
            {
                f->m_table.show();
                if(row!=i)
                   f->m_table.setRowHeight(i,32);
                else
                     f->m_table.setRowHeight(i,52);
            }
            if(words=="")
            {
                 if(row!=i)
                    f->m_table.setRowHeight(i,32);
                 else
                    f->m_table.setRowHeight(i,52);
            }
             f->m_table.updatePlayingWidgetPos(); //更新一下位置
        }
        f->setAutoLayout();
    }
}
void middleLeftStackWidget0::slot_verScrBarChange(int value)
{
    if(value!=0)
        m_convtwowid.show();
    myTablePlayListFinal*final=NULL;
    foreach (myTablePlayListFinal*f,m_Vector) {
       if(!f->m_table.isHidden())
        final=f;
    }
}
void middleLeftStackWidget0::addMusicToDefaultList(const ItemResult &result, bool bPlay)
{
    if(bPlay)
    {
        if(!m_defaultList.mediaList()->GetList().contains(result.strUrl))
        {
            m_defaultList.addToPlayList(result.strFullName,result.strUrl,result.strDur,result.strHash);
        }

        if( m_defaultList.m_table.isHidden())//如果第一列表隐藏
            m_defaultList.m_Btntable.clicked();
        else
            m_defaultList.setAutoLayout();


        int index= m_defaultList.getIndexByUrl(result.strUrl);
        m_defaultList.m_table.slot_doublick(index,0);
        scrolltoCurrentPlayList();
    }
    else
    {
        if(!m_defaultList.isContainUrl(result.strUrl))
        {
            m_defaultList.addToPlayList(result.strFullName,result.strUrl,result.strDur);
        }

       if(m_defaultList.m_table.isHidden())//如果第一列表隐藏
          m_defaultList.m_Btntable.clicked();
       else
          m_defaultList.setAutoLayout();
    }
}

void middleLeftStackWidget0::addMusicToList(const ItemResult&result, myTablePlayListFinal *finalTable)
{
    if(!finalTable->isContainUrl(result.strUrl))
    {
        finalTable->addToPlayList(result.strFullName,result.strUrl,result.strDur);
    }
   if(finalTable->m_table.isHidden())//如果第一列表隐藏
      finalTable->m_Btntable.clicked();
   else
      finalTable->setAutoLayout();
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
