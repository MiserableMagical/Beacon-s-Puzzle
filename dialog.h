#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "QTime"
//#include "mylabel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    //static int hor,ver,phor,pver;
    void horchanging();
    void verchanging();
    void thumbchanging();
    void buzhidaojiaoshenme();
    void correction(int id,int x,int y);
    void winning();
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    void readImage();
    int tim;QTime seconds;
    bool won = 0;
private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
//int hor,ver,phor,pver;
