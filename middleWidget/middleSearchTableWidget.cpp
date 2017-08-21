#include "middleSearchTableWidget.h"
#include<QHeaderView>
#include<QScrollBar>
#include<QDebug>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QToolTip>
#include<QPainter>
#include<QRect>

#include"middleSearchWidget.h"
#include"middleLeftStackWidget0.h"
#include"myTablePlayListFinal.h"
#include"mainwindow.h"
#include"topwidgets.h"

tableCellWidget::tableCellWidget(QWidget *p)
    :baseWidget(p)
    ,m_btntableplay(this)
    ,m_btntableadd(this)
    ,m_btntabledownload(this)
{
    init();
}
void tableCellWidget::init()
{
    QHBoxLayout *hlyout=new QHBoxLayout;

    m_btntableplay.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_searchplay (1).png)}"
                                  "QPushButton:hover{border-image:url(:/image/middlewidget/btn_searchplay (2).png)}"
                                  "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_searchplay (3).png)}"
                                  "QPushButton:enable{border-image:url(:/image/middlewidget/btn_searchplay (4).png)}");
    m_btntableadd.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_searchadd (1).png)}"
                             "QPushButton:hover{border-image:url(:/image/middlewidget/btn_searchadd (2).png)}"
                             "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_searchadd (3).png)}"
                             "QPushButton:enable{border-image:url(:/image/middlewidget/btn_searchadd (4).png)}");
    m_btntabledownload.setStyleSheet("QPushButton{border:NULL;background-image:url(:/image/middlewidget/btn_downloadmini (1).png);}"
                                      "QPushButton:hover{border:NULL;background-image:url(:/image/middlewidget/btn_downloadmini (2).png);}"
                                      "QPushButton:pressed{border:NULL;background-image:url(:/image/middlewidget/btn_downloadmini (3).png);}");

    m_btntableplay.setFixedSize(16,16);
    m_btntableadd.setFixedSize(16,16);
    m_btntabledownload.setFixedSize(16,16);

    hlyout->addWidget(&m_btntableplay);
    hlyout->addWidget(&m_btntableadd);
    hlyout->addWidget(&m_btntabledownload);
    hlyout->setContentsMargins(0,0,0,0);
    setLayout(hlyout);
}

/////////////////////////////////////////////////////////////////////////
middleSearchTableWidget::middleSearchTableWidget(QWidget*p):QTableWidget(p),m_prebgItem(-1)
{
    init();
}
void middleSearchTableWidget::init()
{
    setMouseTracking(true);
    setFrameShadow(QFrame::Plain);//设置平的样式
    setFrameShape(QFrame::NoFrame);//设置无边样式
    setFocusPolicy(Qt::NoFocus);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//水平scrollbar禁用
 //   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//垂直scrollbar禁用
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    setShowGrid(false);//隐藏线
    this->horizontalHeader()->setVisible(false);
    this->verticalHeader()->setVisible(false);
    setEditTriggers(QTableWidget::NoEditTriggers);//涓嶅彲缂栬緫
    setSelectionBehavior (QAbstractItemView::SelectRows); //璁剧疆閫夋嫨琛屼负锛屼互琛屼负鍗曚綅
    setSelectionMode (QAbstractItemView::SingleSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);

    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    insertColumn(0);
    insertColumn(1);
    insertColumn(2);
    insertColumn(3);
    insertColumn(4);
    insertColumn(5);

    QHeaderView *header= this->horizontalHeader();
    header->resizeSection(0,13);
    header->resizeSection(1,(width()-143)/3);
    header->resizeSection(2,(width()-143)/3);
    header->resizeSection(3,(width()-143)/3);
    header->resizeSection(4,50);
    header->resizeSection(5,80);

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;margin: 0px 2px 0px 0px;}"
                                        "QScrollBar::handle{background:rgb(217,217,217);border-radius:4px;}"
                                        "QScrollBar::handle:hover{background: rgb(191,191,191);}"
                                        "QScrollBar::add-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                        "QScrollBar::sub-line:vertical{border:1px rgb(230,230,230);height: 1px;}"
                                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background:transparent;}");

    setStyleSheet("QTableWidget{color:rgb(88,88,88);}"
                  "QTableWidget::item{selection-color:rgb(61,61,61);}");//
    setAlternatingRowColors(true);

    connect(this,SIGNAL(cellEntered(int,int)),this,SLOT(slot_cellEnter(int,int)));
    connect(this,SIGNAL(cellClicked(int,int)),this,SLOT(slot_itemclick(int,int)));
    connect(this,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slot_doubleClicked(QTableWidgetItem*)));

}

