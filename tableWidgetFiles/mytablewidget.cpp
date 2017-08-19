#include "mytablewidget.h"
#include<QDebug>
#include<QHeaderView>
#include<QScrollBar>
#include<qgridlayout.h>
#include<QMessageBox>
#include<QVBoxLayout>
#include <QApplication>

#include"myTablePlayListFinal.h"
#include"middleLeftStackWidget0.h"
#include"myMediaList.h"
#include"middleconvienttwobutton.h"


myTableWidget::myTableWidget(QWidget *parent):
    QTableWidget(parent),
    m_playingWid(this)
{
    init();
    initPlayingWidget();
    initMenu();
    connect(this,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slot_doublick(int,int)));
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_menuRequest(QPoint)));
    connect(this,SIGNAL(cellClicked(int,int)),this,SLOT(slot_cellClicked(int,int)));
    connect(this,SIGNAL(sig_RowCountChange()),this,SLOT(slot_rowCountChanged()));
}

void myTableWidget::slot_btnloveclicked()
{
    myTableWidget *table= (myTableWidget*)(&m_middleftStack0->myTablePlayListFinalVector().last()->m_table);

    if(!m_groupWid->isLoved())//其余列表中存喜爱的 则删除它
    {
          table->removeSong(m_loveNowRow,false);
          m_middleftStack0->showRemovetips(); //show the tips that remove successfully
       }
       else//则添加它 到喜爱的列表
       {
           QString songname=item(m_prebgItem,1)->text();
           QString url=m_finalWidget->getUrlByIndex(m_prebgItem);
           myTablePlayListFinal*plove=m_middleftStack0->myTablePlayListFinalVector().last(); //第二个列表中

           plove->addToPlayList(songname,url,m_text.simplified());
           m_middleftStack0->showAddtips(); //show the tips  that add successfully
       }
}


void myTableWidget::slot_playingWidgetDelBtnClicked()
{
    int playIndex= currentSongIndex();
    removeSong(playIndex);
}


void myTableWidget::slot_removeHoverRow()
{
    removeSong(m_prebgItem);
}
void myTableWidget::initMenu()
{
    m_menu=new QMenu;
    m_Addtoplistmenu=new QMenu("添加到列表",m_menu);//submenu

    QAction*act_del=new QAction("删除",m_menu);
    QAction*act_play=new QAction("播放",m_menu);
    QAction*act_addsong=new QAction("添加歌曲",m_menu);

    m_menu->addAction(act_play);
    m_menu->addAction(new QAction("稍后播",m_menu));
    m_menu->addAction(new QAction("添加到播放列表          ",m_menu));
    m_menu->addAction(new QAction("下载更多音质",m_menu));
    m_menu->addSeparator();
    m_menu->addAction(act_addsong);
    m_menu->addAction(new QAction("排序",m_menu));
    m_menu->addAction(new QAction("播放模式",m_menu));
    m_menu->addAction(new QAction("匹配MV",m_menu));
    m_menu->addSeparator();
    m_menu->addMenu(m_Addtoplistmenu);
    m_menu->addAction(new QAction("移动到",m_menu));
    m_menu->addAction(new QAction("相似歌曲",m_menu));
    m_menu->addAction(new QAction("分享到",m_menu));
    m_menu->addAction(new QAction("K歌",m_menu));
    m_menu->addAction(new QAction("传歌到移动设备",m_menu));
    m_menu->addAction(new QAction("传铃声到移动设备",m_menu));
    m_menu->addAction(new QAction("工具",m_menu));
    m_menu->addAction(new QAction("打开文件所在目录",m_menu));
    m_menu->addAction(new QAction("歌曲信息",m_menu));
    m_menu->addAction(new QAction("查看专辑",m_menu));
    m_menu->addSeparator();
    m_menu->addAction(act_del);
    m_menu->addAction(new QAction("删除（包括文件）",m_menu));
    m_menu->addAction(new QAction("重命名",m_menu));
    m_menu->addSeparator();
    m_menu->addAction(new QAction("搜索~~",m_menu));
    m_menu->addAction(new QAction("搜索~~",m_menu));
    m_menu->setContentsMargins(4,10,3,10);
    m_Addtoplistmenu->setContentsMargins(4,10,3,10);

    connect(act_addsong,SIGNAL(triggered(bool)),this,SIGNAL(sig_addSong()));
    connect(act_del,SIGNAL(triggered(bool)),this,SLOT(slot_removeRow()));
    connect(act_play,SIGNAL(triggered(bool)),this,SLOT(slot_actplay()));
}
void myTableWidget::slot_menuRequest(QPoint)//请求菜单
{
    QVector<myTablePlayListFinal*> &plist=m_middleftStack0->myTablePlayListFinalVector();

    int index=0;
    foreach (myTablePlayListFinal*final, plist) {
     QAction* act=new QAction(final->ShowButtonName(),m_Addtoplistmenu);
       act->setObjectName(QString::number(index));
            m_Addtoplistmenu->addAction(act);
            connect(act,SIGNAL(triggered(bool)),this,SLOT(slot_moveToPList()));
            index++;
            if(final==m_finalWidget)
            {
                act->setEnabled(false);
            }
    }
    if(rowCount()==0)
        return;
    int height2=QApplication::desktop()->height()-QCursor::pos().y();
    if(height2<m_menu->height())
    {
       m_menu->exec(QPoint(QCursor::pos().x(),height2));
    }
    else
    {
        m_menu->exec(QCursor::pos());
    }
    QList<QAction*> actlist=  m_Addtoplistmenu->actions();
    foreach (QAction* act, actlist) {
        act->deleteLater();
    }

}
void myTableWidget::slot_moveToPList()//添加到列表中
{

    QVector<myTablePlayListFinal*> &plist=m_middleftStack0->myTablePlayListFinalVector();
    myTablePlayListFinal* final=plist.value(sender()->objectName().toInt());

    QList<QTableWidgetItem*> items=selectedItems();

        int songcount=items.count();
        if(songcount==0)
            return;

      for(int i=0;i<songcount;i++)
       {
          QTableWidgetItem*it=items.at(i);
          int row= it->row();

          if(!final->isContainUrl(m_finalWidget->getUrlByIndex(row)))//url判断在不在列表中
          {
               QString songname=item(row,1)->text();
               QString duration=item(row,2)->text();
               QString url= m_finalWidget->getUrlByIndex(row);
               final->addToPlayList(songname,url,duration.simplified());
          }
      }

}
void myTableWidget::slot_actplay()
{
    if(rowCount()==0)
        return;
    slot_doublick(currentRow(),0);
}
void myTableWidget::slot_removeRow()
{
    removeSong(currentRow());
}

