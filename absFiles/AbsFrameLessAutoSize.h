#ifndef ABSFRAMELESSAUTOSIZE_H
#define ABSFRAMELESSAUTOSIZE_H

#include <QWidget>
#include<QMouseEvent>
#include"baseWidget.h"

enum CursorPos{Default,Right,Left,Bottom,Top,TopRight,TopLeft,BottomRight,BottomLeft};
struct pressWindowsState
{
    bool    MousePressed;
    bool   IsPressBorder;
    QPoint  MousePos;
    QPoint  WindowPos;
    QSize PressedSize;
};

class AbsFrameLessAutoSize : public QWidget
{
    Q_OBJECT
public:
    AbsFrameLessAutoSize(QWidget *parent = 0);
    virtual~AbsFrameLessAutoSize(){}
    inline void setBorder(int border);
    void mouseMoveRect(const QPoint &p);
protected:
    virtual void mouseMoveEvent(QMouseEvent *event); //交给别个调用
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    pressWindowsState m_state;
    int m_border;
    CursorPos m_curPos;
};

#endif // ABSFRAMELESSAUTOSIZE_H
