#ifndef TEXTMOVINGWIDGET_H
#define TEXTMOVINGWIDGET_H

#include <QWidget>
#include<QTimer>


class TextMovingWidget : public QWidget
{
    Q_OBJECT

public:
    TextMovingWidget(const QString str="" ,QWidget *parent = 0);
    ~TextMovingWidget();
    void setText(const QString&);
    QString Text(){return _text;}
protected:
    virtual void paintEvent(QPaintEvent*);
    int getDirection(const QString&);
    virtual void showEvent(QShowEvent *);
private slots:
    void slot_UpdateTextGeometry();
private:
    QTimer *_timer;
    QString _text;
    int m_direction; //zero means left ,one means right;
    int m_x;
};

#endif // TEXTMOVINGWIDGET_H
