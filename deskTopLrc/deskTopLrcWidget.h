#ifndef DESKTOPLRCWIDGET_H
#define DESKTOPLRCWIDGET_H

#include <QWidget>
#include<QTimer>
#include"baseDialog.h"
#include"myPushButton.h"
#include"AbsFrameLessAutoSize.h"

class deskTopLrcWidget : public AbsFrameLessAutoSize
{
    Q_OBJECT

public:
    deskTopLrcWidget(QWidget *parent = 0);
    ~deskTopLrcWidget();
    void initWidgetMISC();
    void setCurrentAndNextLrc(const QString &,const QString &);
    QString getCurrentLrc()const;
    void setOriginalStatus();
public slots:
    void slot_currentLrcChange(const QString&,const QString&);
    void slot_precentChange(const QString&,float&,int&interval);
private slots:

protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent *);
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent *);
    virtual void mouseMoveEvent(QMouseEvent*);

private:
    myPushButton *m_closeBtn;
    QWidget*m_topWid;

    float m_precent;
    float m_lrcMaskWidth;

    QString m_Text1;
    QString m_Text2;
    bool m_isDrawTop; //linearGradient will draw on the text base on this value;
    bool m_isDrawBrush;
    bool m_isOriginalState;

    QLinearGradient m_maskGradient;
    QLinearGradient m_normalGradient;
};

#endif // DESKTOPLRCWIDGET_H
