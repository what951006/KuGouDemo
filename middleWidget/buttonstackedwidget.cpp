#include "buttonstackedwidget.h"
#include<QPainter>
#include<QDebug>
buttonStackedWidget::buttonStackedWidget(QWidget*parent):baseWidget(parent)
{
    init();
}
void buttonStackedWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QColor(230,230,230));
    p.drawLine(QPoint(0,40),QPoint(width(),40));
}

void buttonStackedWidget::init()
{
    m_hlyout=new QHBoxLayout;
    m_hlyout->setSpacing(0);
    m_hlyout->setContentsMargins(0,0,0,0);
    m_vlyout=new QVBoxLayout;
    m_stackwid=new QStackedWidget(this);
    m_stackwid->setCurrentIndex(0);
    connect(m_stackwid,SIGNAL(currentChanged(int)),this,SLOT(curWidIndexChange(int)));
}

void buttonStackedWidget::curWidIndexChange(int index)
{
    for(int i=0;i<m_btnList.count();i++)
    {
        if(i==index)
        {
            m_btnList.at(i)->setStyleSheet("color:rgb(40,143,231);font-size:14px;");
        }
        else
        {
            m_btnList.at(i)->setStyleSheet("color:rgb(68,68,68);font-size:14px;");
        }
    }
}

void buttonStackedWidget::addPushButton(const QString& name)//先删除再添加
{
    myPushButton *m_btn=new myPushButton(this);
    m_btn->setFixedHeight(40);
    m_btn->setText(name);
    m_btn->setStyleSheet("color:rgb(68,68,68);font-size:14px;");
    m_btnList<<m_btn;//添加进容器
    m_hlyout->addWidget(m_btn);
    delete &m_btn;
    m_btn=NULL;
}
void buttonStackedWidget::showLayout()
{
    for(int i=0;i<m_btnList.count();i++)//建立槽
    {
        m_btnList.at(i)->setObjectName(QString::number(i));//目标名
        connect(m_btnList.at(i),SIGNAL(clicked()),this,SLOT(buttonClicked()));
    }

    m_vlyout->addLayout(m_hlyout);
    m_vlyout->addWidget(m_stackwid);
    m_vlyout->setSpacing(0);
    m_vlyout->setContentsMargins(0,0,0,0);
    setLayout(m_vlyout);
}
void buttonStackedWidget::buttonClicked()
{
    m_stackwid->setCurrentIndex(sender()->objectName().toInt());
}