void myTableWidget::init()
{
    m_loveNowRow=0;
    m_prebgItem=-1;
    m_text="";

    m_enter=false;
    m_isGiven=false;
    setMouseTracking(true);
    setFrameShadow(QFrame::Plain);//设置平的样式
    setFrameShape(QFrame::NoFrame);//设置无边样式
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平scrollbar禁用
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    setShowGrid(false);//隐藏线
    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);
    setEditTriggers(QTableWidget::NoEditTriggers);//涓嶅彲缂栬緫
    setSelectionBehavior (QAbstractItemView::SelectRows); //璁剧疆閫夋嫨琛屼负锛屼互琛屼负鍗曚綅
    setSelectionMode (QAbstractItemView::ExtendedSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);

    insertColumn(0);
    insertColumn(1);
    insertColumn(2);
    horizontalHeader()->resizeSection(0,20);
    horizontalHeader()->resizeSection(1,170);
    horizontalHeader()->resizeSection(2,120);
    verticalHeader()->setDefaultSectionSize(32);//设置默认item高度的32

    setMinimumWidth(310);
    setMaximumWidth(380);
    setMinimumHeight(0);
    setStyleSheet("QTableWidget{color:rgb(48,48,48);}"
                  "QTableWidget::item:selected{background:rgb(180,180,180,100);}"
                  "QTableWidget::item{selection-color:rgb(38,38,38);}");
    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(slot_cellEnter(int,int)));

    emit sig_RowCountChange();//发信号
}
void myTableWidget::slot_cellClicked(int, int)
{
    /*QVector<myTablePlayListFinal *> &wList=m_middleftStack0->myTablePlayListFinalVector();
     foreach (myTablePlayListFinal* f, wList) {
         if(f->m_table!=this)//用来找其它的
         {
             f->m_table.setCurrentCell(-1,0);
         }
      }*/
}
bool myTableWidget::eventFilter(QObject *o, QEvent *e)
{
    if(o==&m_playingWid)
    {
        QWheelEvent *ev=static_cast<QWheelEvent*>(e);
        if(e->type()==QEvent::Wheel)
        {
            wheelEvent(ev);//滚动事件
        }
        if(e->type()==QEvent::Enter)
        {
            slot_cellEnter(-1, 0);
        }
        QMouseEvent*me=static_cast<QMouseEvent*>(e);
        if(e->type()==QEvent::MouseButtonPress)
        {
            if(me->button()==Qt::RightButton)
                slot_menuRequest(QCursor::pos());
            else
            {
                setSelectionMode(QAbstractItemView::ExtendedSelection);
                setCurrentCell(currentSongIndex(),0);
            }
        }
        if(e->type()==QEvent::MouseButtonDblClick)
        {
                slot_doublick(currentSongIndex(),0);
        }
    }
    return QObject::eventFilter(o,e);
}

