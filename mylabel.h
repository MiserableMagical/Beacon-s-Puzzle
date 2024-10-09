#ifndef MYLABEL_H
#define MYLABEL_H

#include "qlabel.h"

class mylabel : public QLabel
{
    Q_OBJECT

public:
    int id;
    mylabel(int idx = 0){id = idx;}
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    int stx,sty,occx=-1,occy=-1;
signals:
    //void released(int id,int x,int y);
    void winning_();
};

#endif // MYLABEL_H
