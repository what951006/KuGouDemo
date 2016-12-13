#ifndef MIDDLEWIDGETS_H
#define MIDDLEWIDGETS_H

#include <QObject>
#include <QWidget>
#include"baseWidget.h"

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
    void setInitParent(mainWindow*p);
    void setlistShowStyle();
    void setlistHideStyle();
    void setBtnShowHide(bool show=true);
    myPushButton* getPushButton(){return m_btnhidelist;}
    middleWidgetLeft *m_leftWid;
    middleWidgetRight *m_rightWid;
protected:
    void mousePressEvent(QMouseEvent *){setFocus();}
    void mouseReleaseEvent(QMouseEvent *){}
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void slot_btnclicked();
private:
    mainWindow *m_mainWindow;
    myPushButton *m_btnhidelist;
};

#endif // MIDDLEWIDGETS_H
