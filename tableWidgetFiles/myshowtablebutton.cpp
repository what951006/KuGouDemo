#include "myshowtablebutton.h"
#include<QHBoxLayout>
#include<QMessageBox>
#include<QPixmap>
#include<QPainter>
#include<QDebug>
#include<QMimeData>
#include<QDrag>
#include<qmath.h>

#include"Global_ValueGather.h"
#include"dataBase/myDataBase.h"
#include"myTablePlayListFinal.h"
#include"middleLeftStackWidget0.h"
#include"middlewidgetleft.h"
#include"middleWidgets.h"
#include"mytablewidget.h"
#include"mainwindow.h"
inline QString mimeType(){return "ShowButton";}

myShowTableButton::myShowTableButton(QWidget*parent):QPushButton(parent)
{
    m_isdrawMove=false;
    m_finalWid=(myTablePlayListFinal*)parent;
    setAcceptDrops(true);
    init();
    initMenu();
    setFixedHeight(40);
}
void myShowTableButton::initMenu()
{
    m_menu=new QMenu;
    QAction *act_addplaylist=new QAction("新建列表",m_menu);
    QAction *act_addsong=new QAction("添加歌曲",m_menu);
    QAction *act_delplayList=new QAction("删除列表",m_menu);
    QAction *act_reName=new QAction("重命名",m_menu);
    QAction* act_emptyList=new QAction("清空列表",m_menu);

    m_menu->addAction(act_addplaylist);
    m_menu->addSeparator();
    m_menu->addAction(act_addsong);
    m_menu->addAction(new QAction("稍后播",m_menu));
    m_menu->addAction(new QAction("添加到播放列表",m_menu));
    m_menu->addAction(new QAction("全部发送到移动设备",m_menu));
    m_menu->addAction(new QAction("下载本列表全部歌曲",m_menu));
    m_menu->addAction(new QAction("排序",m_menu));
    m_menu->addAction(new QAction("匹配MV",m_menu));
    m_menu->addAction(new QAction("收藏整个列表",m_menu));
    m_menu->addSeparator();
    m_menu->addAction(act_emptyList);
    m_menu->addAction(act_delplayList);
    m_menu->addAction(act_reName);
    m_menu->setContentsMargins(4,10,3,10);

    m_menu->actionGeometry(act_reName);//一定不能删除 setEnabledMenuItem要工作必须要它 可能是一个bug// must to call this function if not the setEnabledMenuItem function cant work correctly;

    connect(act_emptyList,SIGNAL(triggered(bool)),this,SIGNAL(sig_emptyList()));
    connect(act_reName,SIGNAL(triggered(bool)),this,SLOT(slot_ReName()));
    connect(act_delplayList,SIGNAL(triggered(bool)),this,SIGNAL(sig_delPlayList()));
    connect(act_addplaylist,SIGNAL(triggered(bool)),this,SIGNAL(sig_addPlayList()));
    connect(act_addsong,SIGNAL(triggered(bool)),this,SIGNAL(sig_addSong()));
    connect(this,SIGNAL(sig_reName(QString)),this,SLOT(slot_reNameDB(QString)));

}

void myShowTableButton::setFinalWidget(QWidget *p)
{
    if(m_finalWid==p)
        return;
    m_finalWid=(myTablePlayListFinal*)p;

    foreach (myTablePlayListFinal*f, m_finalWid->midLeft0()->myTablePlayListFinalVector()) { //断掉所有
        disconnect(this,SIGNAL(sig_emptyList()),f,SLOT(slot_emptyList()));//清空列表
        disconnect(this,SIGNAL(sig_addSong()),f,SLOT(slot_addSong()));//添加歌曲
        disconnect(f->m_table,SIGNAL(sig_RowCountChange()),this,SLOT(slot_updateSongCount()));//歌曲列表改变信息
    }

    connect(this,SIGNAL(sig_emptyList()),m_finalWid,SLOT(slot_emptyList()));//清空列表
    connect(this,SIGNAL(sig_addSong()),m_finalWid,SLOT(slot_addSong()));//添加歌曲
    connect(m_finalWid->m_table,SIGNAL(sig_RowCountChange()),this,SLOT(slot_updateSongCount()));//歌曲列表改变信息
    slot_updateSongCount();
}

 void myShowTableButton::setTableShowIndicator()
{
   m_indicator->setStyleSheet("QLabel{border-image:url(:/image/middlewidget/indicator_down (1).png);}"
                              "QLabel:hover{border-image:url(:/image/middlewidget/indicator_down (2).png);}"
                              "QLabel:pressed{border-image:url(:/image/middlewidget/indicator_down (1).png);}");
}

 void myShowTableButton::setTableHideIndicator()
{
    m_indicator->setStyleSheet("QLabel{border-image:url(:/image/middlewidget/indicator_top (1).png);}"
                               "QLabel:hover{border-image:url(:/image/middlewidget/indicator_top (2).png);}"
                               "QLabel:pressed{border-image:url(:/image/middlewidget/indicator_top (1).png);}");
}

 void myShowTableButton::setEnabledMenuItem(bool isSetting)
{
    m_menu->actionAt(QPoint(5,309))->setEnabled(isSetting);//del
    m_menu->actionAt(QPoint(5,337))->setEnabled(isSetting);//rename
}