void myTableWidget::mousePressEvent(QMouseEvent *event)
{
// setSelectionMode(QAbstractItemView::ExtendedSelection);
    QTableWidget::mousePressEvent(event);
}

void myTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTableWidget::mouseReleaseEvent(event);
}

void myTableWidget::slot_mvclicked()
{
    if(!m_groupWid)
        return;
    int row= m_groupWid->objectName().toInt();
    QString mvname= item(row,1)->text();

    slot_doublick(m_prebgItem,1,true); //mv
    emit sig_requestMv(mvname);
}

void myTableWidget::slot_playingWidgetLoveBtnClicked()  //this slot is used by playingwidget's lovebtn
{
   int index=currentSongIndex();
   if(index==-1)
       return;
   myTablePlayListFinal*plovelist= m_middleftStack0->getLoveList();
   if(plovelist==m_finalWidget)//if this table is the nowplaytable
   {
       removeSong(index);
       m_middleftStack0->showRemovetips(); //show the tips that removed successfully
       emit sig_setLoveState(true);
   }
   else//need to add or delete
   {
       if(plovelist->isContainUrl(m_finalWidget->getUrlByIndex(index)))//if true  then delete
       {
            int plistindex= plovelist->getIndexByUrl(m_finalWidget->getUrlByIndex(index));
            plovelist->m_table.slot_cellEnter(-1,0);
            plovelist->m_table.removeRow(plistindex);
            emit plovelist->m_table.sig_delIndex(plistindex);
            myDataBase::deleteSong(plovelist->ShowButtonName(),plistindex);

            m_playingWid.setUnloveState();
            emit sig_setLoveState(false);
            m_middleftStack0->showRemovetips();
       }
         else          //add
       {
            QString songname=item(index,1)->text();
            QString url=m_finalWidget->getUrlByIndex(index);
            plovelist->addToPlayList(songname,url,m_text.simplified());

            m_playingWid.setLoveState();
            emit sig_setLoveState(true);
            m_middleftStack0->showAddtips(); //show the tips  that add successfully
       }
   }
}
void myTableWidget::initPlayingWidget()
{
    m_playingWid.raise();
    m_playingWid.hide();
    m_playingWid.installEventFilter(this);

    QHBoxLayout *hlyout=new QHBoxLayout;
    m_addWid=new myTableAddWidget(this);
    hlyout->addWidget(m_addWid);
    hlyout->setContentsMargins(0,0,0,0);
    setLayout(hlyout);

    connect(m_addWid->m_add,SIGNAL(clicked(bool)),this,SIGNAL(sig_addSong()));
    connect(m_addWid->m_addFolder,SIGNAL(clicked(bool)),this,SIGNAL(sig_addSongFolder()));
    connect(&m_playingWid.m_btnDel,SIGNAL(clicked(bool)),this,SLOT(slot_playingWidgetDelBtnClicked()));
    connect(&m_playingWid.m_btnLove,SIGNAL(clicked(bool)),this,SLOT(slot_playingWidgetLoveBtnClicked()));

}
void myTableWidget::slot_doublick(int r, int c,bool isMv)
{
    if(rowCount()==0)
        return;

     myTablePlayListFinal::setCurrentList(m_finalWidget); //设置当前列表


     QFont font1;
     font1.setPixelSize(12);//12字号是默认的

     foreach (myTablePlayListFinal* wp, m_middleftStack0->myTablePlayListFinalVector()) {
          int curindex=wp->m_table.currentSongIndex();
          wp->m_table.setRowHeight(curindex,32);
          wp->m_table.m_playingWid.hide();//把其它的都隐藏起来
          wp->m_table.m_playingWid.setCurrentSongItem(NULL);
          if(wp->m_table.item(curindex,1)!=Q_NULLPTR)
          {
              wp->m_table.item(curindex,1)->setFont(font1);
              wp->m_table.item(curindex,2)->setFont(font1);
          }
        }

         /*
          *we set the big font in order to look like invisible
            */
         QFont font2;
         font2.setPointSize(100);
         item(r,1)->setFont(font2);
         item(r,2)->setFont(font2);

        setRowHeight(r,52);
        m_playingWid.setCurrentSongItem(item(r,c));
        m_playingWid.setSongName(item(r,1)->text());
        m_playingWid.show();
        updatePlayingWidgetPos();

        if(isMv)
            emit sig_playMv(r);
        else
            emit sig_play(r);

       if(m_middleftStack0->getLoveList()->isContainUrl(m_finalWidget->getUrlByIndex(r)))
       {
           m_playingWid.setLoveState();
           emit sig_setLoveState(true);
       }
       else
       {
           m_playingWid.setUnloveState();
           emit sig_setLoveState(false);
       }

       if(!m_middleftStack0->middleListSearchs()->isHidden())//无隐藏状态
       {
           m_middleftStack0->middleListSearchs()->m_lineEdit->setText("");
           m_middleftStack0->middleListSearchs()->hide();
           m_middleftStack0->scrolltoCurrentPlayList();
       }
       setAutoLayoutSize();
}

