#ifndef MIDDLEWIDGETS_H
#define MIDDLEWIDGETS_H

#include <QObject>
#include <QWidget>
#include "baseWidget.h"
#include "middlewidgetleft.h"
#include "middleWidgetRight.h"

class middleWidgetLeft;
class middleWidgetRight;
class mainWindow;
class myPushButton;

class middleWidgets : public baseWidget
{
    Q_OBJECT
public:
    explicit middleWidgets(QWidget *parent = 0);
    void initLayout();
    void setlistShowStyle();
    void setlistHideStyle();
    void setBtnShowHide(bool show=true);

    middleWidgetLeft m_leftWid;
    middleWidgetRight m_rightWid;
    myPushButton m_btnhidelist;
protected:
    void mousePressEvent(QMouseEvent *){setFocus();}
    void mouseReleaseEvent(QMouseEvent *){}
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void slot_btnclicked();
private:

};

#endif // MIDDLEWIDGETS_H