void myShowTableButton::setTipsStyle(bool isset)
{
    m_isTipsStyle=isset;
    setTableShowIndicator();
    update();
}

void myShowTableButton::slot_updateSongCount()
{
    if(m_finalWid->m_table->rowCount()==0)
     m_playlistName->setText(m_finalWid->ShowButtonName());
    else
    m_playlistName->setText(m_finalWid->ShowButtonName()+QString("[%1]").arg(QString::number(m_finalWid->m_table->rowCount())));
}
void myShowTableButton::slot_ReName()
{
    m_lineEdit->show();
    m_lineEdit->setFocus();
    m_lineEdit->raise();
    m_playlistName->hide();
    m_lineEdit->setText(m_playlistName->text().split("[").at(0));
    m_lineEdit->selectAll();
}
void myShowTableButton::slot_reNameDB(const QString &newName)
{
    QString oldname= m_finalWid->ShowButtonName();
    myDataBase::renameList(oldname,newName);
}

void myShowTableButton::dragLeaveEvent(QDragLeaveEvent *)
{
    m_isdrawMove=false;
    m_isdrawTop=false;
    update();
}

void myShowTableButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        QPushButton::mousePressEvent(e);
    }
    m_presspos=e->pos();
}

void myShowTableButton::dragMoveEvent(QDragMoveEvent *e)
{
   if(e->mimeData()->hasFormat(mimeType())&&midstack0Pointer->isEnableMoveList(m_finalWid))
   {
       m_isdrawMove=true;
       if(e->pos().y()<height()/2)
       {
           m_isdrawTop=true;
           update();
       }
       else
       {
           m_isdrawTop=false;
           update();
       }
   }
}

void myShowTableButton::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(mimeType()))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void myShowTableButton::dropEvent(QDropEvent *e)
{
     m_isdrawMove=false;
     update();
    if(e->mimeData()->hasFormat(mimeType())&&midstack0Pointer->isEnableMoveList(m_finalWid))
    {
       QByteArray byt= e->mimeData()->data(mimeType());
       int index= byt.toInt();
       if(m_isdrawTop)
       {
            int targetIndex= midstack0Pointer->myTablePlayListFinalVector().indexOf(m_finalWid);
            midstack0Pointer->setListTakeAndInsert(midstack0Pointer->myTablePlayListFinalVector().value(index),midstack0Pointer->myTablePlayListFinalVector().value(targetIndex-1));
       }
       else
       {
           midstack0Pointer->setListTakeAndInsert(midstack0Pointer->myTablePlayListFinalVector().value(index),m_finalWid);
       }
    }
}

void myShowTableButton::mouseMoveEvent(QMouseEvent *e)
{
    QRect re(m_presspos.x()-2,m_presspos.y()-2,m_presspos.x()+2,m_presspos.y()+2);
    if(!re.contains(e->pos())&&midstack0Pointer->isEnableMoveList(m_finalWid))
    {
        int index= midstack0Pointer->myTablePlayListFinalVector().indexOf(m_finalWid);
    //! [15]
        QMimeData *mimeData=new QMimeData; //自动释放
        mimeData->setData(mimeType(), QString::number(index).toUtf8());
        mimeData->setText(m_finalWid->ShowButtonName());

        QDrag *drag = new QDrag(this); //自动释放
        drag->setMimeData(mimeData);

        QPixmap pixmap =this->grab(rect());
        drag->setHotSpot(QPoint(0,height()/2));
        drag->setPixmap(pixmap);
    //! [17]
        drag->exec(Qt::MoveAction);
        if(drag)
            drag->deleteLater();
        if(mimeData)
            mimeData->deleteLater();
    }
}


bool myShowTableButton::eventFilter(QObject *o, QEvent *e)
{
    if(o==m_lineEdit)
    {
        if(e->type()==QEvent::FocusOut)//
        {
            slot_returnPressed();
        }
    }
    return QObject::eventFilter(o,e);
}



