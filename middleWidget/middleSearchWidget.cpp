#include "middleSearchWidget.h"

#include<QHeaderView>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QPainter>
#include<QTime>
#include<QMessageBox>
#include<QMenu>
#include<QScrollBar>

#include"Global_ValueGather.h"
#include"middleLeftStackWidget0.h"
#include"myTablePlayListFinal.h"
#include"myPushButton.h"

#define USE_NETCLOUD 0

loadingWidget::loadingWidget(QString pixurl,int tinypixcount,QString text,QWidget *p):baseWidget(p)
{
    setStyleSheet("QWidget{background:white;}");
    _index=0;
    m_text=text;

    QPixmap pix=QPixmap(pixurl);
    animationtimeline = new QTimeLine(800);
    animationtimeline->setFrameRange(0,12);
    animationtimeline->setCurveShape(QTimeLine::LinearCurve);
    connect(animationtimeline,SIGNAL(frameChanged(int)),SLOT(slot_valuechange(int)));
    animationtimeline->setLoopCount(0);

    for(int i=0;i<tinypixcount;i++)//进入
    {
        m_listpix<<pix.copy(i*(pix.width()/tinypixcount),0,pix.width()/tinypixcount,pix.height());
    }
}
void loadingWidget::paintEvent(QPaintEvent *p)
{
    baseWidget::paintEvent(p);
    QPainter pter(this);
    pter.drawPixmap(width()/2-m_listpix.value(0).width()*2,(height()-m_listpix.value(0).height())/2,m_listpix.at(_index));

    QFont ft("微软雅黑");
    ft.setPixelSize(14);
    QFontMetrics metric(ft);
    pter.setPen(QColor(55,174,254));
    pter.setFont(ft);
    pter.drawText(width()/2-m_listpix.value(0).width()+10,
                  (height()-metric.height())/2,metric.width(m_text),metric.height(),Qt::AlignLeft,m_text);

}

void loadingWidget::showEvent(QShowEvent *e)
{
        baseWidget::showEvent(e);
        if(this->isHidden())
            animationtimeline->stop();
        else
        {
            animationtimeline->start();
            raise();
        }
}
////////////////////////////////////////////////////////////////////////////////
middleSearchWidget::middleSearchWidget(QWidget*p):baseWidget(p)
{
    setStyleSheet("QLabel{color:rgb(153,153,153);font-size:12px;}"
                  "QCheckBox{color:rgb(153,153,153);}"
                  "QPushButton{border:1px solid rgb(204,204,204);color:rgb(95,95,95);border-radius:2px;}"
                  "QPushButton:hover{border:1px solid rgb(161,199,238);color:rgb(22,154,243);}");
    m_isRequestFinished=true;

    init();
    initMaskWidget();
}

void middleSearchWidget::slot_addRequestSong(const QByteArray& byt)
{
#if USE_NETCLOUD
    QJsonDocument doc=QJsonDocument::fromJson(byt);
    QJsonObject obj0=doc.object();
    QJsonObject obj1=obj0.value("result").toObject();
    QJsonArray arry=obj1.value("songs").toArray();
    int arrycount=arry.count();
    for(int i=0;i<arrycount;i++)//在这里完成搜索插入列表
    {
       QJsonObject obj2=arry.at(i).toObject();
       m_songlist<<obj2.value("mp3Url").toString();//添加mp3Url

       int dur=obj2.value("duration").toInt();
       QTime total_time(0, (dur/60000)%60, (dur/1000)%60);
       QString duration=total_time.toString("mm:ss");

       QJsonObject albumnObj=obj2.value("album").toObject();
       QString albumname= albumnObj.value("name").toString();


       QString songname=obj2.value("name").toString();
       QJsonArray arry1=obj2.value("artists").toArray();
       QJsonObject obj3=arry1.at(0).toObject();
       QString author= obj3.value("name").toString();

       int row= m_table->rowCount();
       m_table->insertRow(row);
       m_table->setItem(row,0,new QTableWidgetItem(""));
       m_table->setItem(row,1,new QTableWidgetItem(author+"-"+songname));
       m_table->setItem(row,2,new QTableWidgetItem(albumname));
       m_table->setItem(row,3,new QTableWidgetItem(duration));
       m_table->setItem(row,4,new QTableWidgetItem(""));
    }
#else
    QJsonDocument doc=QJsonDocument::fromJson(byt);
    QJsonArray array=doc.array();
    for(int i=0;i<array.count();i++)//在这里完成搜索插入列表
    {
        QJsonObject obj=array.at(i).toObject();

        m_songlist<<obj.value("ListenUrl").toString();//添加mp3Url

        QString name= obj.value("SongName").toString();
        QString author=obj.value("Artist").toString();
        QString duration=obj.value("Length").toString();
        QString ablumname=obj.value("Album").toString();

        int row= m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row,0,new QTableWidgetItem(""));
        m_table->setItem(row,1,new QTableWidgetItem(name));
        m_table->setItem(row,2,new QTableWidgetItem(author));
        m_table->setItem(row,3,new QTableWidgetItem(ablumname));
        m_table->setItem(row,4,new QTableWidgetItem(duration));
        m_table->setItem(row,5,new QTableWidgetItem(""));
    }
