#include "mainscene.h"
#include "ui_mainscene.h"
#include"mypushbutton.h"



mainscene::mainscene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainscene)
{
    ui->setupUi(this);

    ///-------------------BGM播放-----------------------
    QMediaPlayer *BGM = new QMediaPlayer();
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/res/BGM.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    BGM->setPlaylist(playlist);
    BGM->setVolume(50);
    BGM->play();



    ///------------------窗体设计----------------
    //设置固定大小
    this->setFixedSize(588,520);
    //设置应用图片
    this->setWindowIcon(QPixmap(":/res/rect001.png"));
    //设置窗口标题
    this->setWindowTitle("俄罗斯方块");
    //点击退出，退出程序
    connect(ui->actionnew,&QAction::triggered,[=](){this->close();});


    ///------------------多场景交互-----------------
    //场景初始化
    playScene=new PlayScene;
    playScene->stoptimer();
    playScene_llk=new playscene_llk;


    //监听普通模式场景中的返回按钮信号
    connect(playScene,&PlayScene::playSceneback,[=](){
        //普通模式场景隐藏
        playScene->stoptimer();
        playScene->hide();

        //自身的位置同步为 普通模式场景的位置
        this->setGeometry(playScene->geometry());

        //自身显示
        this->show();
    });

    //监听挑战模式场景中的返回按钮信号
    connect(playScene_llk,&playscene_llk::playSceneback,[=](){
        //挑战模式场景隐藏
        playScene_llk->hide();

        //自身的位置同步为 选择挑战模式场景的位置
        this->setGeometry(playScene_llk->geometry());
        //自身显示
        this->show();
    });

    ///------------自定义按钮控件-------------------
    //普通模式按钮
    mypushButton * startBtn1 = new mypushButton(":/res/Easy.png");
    startBtn1->setParent(this);
    startBtn1->move(this->width()*0.5-startBtn1->width()*0.5,this->height()*0.3);
    //挑战模式按钮
    mypushButton * startBtn2 = new mypushButton(":/res/Medium btn.png");
    startBtn2->setParent(this);
    startBtn2->move(this->width()*0.5-startBtn2->width()*0.5,this->height()*0.5);
    //退出按钮
    mypushButton * startBtn3 = new mypushButton(":/res/Hard btn.png");
    startBtn3->setParent(this);
    startBtn3->move(this->width()*0.5-startBtn3->width()*0.5,this->height()*0.7);

    //游戏介绍控件
    this->flag=0;
    QLabel* winLabel1 = new QLabel;
    QPixmap tmpPix1;
    tmpPix1.load(":/res/Level Cleared_gaitubao_402x44102.png");
    winLabel1->setGeometry(0,0,tmpPix1.width(),tmpPix1.height());
    winLabel1->setPixmap(tmpPix1);
    winLabel1->setParent(this);
    winLabel1->move( (this->width() - tmpPix1.width())*0.5 , -tmpPix1.height()-200);

    //排行榜控件
    QLabel* winLabel2 = new QLabel;
    QPixmap tmpPix2;
    tmpPix2.load(":/res/Level Clearedpaihang.png");
    winLabel2->setGeometry(0,0,tmpPix2.width(),tmpPix2.height());
    winLabel2->setPixmap(tmpPix2);
    winLabel2->setParent(this);
    winLabel2->move( (this->width() - tmpPix2.width())*0.5 , -tmpPix2.height()-200);

    //开发者控件
    QLabel* winLabel3 = new QLabel;
    QPixmap tmpPix3;
    tmpPix3.load(":/res/Level Clearedzuozhe_gaitubao_402x441.png");
    winLabel3->setGeometry(0,0,tmpPix3.width(),tmpPix3.height());
    winLabel3->setPixmap(tmpPix3);
    winLabel3->setParent(this);
    winLabel3->move( (this->width() - tmpPix3.width())*0.5 , -tmpPix3.height()-200);

    //帮助按钮
    mypushButton * helpbtn = new mypushButton(":/res/Help btn.png");
    helpbtn->setParent(this);
    helpbtn->move(15,30);

    //音乐按钮
    mypushButton * musicbtn = new mypushButton(":/res/Music btn.png");
    musicbtn->setParent(this);
    musicbtn->move(65,30);

    //排行榜按钮
    mypushButton * leaderbtn = new mypushButton(":/res/Leaderboard btn.png");
    leaderbtn->setParent(this);
    leaderbtn->move(this->width()-50,30);

    //信息按钮
    mypushButton * infobtn = new mypushButton(":/res/Info btn.png");
    infobtn->setParent(this);
    infobtn->move(15,this->height()-60);

    ///----------------排行榜文件的读取------------------
    textEdit = new QTextEdit(this);
    textEdit->setWindowFlags(Qt::WindowStaysOnTopHint);
    textEdit->setGeometry(QRect(200, -400, 400, 300));
    QTimer *timer = new QTimer(this);


    connect(timer,&QTimer::timeout,[=](){


    });
    timer->start(1000);


    ///----------------按钮特效---------------
    //监听点击事件，执行特效
    connect(startBtn1,&mypushButton::clicked,[=](){
        playsound();
        startBtn1->Zoom1();
        startBtn1->Zoom2();

        //延时 进入到普通模式场景
        QTimer::singleShot(500,this,[=](){
            //将自身隐藏调用

            this->hide();
            playScene->InitGame();

            //设置普通模式场景位置
            playScene->setGeometry(this->geometry());

            //进入到普通模式场景中
            playScene->show();
        });

    });
    connect(startBtn2,&mypushButton::clicked,[=](){
        playsound();
        startBtn2->Zoom1(); //向下跳跃
        startBtn2->Zoom2(); //向上跳跃
        //延时 进入到挑战模式场景
        QTimer::singleShot(500,this,[=](){
            //将自身隐藏调用

            this->hide();
            playScene->close();
            playScene_llk->InitGame_llk();

            //设置挑战模式场景位置
            playScene_llk->setGeometry(this->geometry());

            //进入到挑战模式场景中
            playScene_llk->show();
        });
    });
    connect(startBtn3,&mypushButton::clicked,[=](){
        playsound();
        startBtn3->Zoom1(); //向下跳跃
        startBtn3->Zoom2(); //向上跳跃
        QTimer::singleShot(1000,this,[=](){
            close();
        });

    });
    connect(helpbtn,&mypushButton::clicked,[=](){
        playsound();
        helpbtn->Zoom1(); //向下跳跃
        helpbtn->Zoom2(); //向上跳跃
        if(flag==false){
            QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel1,"geometry");
            animation1->setDuration(1000);
            animation1->setStartValue(QRect(winLabel1->x(),winLabel1->y(),winLabel1->width(),winLabel1->height()));
            animation1->setEndValue(QRect(winLabel1->x(),winLabel1->y()+684,winLabel1->width(),winLabel1->height()));
            animation1->setEasingCurve(QEasingCurve::OutBounce);
            animation1->start();
            this->flag=true;
        }
        else{
            QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel1,"geometry");
            animation1->setDuration(1000);
            animation1->setStartValue(QRect(winLabel1->x(),winLabel1->y(),winLabel1->width(),winLabel1->height()));
            animation1->setEndValue(QRect(winLabel1->x(),winLabel1->y()-684,winLabel1->width(),winLabel1->height()));
            animation1->setEasingCurve(QEasingCurve::OutBounce);
            animation1->start();
            this->flag=false;
        }
    });
    connect(musicbtn,&mypushButton::clicked,[=](){
        playsound();
        musicbtn->Zoom1(); //向下跳跃
        musicbtn->Zoom2(); //向上跳跃

    });
    connect(leaderbtn,&mypushButton::clicked,[=](){
        playsound();
        leaderbtn->Zoom1(); //向下跳跃
        leaderbtn->Zoom2(); //向上跳跃

        QString displayString;

        QFile file("D:/QT/QT FILE/QT LEARNING/Qt_Tetrix_3/res/ranking.txt");//scorerank
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        //按行读取
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            //qDebug()<< str;
            displayString.append(str);
        }

        //设置字体颜色
        textEdit->setTextColor(QColor("brown"));
        //设置文本框背景
        QPalette pl = textEdit->palette();
        pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
        textEdit->setPalette(pl);

        textEdit->setStyleSheet("border: none;");

        //设置字体大小
        textEdit->setFontPointSize(20);
        //设置字体加粗
        textEdit-> setFontWeight(75);

        //设置行高
        QTextCursor textCursor = textEdit->textCursor();
        QTextBlockFormat textBlockFormat;
        textBlockFormat.setLineHeight(60, QTextBlockFormat::FixedHeight);//设置固定行高
        textCursor.setBlockFormat(textBlockFormat);
        textEdit->setTextCursor(textCursor);

        textEdit->clear();
        textEdit->setPlainText(displayString);

        file.close();





        if(flag==false){
            QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel2,"geometry");
            animation1->setDuration(1000);
            animation1->setStartValue(QRect(winLabel2->x(),winLabel2->y(),winLabel2->width(),winLabel2->height()));
            animation1->setEndValue(QRect(winLabel2->x(),winLabel2->y()+684,winLabel2->width(),winLabel2->height()));
            animation1->setEasingCurve(QEasingCurve::OutBounce);
            animation1->start();

            QPropertyAnimation * animation5 =  new QPropertyAnimation(textEdit,"geometry");
            animation5->setDuration(1000);
            animation5->setStartValue(QRect(textEdit->x(),textEdit->y(),textEdit->width(),textEdit->height()));
            animation5->setEndValue(QRect(textEdit->x(),textEdit->y()+590,textEdit->width(),textEdit->height()));
            animation5->setEasingCurve(QEasingCurve::OutBounce);
            animation5->start();

            this->flag=true;
        }
        else{
            QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel2,"geometry");
            animation1->setDuration(1000);
            animation1->setStartValue(QRect(winLabel2->x(),winLabel2->y(),winLabel2->width(),winLabel2->height()));
            animation1->setEndValue(QRect(winLabel2->x(),winLabel2->y()-684,winLabel2->width(),winLabel2->height()));
            animation1->setEasingCurve(QEasingCurve::OutBounce);
            animation1->start();

            QPropertyAnimation * animation5 =  new QPropertyAnimation(textEdit,"geometry");
            animation5->setDuration(1000);
            animation5->setStartValue(QRect(textEdit->x(),textEdit->y(),textEdit->width(),textEdit->height()));
            animation5->setEndValue(QRect(textEdit->x(),textEdit->y()-590,textEdit->width(),textEdit->height()));
            animation5->setEasingCurve(QEasingCurve::OutBounce);
            animation5->start();

            this->flag=false;
        }
    });
    connect(infobtn,&mypushButton::clicked,[=](){
        playsound();
        infobtn->Zoom1(); //向下跳跃
        infobtn->Zoom2(); //向上跳跃
        if(flag==false){
            QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel3,"geometry");
            animation1->setDuration(1000);
            animation1->setStartValue(QRect(winLabel3->x(),winLabel3->y(),winLabel3->width(),winLabel3->height()));
            animation1->setEndValue(QRect(winLabel3->x(),winLabel3->y()+684,winLabel3->width(),winLabel3->height()));
            animation1->setEasingCurve(QEasingCurve::OutBounce);
            animation1->start();
            this->flag=true;
        }
        else{
            QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel3,"geometry");
            animation1->setDuration(1000);
            animation1->setStartValue(QRect(winLabel3->x(),winLabel3->y(),winLabel3->width(),winLabel3->height()));
            animation1->setEndValue(QRect(winLabel3->x(),winLabel3->y()-684,winLabel3->width(),winLabel3->height()));
            animation1->setEasingCurve(QEasingCurve::OutBounce);
            animation1->start();
            this->flag=false;
        }
    });

    ///------------------------------------------------------------
}

mainscene::~mainscene()
{
    delete ui;
}
///------------绘图器------------------
void mainscene::paintEvent(QPaintEvent *)
{
    //创建画家，指定绘图设备
    QPainter painter(this);
    //创建QPixmap对象
    QPixmap pix;
    //加载图片
    pix.load(":/res/mainbg.png");
    //绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //加载标题
    pix.load(":/res/mainscenebold.PNG");
    //缩放图片
    pix = pix.scaled(pix.width()*1.0,pix.height()*1.0);
    //绘制标题
    painter.drawPixmap( 90,30,pix.width(),pix.height(),pix);
}

///------------播放器------------------
void mainscene::playsound()
{

    QMediaPlayer *btn_2 = new QMediaPlayer();
    btn_2->setMedia(QUrl("qrc:/res/Btn.wav"));
    btn_2->setVolume(50);
    btn_2->play();

}
