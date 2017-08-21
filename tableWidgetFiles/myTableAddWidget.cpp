#include "myTableAddWidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
myTableAddWidget::myTableAddWidget(QWidget*p)
    :baseWidget(p)
    ,m_add("添加本地歌曲",this)
    , m_addFolder("添加本地歌曲文件夹",this)
    ,m_labadd(this)
    ,m_labaddFolder(this)
{
    init();
    setFixedSize(310,80);
    connect(&m_labadd,SIGNAL(clicked(bool)),&m_add,SIGNAL(clicked(bool)));
    connect(&m_labaddFolder,SIGNAL(clicked(bool)),&m_addFolder,SIGNAL(clicked(bool)));
}
void myTableAddWidget::init()
{
    QHBoxLayout *hlyout=new QHBoxLayout;
    QHBoxLayout *hlyout1=new QHBoxLayout;
    QVBoxLayout *vlyout=new QVBoxLayout;

   m_labadd.setFixedSize(16,16);
   m_labaddFolder.setFixedSize(16,16);

   m_labadd.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/lab_add.png);}");
   m_labaddFolder.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/lab_addfolder.png);}");

   m_add.adjustSize();
   m_addFolder.adjustSize();

   m_add.setStyleSheet("QPushButton{color: rgb(58,58,58);text-decoration: underline;}");
   m_addFolder.setStyleSheet("QPushButton{color: rgb(58,58,58);text-decoration: underline;}");


   hlyout->addStretch(20);
   hlyout->addWidget(&m_labadd);
   hlyout->addWidget(&m_add);
   hlyout->addStretch(20);
   hlyout->setSpacing(5);
   hlyout->setContentsMargins(0,0,0,0);

   hlyout1->addStretch(20);
   hlyout1->addWidget(&m_labaddFolder);
   hlyout1->addWidget(&m_addFolder);
   hlyout1->addStretch(20);
   hlyout1->setSpacing(5);
   hlyout1->setContentsMargins(0,0,0,0);

   vlyout->addLayout(hlyout);
   vlyout->addLayout(hlyout1);
   vlyout->setSpacing(5);
   vlyout->setContentsMargins(0,0,0,0);
   setLayout(vlyout);

}