#endif
    setRequestisFinished(true);//request finished so we could request the song of the next page;
}
void middleSearchWidget::initMaskWidget()
{
    m_maskwid=new loadingWidget(":/image/middlewidget/loading.png",12,"正在努力加载中~~",this);
    m_maskwid->setGeometry(0,1,width(),height());
    m_maskwid->show();
}
void middleSearchWidget::slot_requestSong(const QByteArray &byt)//返回的chao
{
    m_maskwid->hide();
    m_songlist.clear();
    int count=m_table->rowCount();
    for(int i=0;i<count;i++)
    {
         m_table->removeRow(0); //不是一个好办法 的没办法
    }
    slot_addRequestSong(byt);
}

void middleSearchWidget::resizeEvent(QResizeEvent *e)
{
    baseWidget::resizeEvent(e);
    m_maskwid->setGeometry(0,1,width(),height());

    m_table->horizontalHeader()->resizeSection(1,(m_table->width()-150)/3);
    m_table->horizontalHeader()->resizeSection(2,(m_table->width()-150)/3);
    m_table->horizontalHeader()->resizeSection(3,(m_table->width()-150)/3);

    m_labelSinger->setGeometry(m_table->columnWidth(1)+13+(m_table->columnWidth(2)-m_labelSinger->width())/2+7,
                               62,
                               m_labelSinger->width(),
                               m_labelSinger->height());

    m_labelAlbum->setGeometry(m_table->columnWidth(1)+13+m_table->columnWidth(2)+(m_table->columnWidth(3)-m_labelAlbum->width())/2+7,
                              62,
                              m_labelAlbum->width(),
                              m_labelAlbum->height());
}

void middleSearchWidget::slot_autoRequestNextPage(int value)
{
    if(value==0)
        return;
    if(m_table->verticalScrollBar()->maximum()==value)//scroll to bottom
    {
        if(m_isRequestFinished)
        {
          emit sig_requestSongNextPage();
          setRequestisFinished(false);//prevent to request network for several times;
        }
    }

}
void middleSearchWidget::init()
{
    QHBoxLayout *hlyout=new QHBoxLayout;

    QHBoxLayout *hlyout1=new QHBoxLayout;

    QVBoxLayout *vlyout=new QVBoxLayout;


    m_table=new middleSearchTableWidget(this);
    m_table->setInitSearchTableWidget(this);


    baseWidget *wid1=new baseWidget(this);
    wid1->setFixedHeight(36);

    QLabel *label=new QLabel("搜索到",this);
    m_labtext=new QLabel("",this);
    QLabel *label1=new QLabel("的相关歌曲",this);

    m_btnplay=new myPushButton("播放",this);
    m_btnadd=new myPushButton("添加",this);
    m_btndownload=new myPushButton("下载",this);

    m_labtext->setStyleSheet("QLabel{color:red;}");

    m_btnplay->setStyleSheet("QPushButton{border:1px;color:white;background:rgb(22,154,243);border-radius:2px;}"
                             "QPushButton:hover{background:rgb(21,143,225);}");

    m_btnplay->setFixedSize(53,26);
    m_btnadd->setFixedSize(40,26);
    m_btndownload->setFixedSize(40,26);

    hlyout->addWidget(label);
    hlyout->addWidget(m_labtext);
    hlyout->addWidget(label1);
    hlyout->addStretch();
    hlyout->addWidget(m_btnplay);
    hlyout->addWidget(m_btnadd);
    hlyout->addWidget(m_btndownload);
    wid1->setContentsMargins(0,0,0,0);
    wid1->setLayout(hlyout);

    baseWidget *wid2=new baseWidget(this);
    wid2->setFixedHeight(32);
    wid2->setStyleSheet("QWidget{background:rgb(245,245,245);}");
    m_checkbox=new QCheckBox("歌曲名",this);
    m_labelSinger=new QLabel("歌手",this);
    m_labelAlbum=new QLabel("专辑",this);
    QLabel *label4=new QLabel("时长",this);
    QLabel *label5=new QLabel("操作",this);

    m_checkbox->setStyleSheet("QCheckBox::indicator:checked{border-image:url(:/image/middlewidget/checked.png)}"
                                   "QCheckBox::indicator:unchecked{border-image:url(:/image/middlewidget/unchecked.png)}");

    m_labelSinger->adjustSize();
    m_labelAlbum->adjustSize();
    label4->setFixedSize(55,32);
    label5->setFixedSize(85,32);

    m_labelSinger->setAlignment(Qt::AlignCenter);
    m_labelAlbum->setAlignment(Qt::AlignCenter);
    label4->setAlignment(Qt::AlignCenter);
    label5->setAlignment(Qt::AlignCenter);

    hlyout1->addWidget(m_checkbox);
    hlyout1->addStretch(1);
    hlyout1->addWidget(label4);
    hlyout1->addWidget(label5);
    hlyout1->setSpacing(0);
    hlyout1->setContentsMargins(0,0,0,0);
    wid2->setLayout(hlyout1);


    vlyout->addWidget(wid1);
    vlyout->addSpacing(10);
    vlyout->addWidget(wid2);
    vlyout->addWidget(m_table);
    vlyout->setSpacing(0);
    vlyout->setContentsMargins(5,5,5,0);
    setLayout(vlyout);

    connect(m_btnadd,SIGNAL(clicked()),this,SLOT(slot_btnaddclicked()));
    connect(m_btnplay,SIGNAL(clicked()),this,SLOT(slot_btnplayclicked()));
    connect(m_btndownload,SIGNAL(clicked()),this,SLOT(slot_btndownloadclicked()));
    connect(m_checkbox,SIGNAL(clicked()),this,SLOT(slot_checkBoxClicked()));

    connect(m_table->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(slot_autoRequestNextPage(int)));
}
void middleSearchWidget::slot_checkBoxClicked()
{
    if(m_checkbox->checkState()==Qt::Checked)
    {
        for(int i=0;i<m_table->rowCount();i++)
        {
           QCheckBox*box= (QCheckBox*)m_table->cellWidget(i,0);
           box->setCheckState(Qt::Checked);
        }
    }
    else
    {
        for(int i=0;i<m_table->rowCount();i++)
        {
           QCheckBox*box= (QCheckBox*)m_table->cellWidget(i,0);
           box->setCheckState(Qt::Unchecked);
        }
    }
}

