#ifndef PUSHBUTTONONTABLE_H
#define PUSHBUTTONONTABLE_H

#include <QObject>
#include <QWidget>
#include"myPushButton.h"
class pushButtonCrossWidget : public QWidget
{
    Q_OBJECT
public:
    explicit pushButtonCrossWidget(QWidget *parent = 0);
    myPushButton m_btn;
signals:

public slots:
};
//////////////////////////////////////////////////////
class pushButtonGroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit pushButtonGroupWidget(QWidget *parent = 0);
    myPushButton m_btnMovie;
    myPushButton m_btnLove;
    myPushButton m_btnDel;
    myPushButton m_btnMore;
    void setLoved();
    bool isLoved(){return m_islove;}
signals:

public slots:
    void slot_btnloveclicked();
private:
    bool m_islove;
};
#endif // PUSHBUTTONONTABLE_H
