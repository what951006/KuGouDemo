#ifndef SKINWIDGET_H
#define SKINWIDGET_H

#include <QObject>
#include <QSignalMapper>
#include <QDebug>

#include "basewindow.h"
#include "baseWidget.h"
#include "baseDialog.h"
#include "myPushButton.h"
#include "skinWidgetSliderWidget.h"
#include "middleLeftStackWidget0.h"

class skinWidget;
class skinWidgetContentItem:public baseWidget
{
    Q_OBJECT
public:
    explicit skinWidgetContentItem(QWidget*p=0);
    explicit skinWidgetContentItem(const QString &path,QWidget*p=0);
    void setSelected(bool sel=false);
    void setPixmap(const QString &);
    void loadFromPath(const QString& path);
    void setMaskText(const QString&);
   const QString PixPath(){return m_path;}
protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);


    void mouseReleaseEvent(QMouseEvent *);
signals:
    void clicked();
private:

    QPixmap m_pix;
    QString m_maskText;
    bool m_isenter;
    bool m_isselected;
    QString m_path;
    QPixmap m_selePix=QPixmap("");
};

class skinContentWidget:public baseWidget
{
  Q_OBJECT
public:
   explicit skinContentWidget(QWidget*p=0);
   void setSkinWidget(skinWidget*);
   void addSkin(skinWidgetContentItem*);
   void loadFromDir(const QString &dir);
    void setDefaultSkin();
    void UpdateSkin();
   QSignalMapper *signalMapper(){return m_sigmap;}

public slots:
    void slot_itemClicked();
signals:
    void sig_setBackground(QString);

private:
   QVector<skinWidgetContentItem*> m_vector;
   skinWidget *m_skinWid;
   QSignalMapper *m_sigmap;
};
////////////////////////////////////////////////////
class skinWidget : public baseDialog
{
    Q_OBJECT
public:
    explicit skinWidget(QWidget *parent = 0);
    void initTopLayout();
    QSignalMapper *signalMapper(){return m_skincontwid.signalMapper();}
    void setdefaultSkin();
    void initBottomLayout();

    myScrollArea scrollArea;
    myPushButton m_btnAdjustWindow;
    myPushButton m_btnOpacity;
    skinContentWidget m_skincontwid;
    skinWidgetSliderWidget m_sliderWidget;

public slots:
    void slot_openPersonalBG();
    void slot_setOpacityText(int);

signals:

public slots:

};

#endif // SKINWIDGET_H
