#include "skinWidget.h"


#include<QFileDialog>
#include<QFontMetrics>
#include<QApplication>
#include<QFileDialog>
#include<QScrollBar>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QScrollArea>
#include<QLabel>
#include<QDebug>
skinWidget::skinWidget(QWidget *parent)
    : baseDialog(parent)
    ,scrollArea(this)
    ,m_skincontwid(this)
    ,m_btnOpacity("",this)
    ,m_btnAdjustWindow("恢复窗口默认状态",this)
    ,m_sliderWidget(this)
{
    setStyleSheet("QLabel{color:rgb(154,154,154);}"
                        "QPushButton{color: rgb(154,154,154);border:1px solid rgb(197,197,197);}"
                        "QPushButton:hover{border:1px solid rgb(100,176,250); color: rgb(100,176,250);}");
    setFixedSize(570,450);
    myPushButton *m_btn=new myPushButton(&m_mainwid);
    QLabel *label=new QLabel("主题皮肤与窗口调整",&m_mainwid);
    m_btn->setGeometry(540,5,20,20);
    label->setGeometry(5,5,200,20);
    label->setStyleSheet("QLabel{font-size: 14px;color: white;}");
    m_btn->setStyleSheet("QPushButton{border-image:url(:/image/topwidget/btn_close (1).png);}"
                          "QPushButton::hover{border-image:url(:/image/topwidget/btn_close (2).png);}"
                          "QPushButton::pressed{border-image:url(:/image/topwidget/btn_close (3).png);}");

    connect(m_btn,SIGNAL(clicked(bool)),this,SLOT(hide()));//隐藏

    initTopLayout();
    initBottomLayout();
}