void myTableWidget::slot_animation(QVariant& var)
{
     m_middleftStack0->verticalScrollBar()->setValue(var.toInt());
}
void myTableWidget::setAutoLayoutSize()
{
    ////////////////////
      int minheight=0;
        foreach(myTablePlayListFinal *f,m_middleftStack0->myTablePlayListFinalVector())
        {
            int height=0;
            int btnheight=f->m_Btntable.height();
            if(!f->m_table.isHidden())//如果显示的话
            {
                for(int i=0;i<f->m_table.rowCount();i++)
                    height+=f->m_table.rowHeight(i);

                if(f->m_table.rowCount()==0)
                    height=m_addWid->height();
                f->m_table.setMinimumHeight(height);
                f->setMaximumHeight(height+btnheight);
                minheight+=(height+btnheight);
            }
            else
            {
                f->m_table.setMinimumHeight(height);
                f->setMaximumHeight(btnheight);
                minheight+=(btnheight);
            }
        }
      m_middleftStack0->GetWidget()->setMinimumHeight(minheight);
    //////////////////////////////////////////

}

int myTableWidget::currentSongIndex()
{
    return m_playingWid.currentSongIndex();
}

void myTableWidget::updatePlayingWidgetPos()
{
    int index= currentSongIndex();
    int height=0;

    for(int i=0;i<index;i++)
    {
        int rh= rowHeight(i);
        height+=rh;
    }
    if(index==-1)//没有播放的
       m_playingWid.hide();
    if(rowHeight(index)==0)
         m_playingWid.hide();
    else
         m_playingWid.show();
    m_playingWid.raise();
    m_playingWid.setGeometry(0,height,width()-1,52);
}

void myTableWidget::removeSong(int row,bool setAutoLayout)
{

    int curindex=currentSongIndex();
    if(row<=-1)
        return;
    if(curindex<row)
    {
        slot_cellEnter(-1,0);
        emit sig_delIndex(row);
        myDataBase::deleteSong(m_finalWidget->ShowButtonName(),row);
        removeRow(row);
    }

    if(curindex==row)//当行正在播放
    {
        if(rowCount()==1)//删除最后一首歌曲
        {
            m_finalWidget->stopCurrentSong();
            emit sig_delIndex(curindex);
            myDataBase::deleteSong(m_finalWidget->ShowButtonName(),curindex);
            removeRow(curindex);
            m_playingWid.setCurrentSongItem(NULL);
        }
        else
        {
            emit sig_delIndex(curindex);
            myDataBase::deleteSong(m_finalWidget->ShowButtonName(),curindex);
            removeRow(curindex);
            m_playingWid.setCurrentSongItem(NULL);
            m_middleftStack0->slot_btnnextSong();//下一曲
        }
    }
    if(curindex>row)
    {
        slot_cellEnter(-1,0);
        emit sig_delIndex(row);
        myDataBase::deleteSong(m_finalWidget->ShowButtonName(),row);
        removeRow(row);
        updatePlayingWidgetPos();
    }

    if(setAutoLayout)
     setAutoLayoutSize();
}

