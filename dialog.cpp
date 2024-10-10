#include "dialog.h"
#include "ui_dialog.h"
#include "QMessageBox"
#include "QFileDialog"
#include "QFile"
#include "QImage"
#include "QPixmap"
#include "QDebug"
#include "mylabel.h"
#include "QMouseEvent"
#include "random"
#include "QTimer"
#include "QPainter"

QPixmap * qwq = nullptr;
QImage latest_img;

extern int hor,ver,phor,pver;
bool in_window,in_game,isthumb = true;
void Dialog::horchanging(){
    int ret = ui->spinBox_1->value();
    hor = ret;
    qDebug()<<ret;
}
void Dialog::verchanging(){
    int ret = ui->spinBox_2->value();
    ver = ret;
    qDebug()<<ret;
}
void Dialog::thumbchanging(){
    isthumb = ui->checkBox->isChecked();
}
void Dialog::paintEvent(QPaintEvent *event){
    QPainter paint(this),paintback(this);
    paintback.setOpacity(0.11);
    if(in_window) paintback.drawPixmap(rect(),QPixmap(":/pic/wallpaper.jpg"));
    QPen pen;
    if(in_game) {
        pen.setWidth(2);
        pen.setColor((QColor){70,70,70});
        int v = ver, h = hor, ph = phor, pv = pver;
        if(v * h > 10) pen.setWidth(1);
        paint.setPen(pen);
        for(int i = 0; i < v; i++) {
            for(int j = 0;j < h;j++) {
                QRect cur = (QRect){ph * j + 50, pv * i + 50,  ph, pv};
                paint.drawRect(cur);
            }
        }
        paint.drawRect(50, 50,  ph * h, pv * v);
    }
}
mylabel* Parts[66][66];
//x --- h   y --- v
std::mt19937 mt(time(0));
int getrnd(){return abs((int)mt());}
int getbetween(int l,int r){return l + getrnd() % (r - l + 1);}
int hor=2,ver=2,phor,pver;
bool occupied[66][66];
int num_cor;
QLabel *showt = nullptr;
const int WMax = 1150, HMax = 800;
//void mylabel::released(int id,int x,int y){}
/*void Dialog::winning(){
    qDebug()<<"666";
    QMessageBox::information(this, "title", "图片读取失败！");
}*/
void Dialog::winning(){
    if(won) return;
    won = true;
    qDebug()<<"666";
    QString info = "Puzzle Completed!\nElapsed time : " + seconds.toString("mm:ss");
    QMessageBox::information(this, "You won", info);
}
void Dialog::timerEvent(QTimerEvent *event){
    if(event->timerId() == tim && !won) {
        seconds = seconds.addSecs(1);
        showt->setText(seconds.toString("mm:ss"));
        showt->setFont(QFont("Arial",15));
    }
}
void Dialog::buzhidaojiaoshenme(){
    QImage timg = latest_img;
    //QTimer *timer = new QTimer;
    in_game = true;
    repaint();
    //this->resize(1440,900);
    ui->label->hide();
    ui->btn->hide();
    ui->btn_2->hide();
    ui->spinBox_1->hide();
    ui->spinBox_2->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->checkBox->hide();
    qDebug()<<timg.width()<<" "<<timg.height();
    //while(timg.height() > 800) timg = timg.scaled((QSize){timg.width()/2,timg.height()/2});
    double rt1 = (double)timg.height() / HMax, rt2 = (double)timg.width() / WMax;int tmp1 = 0, tmp2 = 0;
    if(rt1 < 1 && rt2 < 1) {
        tmp1 = timg.width(), tmp2 = timg.height();
    }
    else if(rt1 > rt2) {
        tmp1 = int(WMax*rt2/rt1), tmp2 = HMax;
    }
    else {
        tmp1 = WMax, tmp2 = int(HMax*rt1/rt2);
    }
    timg = timg.scaled((QSize){tmp1,tmp2},Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    int DWidth = 290 + tmp1, DHeight = 100 + tmp2;
    QImage parts[66][66];
    int h = hor, v = ver;
    int ph = timg.width() / h, pv = timg.height() / v;
    phor = ph, pver = pv;
    qDebug()<<timg.width()<<" "<<timg.height()<<" "<<ph<<" "<<pv;
    this->resize(DWidth,DHeight);
    for(int i = 0; i < v; i++) {
        for(int j = 0;j < h;j++) {
            parts[i][j] = timg.copy((QRect){ph * j,pv * i,  ph,pv});
        }
    }
    for(int i = 0; i < v; i++) {
        for(int j = 0;j < h;j++) {
            QPixmap tmp = QPixmap::fromImage(parts[i][j]);
            Parts[i][j] = new mylabel(i * h + j);
            Parts[i][j] -> setParent(this);
            Parts[i][j] -> move(getbetween(20,200),getbetween(50,DHeight-50-pv));
            Parts[i][j] -> setFixedSize(ph,pv);
            Parts[i][j] -> setScaledContents(true);
            Parts[i][j] -> setPixmap(tmp);
            Parts[i][j] -> show();
            //connect(tmp_[i][j],SIGNAL(released(int,int,int)),this,SLOT(Dialog::correction(int,int,int)));
            connect(Parts[i][j],&mylabel::winning_,this,&Dialog::winning);
            //connect(tmp_[i][j],SIGNAL(mylabel::winning()),this,SLOT(Dialog::winning()));
        }
    }

    /*create scaled image*/
    if(isthumb) {
        int thumb_size = 200;
        if((long long)tmp1 * tmp2 < 300000ll) thumb_size = 100;
        QLabel *scaled = new QLabel;
        scaled->setParent(this);
        scaled->move(DWidth-125-thumb_size/2,50);
        //scaled->setFixedSize(200,200);
        if(timg.height() > timg.width()) scaled->setFixedSize(ceil((double)timg.width() * thumb_size / timg.height()),thumb_size);
        else scaled->setFixedSize(thumb_size,ceil((double)timg.height() * thumb_size / timg.width()));
        scaled->setPixmap(QPixmap::fromImage(timg));
        scaled->setScaledContents(true);
        scaled->show();
    }

    /*create timer*/
    tim = startTimer(1000);
    seconds.setHMS(0,0,0);
    showt->setParent(this);
    showt->move(DWidth-138,15);
    //showt->adjustSize();
    showt->resize(50,20);
    showt->show();
}
bool FirstConnected = true;
void Dialog::readImage(){
    QString curpath = QDir::currentPath();
    QString fname = QFileDialog::getOpenFileName(this,"选择一张图片...",curpath,"图片文件(*.jpg *.gif *.png)");
    QFile file(fname);
    QPixmap img(fname);

    if(img.height() == 0) {
        QMessageBox::information(this, "提示", "图片读取失败！");
        return;
    }

    if(img.height() > img.width())
    {
        ui->label->setFixedSize(ceil((double)img.width() * 200 / img.height()),200);
    }
    else {
        ui->label->setFixedSize(200,ceil((double)img.height() * 200 / img.width()));
    }
    ui->label->setPixmap(img);
    ui->label->setScaledContents(true);
    ui->label->show();

    latest_img = img.toImage();
    if(!FirstConnected) return;
    FirstConnected = false;
    ui->btn->show();
    connect(ui->btn,&QPushButton::clicked,this,&Dialog::buzhidaojiaoshenme);
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->label->hide();
    ui->btn->hide();
    //ui->checkBox->hide();
    connect(ui->btn_2,&QPushButton::clicked,this,&Dialog::readImage);
    showt = new QLabel;

    //read the image
    in_window = true;
    repaint();
    this->resize(800,600);
    this->setWindowTitle("Beacon's Puzzle");
    this->setPalette(QPalette(Qt::white));

    //qDebug()<<(img.height());
    //qDebug()<<(img.width());//成功读取图片
    //connect(ui->btn,&QPushButton::clicked,this,[img,this](){slt(img);});
    QString styleSheet = "QCheckBox{ spacing:5px; }";
    ui->checkBox->setStyleSheet(styleSheet);


    connect(ui->checkBox,&QCheckBox::stateChanged,this,&Dialog::thumbchanging);
    connect(ui->spinBox_1,&QSpinBox::valueChanged,this,&Dialog::horchanging);
    connect(ui->spinBox_2,&QSpinBox::valueChanged,this,&Dialog::verchanging);
}

Dialog::~Dialog()
{
    delete ui;
}
