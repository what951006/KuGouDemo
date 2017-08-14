#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QObject>
#include <QWidget>
#include<QPushButton>
#include<QEvent>
#include<QList>
#include<QMouseEvent>
#include<QTimer>
#include<QSlider>

#include<myMediaList.h>
class myPushButton : public QPushButton
{
    Q_OBJECT
public:
    myPushButton(QWidget*parent=0);
    myPushButton(const QString& str,QWidget*parent=0);
};
///////////////////////////////////////////////////////////////////
class volButton:public QPushButton
{
    Q_OBJECT
public:
    volButton(const QString& normal,const QString& hover,const QString& pressed,QWidget*parent=0);
    void setParentSlider(QSlider* slider){m_partnerslider=slider;}
protected:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *e)override;
private:
    bool m_isenter;
    int m_savevol;
    int m_isvolempty;
    int m_index;
    QVector<QPixmap> m_listnormal;
    QVector<QPixmap> m_listhover;
    QVector<QPixmap> m_listpressed;

    QSlider *m_partnerslider;
public slots:
    void setButtonPixmap(int);//getFromSlider
signals:
    void setMute(int);
};

class volButton2:public QPushButton
{
    Q_OBJECT
public:
    volButton2(const QString& normal,const QString& hover,const QString& pressed,QWidget*parent=0);
    void setPartnerSlider(QSlider *slider){m_partnerslider=slider;}
    QTimer m_timer;
public slots:

protected:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e)override;
    void mouseReleaseEvent(QMouseEvent *e)override;

private:
    QSlider *m_partnerslider;
    bool m_isenter;
    int m_savevol;
    int m_isvolempty;
    int m_index;
    QVector<QPixmap> m_listnormal;
    QVector<QPixmap> m_listhover;
    QVector<QPixmap> m_listpressed;


public slots:
    void setButtonPixmap(int);//getFromSlider
signals:
    void setMute(int);
    void sig_hideVolWidget();
};
////////////////////////////////////////////////////////////////////////////////////////
class stackButton:public QPushButton
{
    Q_OBJECT
public:
    explicit stackButton(const QString& pixnormal,const QString& pixhover,const QString& pixsel,QWidget*parent);
    void setselected(bool=true);
protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
private:
    QPixmap m_pixnormal;
    QPixmap m_pixhover;
    QPixmap m_pixselected;

    int m_index;
    bool m_enter;
    bool m_pressed;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class leftPixButton:public QPushButton
{
    Q_OBJECT
public:
    explicit leftPixButton(QString pixnormal,QString pixhover,QWidget*parent);
protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
private:
    bool m_enter;
    bool m_pressed;
    QPixmap m_pixnormal;
    QPixmap m_pixhover;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class playModeButton:public QPushButton
{
    Q_OBJECT
public:
    explicit playModeButton(QString pixnormal,QString pixhover,QString text,QWidget*parent);
    void setCheckedBtn(bool check);
    void setPlayMode(PlayMode mode){m_mode=mode;}
    PlayMode& playMode(){return m_mode;}
protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

    void paintEvent(QPaintEvent *);
private:
    bool m_enter;
    bool m_pressed;
    PlayMode m_mode;
    QPixmap m_pixnormal;
    QPixmap m_pixhover;
    QString m_text;
};

class playingWidgetBtn :public myPushButton
{
 Q_OBJECT
public:
    explicit playingWidgetBtn(QWidget*p=0);
    void setNormalColor(const QColor &color);
    void setHoverColor(const QColor &color);
    void adjustSize();
    void setText(const QString &text);
    QString text(){return m_text;}

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void leaveEvent(QEvent*);
private:
    QString m_text;
    QColor m_normalcolor;
    QColor m_hovercolor;
    bool m_isenter;
};

#endif // MYPUSHBUTTON_H