void middleSearchTableWidget::setItem(int row, int column, QTableWidgetItem *item)
{
        switch (column) {
        case 0:
             {
                QCheckBox *box=new QCheckBox(item->text(),this);
                box->setStyleSheet("QCheckBox::indicator:checked{border-image:url(:/image/middlewidget/checked.png)}"
                                   "QCheckBox::indicator:unchecked{border-image:url(:/image/middlewidget/unchecked.png)}");
                box->setText("");
                setCellWidget(row,0,box);
                item->setText("");
                break;
             }
        case 1:
            {
                  item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                  break;
             }
        case 2:
             {
                    playingWidgetBtn *btn=new playingWidgetBtn;
                    btn->setNormalColor(QColor(88,88,88));
                    btn->setHoverColor(QColor(61,61,61));
                    btn->setText(item->text());
                    item->setText("");
                    setCellWidget(row,2,btn);
                    connect(btn,SIGNAL(clicked(bool)),this,SLOT(slot_tableContainerBtnClicked()));
                    break;
             }
        case 3:
            {
                 playingWidgetBtn *btn=new playingWidgetBtn;
                 btn->setNormalColor(QColor(88,88,88));
                 btn->setHoverColor(QColor(61,61,61));
                 btn->setText(item->text());
                 item->setText("");
                 setCellWidget(row,3,btn);
                 connect(btn,SIGNAL(clicked(bool)),this,SLOT(slot_tableContainerBtnClicked()));
                 break;
            }
        case 5:
            {
                 item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                 tableCellWidget *cellwid=new tableCellWidget(this);
                 connect(&cellwid->m_btntableplay,SIGNAL(clicked(bool)),this,SLOT(slot_playbtnclicked()));
                 connect(&cellwid->m_btntableadd,SIGNAL(clicked(bool)),this,SLOT(slot_addbtnclicked()));
                 connect(&cellwid->m_btntabledownload,SIGNAL(clicked(bool)),this,SLOT(slot_downloadbtnclicked()));
                 setCellWidget(row,5,cellwid);
                 break;
             }
        default:
            {
               item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
               break;
            }
        }
        QTableWidget::setItem(row,column,item);
}

void middleSearchTableWidget::removeRow(int row)
{
    for(int i=0;i<this->columnCount();i++)
    {
        QWidget*wid= cellWidget(row,i);
        if(wid)
        wid->deleteLater();
    }
    QTableWidget::removeRow(row);
}
void middleSearchTableWidget::slot_doubleClicked(QTableWidgetItem *it)
{
    middleSearchWidget*parent=(middleSearchWidget*)parentWidget();
    int row= it->row();
    const ItemResult &result= parent->GetItemByIndex(row);
    mainWindow::GetInstance()->middleStack0()->addMusicToDefaultList(result,1);
}

void middleSearchTableWidget::slot_itemclick(int row, int)
{
    QCheckBox *box=(QCheckBox*)cellWidget(row,0);
    if(box!=NULL)
      box->click();

}
void middleSearchTableWidget::leaveEvent(QEvent *e)
{
    QTableWidget::leaveEvent(e);
    slot_cellEnter(-1,0);
}

void middleSearchTableWidget::slot_tableContainerBtnClicked()
{
   playingWidgetBtn*btn= (playingWidgetBtn*)sender();
   if(!btn)
     return;
   mainWindow::GetInstance()->topWidget()->searchFromLineEdit(btn->text());
}


void middleSearchTableWidget::slot_playbtnclicked()
{
    middleSearchWidget*parent=(middleSearchWidget*)parentWidget();
    int index= indexAt(mapFromGlobal(QCursor::pos())).row();
    const ItemResult &result= parent->GetItemByIndex(index);
    mainWindow::GetInstance()->middleStack0()->addMusicToDefaultList(result,1);
}

void middleSearchTableWidget::slot_addbtnclicked()
{
    int index= indexAt(mapFromGlobal(QCursor::pos())).row();
    middleSearchWidget*parent=(middleSearchWidget*)parentWidget();
    const ItemResult &result= parent->GetItemByIndex(index);
    mainWindow::GetInstance()->middleStack0()->addMusicToDefaultList(result,0);
    QToolTip::showText(QCursor::pos(),"已添加至播放列表");
}

void middleSearchTableWidget::slot_downloadbtnclicked()
{

}
void middleSearchTableWidget::slot_cellEnter(int row, int c)
{
  /* if(item(m_prebgItem,c)!=Q_NULLPTR&&row!=m_prebgItem)
    {
        for(int i=0;i<columnCount();i++)
        item(m_prebgItem,i)->setBackgroundColor(Qt::transparent);
    }
    if(row!=m_prebgItem)
    {
        if(item(row,c)!=Q_NULLPTR)
        {
            for(int i=0;i<columnCount();i++)
            this->item(row,i)->setBackgroundColor(QColor(245,245,245));
        }
         m_prebgItem=row;
    }*/
}
