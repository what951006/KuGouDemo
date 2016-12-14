#include "middleLeftStackWidget4.h"
#include"qlabel.h"
#include<QVBoxLayout>
middleLeftStackWidget4::middleLeftStackWidget4(QWidget*parent):baseWidget(parent)
{
     setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    init();
}
void middleLeftStackWidget4::init()
{
    QVBoxLayout *vLyout=new QVBoxLayout;
    m_wid=new buttonStackedWidget(this);
    m_wid->addPushButton("我的下载");
    m_wid->addPushButton("本地导入");
    m_wid->showLayout();
    m_wid->m_stackwid->addWidget(new QWidget());
    m_wid->m_stackwid->addWidget(new QWidget());

    vLyout->addWidget(m_wid);
    vLyout->setContentsMargins(0,0,0,0);
    setLayout(vLyout);

}
