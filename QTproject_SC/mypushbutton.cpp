#include "mypushbutton.h"

mypushButton::mypushButton(QString normalImg, QString pressImg)
{
    //成员变量normalImgPath保存正常显示图片路径
    normalImgPath = normalImg;
    //成员变量pressedImgPath保存按下后显示的图片
    pressedImgPath = pressImg;
    //创建QPixmap对象
    QPixmap pixmap;
    //判断是否能够加载正常显示的图片，若不能提示加载失败
    bool ret = pixmap.load(normalImgPath);
    if(!ret)
    {
        qDebug() << normalImg << "加载图片失败!";
    }
    //设置图片的固定尺寸
    this->setFixedSize( pixmap.width(), pixmap.height() );
    //设置不规则图片的样式表
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pixmap);
    //设置图标大小
    this->setIconSize(QSize(pixmap.width(),pixmap.height()));


}

void mypushButton::Zoom1()
{
    //创建动画对象
    QPropertyAnimation * animation1 = new QPropertyAnimation(this,"geometry");
    //设置时间间隔，单位毫秒
    animation1->setDuration(200);
    //创建起始位置
    animation1->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //创建结束位置
    animation1->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置缓和曲线，QEasingCurve::OutBounce 为弹跳效果    animation1->setEasingCurve(QEasingCurve::OutBounce);
    //开始执行动画
    animation1->start();
}

void mypushButton::Zoom2()
{
    QPropertyAnimation * animation1 =  new QPropertyAnimation(this,"geometry");
    animation1->setDuration(200);
    animation1->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation1->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->start();
}

void mypushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressedImgPath!="")//传入选中后的图片不为空，需要选中后进行图片的切换
    {
        QPixmap pix;
        bool ret =pix.load(this->pressedImgPath);
        if(! ret)
        {
            qDebug()<<"图片加载失败";
            return;
        }
        //设定图片大小
        this->setFixedSize(pix.width(),pix.height());
        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");
        //设置图片
        this->setIcon(pix);
        //设置图片大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    //其他事情交给父类处理
    QPushButton::mousePressEvent(e);
}

void mypushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressedImgPath!="")//传入选中后的图片不为空，需要选中后进行图片的切换
    {
        QPixmap pix;
        bool ret =pix.load(this->normalImgPath);
        if(! ret)
        {
            qDebug()<<"图片加载失败";
            return;
        }
        //设定图片大小
        this->setFixedSize(pix.width(),pix.height());
        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");
        //设置图片
        this->setIcon(pix);
        //设置图片大小
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
    //其他事情交给父类处理
    QPushButton::mouseReleaseEvent(e);
}