void middleSearchWidget::slot_menuWork()
{
  int index=sender()->objectName().toInt();
  QStringList list_name;
  QStringList list_dur;
  QStringList list_url;

  int count=m_table->rowCount();
  for(int i=0;i<count;i++)
  {
      QCheckBox* box= (QCheckBox*)m_table->cellWidget(i,0);
      playingWidgetBtn *btn=NULL;
          if(box->checkState()==Qt::Checked)//如果选中项
          {
              btn=(playingWidgetBtn*)m_table->cellWidget(i,2);
              list_name<<btn->text()+"-"+m_table->item(i,1)->text();

              list_dur<<m_table->item(i,4)->text();

              list_url<<m_songlist.value(i);
          }
  }
  if(list_name.count()==0)
  {
      QMessageBox::information(NULL,"提示","请选择一首歌曲");
      return;
  }
   midstack0Pointer->myTablePlayListFinalVector().value(index)->slot_addSongFromSearchTable(list_name,list_url,list_dur);

}
void middleSearchWidget::slot_btnplayclicked()//obviously,playbutton
{
    QStringList list_name;
    QStringList list_dur;
    QStringList list_url;

    int count=m_table->rowCount();
    for(int i=0;i<count;i++)
    {
        QCheckBox* box= (QCheckBox*)m_table->cellWidget(i,0);
        playingWidgetBtn *btn=NULL;
         if(box->checkState()==Qt::Checked)//如果选中项
            {
                btn=(playingWidgetBtn*)m_table->cellWidget(i,2);
                list_name<<btn->text()+"-"+m_table->item(i,1)->text();

                list_dur<<m_table->item(i,4)->text();

                list_url<<m_songlist.value(i);
            }
    }
    if(list_name.count()!=0)
        midstack0Pointer->myTablePlayListFinalVector().value(0)->slot_playSongFromSearchTable(list_name,list_url,list_dur);
    else
        QMessageBox::information(NULL,"提示","请选择一首歌曲");
}
void middleSearchWidget::slot_btnaddclicked()
{
    QMenu menu;
    menu.setContentsMargins(4,5,4,10);
    int index=0;
    foreach(myTablePlayListFinal*f,midstack0Pointer->myTablePlayListFinalVector())
    {
        QAction *act=new QAction(f->ShowButtonName(),&menu);
        act->setObjectName(QString::number(index));
        connect(act,&QAction::triggered,this,&middleSearchWidget::slot_menuWork);
        menu.addAction(act);
        index++;
    }
    menu.exec(QCursor::pos());

    QList<QAction*> actlist=  menu.actions();
    foreach (QAction* act, actlist) {
        act->deleteLater();
    }
}
void middleSearchWidget::slot_btndownloadclicked()
{

}
