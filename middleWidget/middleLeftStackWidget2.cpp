#include "middleLeftStackWidget2.h"
#include"qlabel.h"
middleLeftStackWidget2::middleLeftStackWidget2(QWidget*parent)
    :baseWidget(parent)
    ,m_Wid(this)
{
      QVBoxLayout *vLyout=new QVBoxLayout;

      m_Wid.addPushButton("音乐电台");
      m_Wid.addPushButton("MV电台");
      m_Wid.showLayout();
      m_Wid.addWidget(new QWidget());
      m_Wid.addWidget(new QWidget());


      vLyout->addWidget(&m_Wid);
      vLyout->setContentsMargins(0,0,0,0);
      setLayout(vLyout);
}
