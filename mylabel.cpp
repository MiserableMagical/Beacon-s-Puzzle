#include "mylabel.h"
#include "QMouseEvent"
//#include "dialog.h"

//mylabel::mylabel(int ) {}

//void mylabel::released(int id,int x,int y){}
extern int hor,ver,phor,pver;
extern bool occupied[66][66];
extern int num_cor;
//extern void Dialog::winning();
//void mylabel::winning_(){}
void mylabel::mouseMoveEvent(QMouseEvent* event)
{
    this->raise();
    if (event->buttons() & Qt::LeftButton)  //进行的按位与（只有左键点击移动才满足）
    {
        auto ret = event->scenePosition();
        this->move(ret.x()-stx,ret.y()-sty);
    }

}
void mylabel::mousePressEvent(QMouseEvent* event)
{
    //setText("Ok, mouse is pressed");
    stx=(event->pos()).x();sty = (event->pos()).y();
    if(occx != -1) {
        if(occy * hor + occx == id) num_cor--;
        occupied[occx][occy] = false;
        occx = occy = -1;
    }
}
void mylabel::mouseReleaseEvent(QMouseEvent* event)
{
    auto ret = event->scenePosition();
    int h = phor, v = pver;
    int xpos = (ret.x() - 50 + h) / h - 1, ypos = (ret.y() - 50 + v) / v - 1;
    if(0 <= xpos && xpos < hor && 0 <= ypos && ypos < ver) {
        int ex_x = 50 + xpos * h, ex_y = 50 + ypos * v;
        if(!occupied[xpos][ypos]) {
            occupied[xpos][ypos] = true;
            occx = xpos;
            occy = ypos;
            if(occy * hor + occx == id) {
                num_cor++;
                qDebug()<<num_cor;

            }
            this->move(ex_x,ex_y);
            if(num_cor == hor * ver) {
                emit winning_();
                //Dialog::winning();
            }
        }
        //this->move(ex_x,ex_y);
    }
    //emit released(id,ret.x(),ret.y());
}