void myShowTableButton::paintEvent(QPaintEvent *)
{
    if(m_isTipsStyle)
    {
        QPainter painter(this);
        QLinearGradient gradient;
        gradient.setStart(0,37);
        gradient.setFinalStop(0,height());
        gradient.setColorAt(0.1,QColor(150,150,150,150));
        gradient.setColorAt(0.9,QColor(180,180,180,50));

        painter.setPen(Qt::transparent);
        painter.setBrush(gradient);
        painter.drawRect(0,37,width(),height());

        if(mainwid)
        {
            if(mainwid->m_mainwid->CurrentIndex()==5)
                painter.drawPixmap(0,0,width(),height()-3
                            ,mainwid->m_mainwid->getRectPix(QRect(0,90,width(),height()-3)));
            else
            {
                painter.setBrush(Qt::white);
                painter.drawRect(-1,0,width(),height()-3);
            }
        }
        painter.setBrush(middleWidgetLeft::bgcolor);
        painter.drawRect(-1,0,width(),height()-3);
    }

    if(m_isdrawMove)
    {
        QPainter p(this);
        if(m_isdrawTop)
        {
            p.drawLine(0,0,width(),0);
        }
        else
        {
            p.drawLine(0,height()-1,width(),height()-1);
        }
    }
}
void myShowTableButton::init()
{
    m_isTipsStyle=false;
    setCursor(Qt::PointingHandCursor);
    this->setAcceptDrops(true);


    m_lineEdit=new QLineEdit(this);
    m_lineEdit->setGeometry(16,5,250,30);
    m_lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_lineEdit->setStyleSheet("QLineEdit{border:4px solid rgb(40,80,150);}");
    m_lineEdit->installEventFilter(this);
    m_lineEdit->raise();
    m_lineEdit->hide();

    QHBoxLayout *lyout=new QHBoxLayout;

    m_btnmenu=new myPushButton(this);
    m_indicator=new QLabel(this);
    m_playlistName=new QLabel("新建列表[]",this);
    m_playlistName->setStyleSheet("color:rgb(38,38,38);font: 14px 黑体;");


    m_indicator->setFixedSize(16,16);
    m_btnmenu->setFixedSize(16,16);

    m_indicator->setStyleSheet("QLabel{border-image:url(:/image/middlewidget/indicator_top (1).png);}"
                               "QLabel:hover{border-image:url(:/image/middlewidget/indicator_top (2).png);}"
                               "QLabel:pressed{border-image:url(:/image/middlewidget/indicator_top (1).png);}");

    m_btnmenu->setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/indicator_menu (1).png);}"
                             "QPushButton:hover{border-image:url(:/image/middlewidget/indicator_menu (2).png);}"
                             "QPushButton:pressed{border-image:url(:/image/middlewidget/indicator_menu (3).png);}");

    lyout->addWidget(m_indicator,0,Qt::AlignVCenter);
    lyout->addWidget(m_playlistName,0,Qt::AlignVCenter);
    lyout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding,QSizePolicy::Fixed));
    lyout->addWidget(m_btnmenu,0,Qt::AlignVCenter);
    lyout->addSpacing(14);
    lyout->setContentsMargins(0,0,0,0);
    lyout->setSpacing(2);
    setLayout(lyout);

    connect(m_lineEdit,SIGNAL(returnPressed()),this,SLOT(setFocus()));
    connect(m_btnmenu,SIGNAL(clicked(bool)),this,SLOT(slot_menuRequest()));
}

void myShowTableButton::slot_returnPressed()
{
  QList<myShowTableButton*> btnlist= m_finalWid->midLeft0()->m_wid->findChildren<myShowTableButton*>();

  foreach(myShowTableButton*btn,btnlist)
  {
    QString pname= btn->m_playlistName->text().split("[").at(0);
    if(btn!=this&&btn!=m_finalWid->m_Btntable)
    {
        if(pname==m_lineEdit->text())
        {
            QMessageBox::warning(NULL,"warning","the same playlist name!");
            slot_ReName();
            return;
        }
    }
  }
    if(m_lineEdit->text().isEmpty())
        slot_ReName();
    else
    {
        emit sig_reName(m_lineEdit->text()); //要先发送消息

        m_finalWid->setShowButtonName(m_lineEdit->text());
        m_finalWid->updateCount();
        m_playlistName->show();
        m_lineEdit->hide();
        slot_updateSongCount();
    }
}

void myShowTableButton::mouseReleaseEvent(QMouseEvent*e)
{
    if(e->button()==Qt::RightButton)
    {
         m_menu->exec(QCursor::pos());
    }
    QPushButton::mouseReleaseEvent(e);
}