void myTableWidget::showEvent(QShowEvent *e)
{
    QTableWidget::showEvent(e);
    m_finalWidget->m_Btntable.setTableShowIndicator();
    setAutoLayoutSize();
}
void myTableWidget::hideEvent(QHideEvent *e)
{
    QTableWidget::hideEvent(e);
    m_finalWidget->m_Btntable.setTableHideIndicator();
    setAutoLayoutSize();
}
void myTableWidget::resizeEvent(QResizeEvent *e)
{
    QTableWidget::resizeEvent(e);

    horizontalHeader()->resizeSection(1,width()-140);//resizeHeader
    m_playingWid.resize(width()-1,52);
    ////////////////////
}
void myTableWidget::mouseMoveEvent(QMouseEvent *e)
{
    QTableWidget::mouseMoveEvent(e);

   // setSelectionMode(QAbstractItemView::SingleSelection);
    if(itemAt(mapFromGlobal(QCursor::pos()))==Q_NULLPTR)//不在格子上面移动
    {
            if(m_enter)
          slot_cellEnter(-1, 0);
    }
    else
    {
         m_enter=true;
    }

}
void myTableWidget::leaveEvent(QEvent *e)
{
    slot_cellEnter(-1, 0);
    QTableWidget::leaveEvent(e);
}
void myTableWidget::slot_cellEnter(int row, int c)
{
    if(item(m_prebgItem,c)!=Q_NULLPTR&&row!=m_prebgItem)
    {
        removeCellWidget(m_prebgItem,0);
        removeCellWidget(m_prebgItem,2);

        item(m_prebgItem,2)->setText(m_text);
        item(m_prebgItem,2)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);


        item(m_prebgItem,0)->setBackgroundColor(Qt::transparent);
        item(m_prebgItem,1)->setBackgroundColor(Qt::transparent);
        item(m_prebgItem,2)->setBackgroundColor(Qt::transparent);

        m_crossWid->deleteLater();
        m_groupWid->deleteLater();

        m_crossWid=NULL;
        m_groupWid=NULL;

    }
    if(row!=m_prebgItem)
    {
        if(item(row,2)!=Q_NULLPTR)//防止内存无用增加
        {
            m_crossWid=new pushButtonCrossWidget(this);
            m_groupWid=new pushButtonGroupWidget(this);
            m_groupWid->setObjectName(QString::number(row));
            if(m_middleftStack0->getLoveList() == m_finalWidget)//如果是正在我的最爱列表  全部都是红色的
            {
                m_groupWid->setLoved();
                connect(m_groupWid->m_btnLove,SIGNAL(clicked(bool)),this,SLOT(slot_removeHoverRow()));
            }
            else
            {
                QString url=m_finalWidget->getUrlByIndex(row);//当前的url

                if(m_middleftStack0->getLoveList()->isContainUrl(url))//如果歌曲存在在我的喜欢列表中 就有Loveed
                {
                    m_loveNowRow=m_middleftStack0->getLoveList()->getIndexByUrl(url);
                    m_groupWid->setLoved();
                }
                connect(m_groupWid->m_btnLove,SIGNAL(clicked()),this,SLOT(slot_btnloveclicked()));
            }

            setCellWidget(row,0,m_crossWid);
            setCellWidget(row,2,m_groupWid);

            m_text=item(row,2)->text();
            item(row,2)->setText("");
            QColor hoverbgcolor(180,180,180,50);

            this->item(row,0)->setBackgroundColor(hoverbgcolor);
            this->item(row,1)->setBackgroundColor(hoverbgcolor);
            this->item(row,2)->setBackgroundColor(hoverbgcolor);
            connect(m_groupWid->m_btnDel,SIGNAL(clicked()),this,SLOT(slot_removeHoverRow()));
            connect(m_groupWid->m_btnMovie,SIGNAL(clicked(bool)),this,SLOT(slot_mvclicked()));
        }
        m_prebgItem=row;
    }
    m_enter=true;
}

void myTableWidget::wheelEvent(QWheelEvent *e)
{
    QTableWidget::wheelEvent(e);
}
void myTableWidget::slot_rowCountChanged()//如果空
{
    if(rowCount()==0)
    {
        m_addWid->show();
        m_playingWid.hide();
        m_playingWid.setOriginalState();
    }
    else
    {
        m_addWid->hide();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

