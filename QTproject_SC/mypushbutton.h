#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include<QDebug>
#include<QPropertyAnimation>

class mypushButton:public QPushButton
{   Q_OBJECT
public:
    //explicit mypushButton(QWidget *parent=0);

    //normalImg 代表正常显示的图片
    //pressImg  代表按下后显示的图片，默认为空
    mypushButton(QString normalImg,QString pressImg = "");

    QString normalImgPath;  //默认显示图片路径
    QString pressedImgPath; //按下后显示图片路径


    //向下跳跃
    void Zoom1();
    //向上跳跃
    void Zoom2();

//    //鼠标按下事件
    void mousePressEvent(QMouseEvent *e);
//    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *e);


signals:

public slots:

};

#endif // MYPUSHBUTTON_H