void skinWidget::initTopLayout()
{
    scrollArea.setParent(&m_mainwid);
    scrollArea.verticalScrollBar()->setStyleSheet("QScrollBar{background:white; width:12px; padding-right: 4px;}"
                                      "QScrollBar::handle{background:rgb(217,217,217);border-radius:4px;}"
                                      "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                                      "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                      "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                      "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");

    scrollArea.setGeometry(0,30,564,350);
    QWidget *contentwidget=new QWidget(&scrollArea);
    scrollArea.setWidget(contentwidget);
    contentwidget->setGeometry(0,0,scrollArea.width(),scrollArea.height());

    contentwidget->setStyleSheet("QWidget{background: white;}");

    m_skincontwid.setParent(contentwidget);
    m_skincontwid.setSkinWidget(this);

    QLabel *label2=new QLabel("皮肤",contentwidget);
    label2->setFixedSize(100,24);


    m_btnAdjustWindow.setParent(contentwidget);
    m_btnAdjustWindow.setStyleSheet("QPushButton{border:NULL; color: rgb(134,134,134);}"
                          "QPushButton::hover{ text-decoration: underline; color: rgb(100,176,250);}");

    QHBoxLayout *hlyout=new QHBoxLayout;
    hlyout->addWidget(label2);
    hlyout->addStretch(10);
    hlyout->addWidget(&m_btnAdjustWindow);
    hlyout->setContentsMargins(20,0,20,0);


    QVBoxLayout *lyout=new QVBoxLayout;
    lyout->addLayout(hlyout);
    lyout->addWidget(&m_skincontwid);
    lyout->setSpacing(4);
    lyout->setContentsMargins(0,0,0,0);
    contentwidget->setLayout(lyout);
}

void skinWidget::setdefaultSkin()
{
    m_skincontwid.setDefaultSkin();
}

void skinWidget::initBottomLayout()
{
    QWidget* bottomwid=new QWidget(&m_mainwid);
    bottomwid->setStyleSheet("QWidget{background:white}");
    bottomwid->setGeometry(0,380,564,100);

    QLabel *label=new QLabel("列表透明",bottomwid);
    label->setGeometry(20,25,60,20);


    m_btnOpacity.setParent(bottomwid);
    m_btnOpacity.setGeometry(80,24,73,24);

    m_sliderWidget.setParent(&m_mainwid);
    m_sliderWidget.hide();
    m_sliderWidget.setGeometry(80,254,72,150);

    myPushButton *btnpersonal=new myPushButton("个性化壁纸",bottomwid);
    btnpersonal->setGeometry(470,24,73,24);

    connect(btnpersonal,SIGNAL(clicked(bool)),this,SLOT(slot_openPersonalBG()));

    connect(&m_sliderWidget.m_slider,SIGNAL(valueChanged(int)),this,SLOT(slot_setOpacityText(int)));
    connect(&m_btnOpacity,SIGNAL(clicked(bool)),&m_sliderWidget,SLOT(show()));
    connect(&m_btnOpacity,SIGNAL(clicked(bool)),&m_sliderWidget,SLOT(setFocus()));
}

void skinWidget::slot_openPersonalBG()
{
   QString filename= QFileDialog::getOpenFileName(this,"添加个性化图片","","*.*;;.png;;.jpg");
   QFileInfo info(filename);

   if(info.suffix().contains("png")||info.suffix().contains("jpg"))
   {
       QFile file(filename);
       file.copy(filename,QApplication::applicationDirPath()+"/skin/"+info.fileName());
   }
   m_skincontwid.UpdateSkin();
}

void skinWidget::slot_setOpacityText(int opa)
{
   int value= 100*opa/255;
   m_btnOpacity.setText(QString::number(value)+="%");
}
////////////////////////////////////////////////////////////////////////////widget

skinContentWidget::skinContentWidget(QWidget *p):baseWidget(p)
{
    m_sigmap=new QSignalMapper(this);
    connect(m_sigmap,SIGNAL(mapped(QString)),this,SIGNAL(sig_setBackground(QString)));
    skinWidgetContentItem *it=new skinWidgetContentItem(":/image/skin/默认.jpg",this);
    it->setMaskText("默认");
    addSkin(it);

    loadFromDir(QApplication::applicationDirPath()+"/skin");//本地自动加载
}

void skinContentWidget::setSkinWidget(skinWidget *p)
{
    m_skinWid=p;
    connect(m_sigmap,SIGNAL(mapped(QString)),&m_skinWid->m_mainwid,SLOT(setSkin(QString)));
}

void skinContentWidget::addSkin(skinWidgetContentItem *widitem)
{
    m_vector<<widitem;
    m_sigmap->setMapping(widitem,widitem->PixPath());
    connect(widitem,SIGNAL(clicked()),m_sigmap,SLOT(map()));
    connect(widitem,SIGNAL(clicked()),this,SLOT(slot_itemClicked()));

        for(int y=0;y<=m_vector.size()/4;y++)//纵列
       {
            int xcount=m_vector.size()- y*4;
            if(xcount>4)
             xcount=4;
            for(int x=0;x<xcount;x++)//横列
            {
                m_vector.value(y*4+x)->setGeometry(x*(125+4)+20,y*(80+4),widitem->width(),widitem->height());
            }
        }
     widitem->show();
     widitem->raise();
     parentWidget()->setMinimumHeight((m_vector.size()/4+1)*88+20);//scrollbar need this value
}

void skinContentWidget::loadFromDir(const QString &dirrr)
{
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

        for(int i=0; i<file_count;i++)
        {
            QFileInfo file_info = list.at(i);
            QString suffix = file_info.suffix();
            if(QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0||QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0)
            {
              QString abfp= file_info.absoluteFilePath();
              skinWidgetContentItem* item= new skinWidgetContentItem(this);
              item->loadFromPath(abfp);
              item->setMaskText(file_info.baseName());
              addSkin(item);
            }
        }
}

void skinContentWidget::setDefaultSkin()
{
    emit m_vector.value(0)->clicked();
}
void skinContentWidget::UpdateSkin()
{

    QDir dir(QApplication::applicationDirPath()+"/skin");
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

        for(int i=0; i<file_count;i++)
        {
            QFileInfo file_info = list.at(i);
            QString suffix = file_info.suffix();
            if(QString::compare(suffix, QString("png"), Qt::CaseInsensitive) == 0||QString::compare(suffix, QString("jpg"), Qt::CaseInsensitive) == 0)
            {
                QString FP=file_info.absoluteFilePath();
                if(m_sigmap->mapping(FP)==NULL)
                 {
                    skinWidgetContentItem* item= new skinWidgetContentItem(this);
                    item->loadFromPath(FP);
                    item->setMaskText(file_info.baseName());
                    addSkin(item);
                }
            }
        }
}

void skinContentWidget::slot_itemClicked()
{
    for(int i=0;i<m_vector.size();i++)
    {
        m_vector.value(i)->setSelected(false);
    }
    skinWidgetContentItem *item= (skinWidgetContentItem*)sender();
    item->setSelected(true);

}

/////////////////////////////////////////////////////////////////////////////////// item
skinWidgetContentItem::skinWidgetContentItem(QWidget *p):baseWidget(p)
{
    setFixedSize(125,80);
    m_isenter=false;
    m_isselected=false;
    m_selePix=QPixmap(":/image/otherWidget/selected.png");
}

skinWidgetContentItem::skinWidgetContentItem(const QString & path, QWidget *p):baseWidget(p)
{
    setFixedSize(125,80);
    m_isenter=false;
    m_isselected=false;
    m_selePix=QPixmap(":/image/otherWidget/selected.png");
    m_pix=QPixmap(path).scaled(125,80);
    m_path=path;
}

void skinWidgetContentItem::setSelected(bool sel)
{
    m_isselected=sel;
    update();
}
void skinWidgetContentItem::setPixmap(const QString & path)
{
    m_pix=QPixmap(path).scaled(125,80);
    m_path=path;
    update();
}

void skinWidgetContentItem::loadFromPath(const QString& path)
{
    m_path=path;
    m_pix=QPixmap(path).scaled(125,80);
    update();
}

void skinWidgetContentItem::setMaskText(const QString &str)
{
    m_maskText=str;
}

void skinWidgetContentItem::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,width(),height(),m_pix);
    if(m_isselected)
        p.drawPixmap(105,60,17,17,m_selePix);
    if(m_isenter)
    {
        p.setBrush(QColor(55,55,55,100));
        p.drawRect(-1,-1,width()+1,height()+1);

        QFontMetrics metric(font());
        QFont font;
        font.setFamily("宋体");
        font.setPixelSize(13);
        p.setFont(font);
        p.setPen(Qt::white);
        p.drawText((width()-metric.width(m_maskText))/2,32,m_maskText);
        p.drawText((width()-metric.width("作者：严涛"))/2,46,"作者：严涛");
        p.drawText((width()-metric.width("250人已使用"))/2,60,"250人已使用");
    }

}

void skinWidgetContentItem::enterEvent(QEvent *)
{
    this->setCursor(Qt::PointingHandCursor);
    m_isenter=true;
    update();
}

void skinWidgetContentItem::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor);
    m_isenter=false;
    update();
}

void skinWidgetContentItem::mouseReleaseEvent(QMouseEvent *e)
{
    if(rect().contains(e->pos()))
    emit clicked();
}

