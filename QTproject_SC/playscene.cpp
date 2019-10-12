#include <time.h>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include "playscene.h"

//定义图案代码和边界
//田字
int item1[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右L
int item2[4][4]=
{
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
//左L
int item3[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右S
int item4[4][4]=
{
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};
//左S
int item5[4][4]=
{
    {0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};
//山形
int item6[4][4]=
{
    {0,0,0,0},
    {0,0,1,0},
    {0,1,1,1},
    {0,0,0,0}
};
//长条
int item7[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
};

//拷贝方块图案
inline void block_cpy(int dblock[4][4],int sblock[4][4])
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            dblock[i][j]=sblock[i][j];
}

void PlayScene::setRanking(int s)
{

   scores.clear();//scores是一个vector数组，存放所有的分数，以便排序
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForLocale(codec);

    QFile file("D:/QT/QT FILE/QT LEARNING/Qt_Tetrix_3/res/ranking.txt");//D:/QT/QT FILE/QT LEARNING/Qt_Tetrix_3/ranking.txt
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;


    QTextStream in(&file);
    QString line = in.readLine();
    scores.append(s);


    while(!line.isNull())
    {
        int t = line.section('\t',0,0).toInt();
        scores.append(t);
        line = in.readLine();
    }
    file.close();

    if(scores.length() > 10)
    {
        int t = 0;
        for(int i = 0; i < scores.length(); i++)
        {
           if(scores.at(i) < scores.at(t))
              t = i;
        }
        scores.removeAt(t);
    }

    //排序
    qSort(scores.begin(),scores.end(),qGreater<int>());

//    for(int i = 0; i < scores.length(); i++)
//        qDebug() << scores.at(i) << "     ";


    QVector<int>::iterator it = scores.begin();

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << *it << endl;

    QTextStream out(&file);
    int i = 1;
    while(it != scores.end())
    {
        //out.setFieldWidth(10);
        out << *it  << "\t\t"<< i <<endl;

        i++;
        it++;
    }

    file.close();

}

PlayScene::PlayScene(QWidget *parent) :QMainWindow (parent)
{
    //设置固定大小
    this->setFixedSize(588,520);
    //设置标题
    this->setWindowTitle("单机模式");
    //设置应用图片
    this->setWindowIcon(QIcon(":/res/rect001.png"));

    //创建菜单栏
    QMenuBar*bar=menuBar();
    this->setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu=bar->addMenu("开始");

    //创建菜单项
    QAction *quitAction=startMenu->addAction("退出");

    //点击退出按钮 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        playsound();
        this->close();       

    });


    mypushButton*startbtn=new mypushButton(":/res/Play btn.png");
    startbtn->setParent(this);
    startbtn->move( 10,30);

    connect(startbtn,&mypushButton::clicked,[=](){
        playsound();
        startbtn->Zoom1();
        startbtn->Zoom2();
        game_timer=startTimer(speed_ms);
        paint_timer=startTimer(refresh_ms);
        this->setFocus();
    });

    mypushButton*pausebtn=new mypushButton(":/res/Pause btn.png");
    pausebtn->setParent(this);
    pausebtn->move( 60,30);
    connect(pausebtn,&mypushButton::clicked,[=](){
        playsound();
        pausebtn->Zoom1();
        pausebtn->Zoom2();
        this->stoptimer();
        this->setFocus();
    });

    mypushButton*forwardbtn=new mypushButton(":/res/Forward btn.png");
    forwardbtn->setParent(this);
    forwardbtn->move(110,30);
    connect(forwardbtn,&mypushButton::clicked,[=](){
        playsound();
        forwardbtn->Zoom1();
        forwardbtn->Zoom2();
        game_timer=startTimer(0.5*speed_ms);
        paint_timer=startTimer(0.5*refresh_ms);
        this->setFocus();
    });



    //重新游戏按钮创建
    mypushButton*restartbtn=new mypushButton(":/res/Reload btn.png");
    restartbtn->setParent(this);
    restartbtn->move( this->width()-restartbtn->width()-30,30);

    connect(restartbtn,&mypushButton::clicked,[=](){
        playsound();
        restartbtn->Zoom1();
        restartbtn->Zoom2();
        this->restartgame();

    });


    //返回按钮创建
    mypushButton*backbtn=new mypushButton(":/res/Back btn.png",":/res/Back btn.png");
    backbtn->setParent(this);
    backbtn->move( this->width()-backbtn->width()-30,this->height()-backbtn->height()-30);

    connect(backbtn,&mypushButton::clicked,[=](){
        playsound();

        backbtn->Zoom1();
        backbtn->Zoom2();

       // qDebug()<<"返回到主界面1"<<score;
        this->setRanking(score);
       // qDebug()<<"返回到主界面2";

        QTimer::singleShot(500,this,[=](){
            this->restartgame();
            emit this->playSceneback();
        });
        //回到主场景
    });

    //初始化游戏
    InitGame();
}

PlayScene::~PlayScene()
{
    delete ui;
}

void PlayScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/mainbg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画游戏场景边框
    painter.setBrush(QBrush(Qt::gray,Qt::SolidPattern));
    painter.drawRect(MARGIN+150,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);

    //画方块预告
    painter.setBrush(QBrush(Qt::darkBlue,Qt::SolidPattern));
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_block[i][j]==1)
                painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+j*BLOCK_SIZE+150,MARGIN+i*BLOCK_SIZE+150,BLOCK_SIZE,BLOCK_SIZE);
    //绘制得分
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(QRect(MARGIN*3+AREA_COL*BLOCK_SIZE+150,MARGIN*2+4*BLOCK_SIZE+150,BLOCK_SIZE*4,BLOCK_SIZE*4),Qt::AlignCenter,"score: "+QString::number(score));


    //绘制下落方块和稳定方块,注意方块边线的颜色是根据setPen来的，默认黑色
    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
        {
            //绘制活动方块
            if(game_area[i][j]==1)
            {
                painter.setBrush(QBrush(Qt::darkRed,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN+150,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
            //绘制稳定方块
            else if(game_area[i][j]==2)
            {
                painter.setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN+150,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
        }
}

void PlayScene::timerEvent(QTimerEvent *event)
{
    //方块下落
    if(event->timerId()==game_timer)
        BlockMove(DOWN);
    //刷新画面
    if(event->timerId()==paint_timer)
        update();
}

void PlayScene::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        BlockMove(UP);
        break;
    case Qt::Key_Down:
        BlockMove(DOWN);
        break;
    case Qt::Key_Left:
        BlockMove(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove(RIGHT);
        break;
    case Qt::Key_Space:
        BlockMove(SPACE);
        break;
    default:
        break;
    }
}

void PlayScene::CreateBlock(int block[4][4],int block_id)
{
    switch (block_id)
    {
    case 0:
        block_cpy(block,item1);
        break;
    case 1:
        block_cpy(block,item2);
        break;
    case 2:
        block_cpy(block,item3);
        break;
    case 3:
        block_cpy(block,item4);
        break;
    case 4:
        block_cpy(block,item5);
        break;
    case 5:
        block_cpy(block,item6);
        break;
    case 6:
        block_cpy(block,item7);
        break;
    default:
        break;
    }
}

void PlayScene::GetBorder(int block[4][4],Border &border)
{
    //计算上下左右边界
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.dbound=i;
                break; //直到计算到最后一行有1
            }
    for(int i=3;i>=0;i--)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.ubound=i;
                break;
            }
    for(int j=0;j<4;j++)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.rbound=j;
                break;
            }
    for(int j=3;j>=0;j--)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.lbound=j;
                break;
            }
}

void PlayScene::InitGame()
{
    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
            game_area[i][j]=0;

    this->setFocus();
    speed_ms=800;
    refresh_ms=30;

    //初始化随机数种子
    srand(time(0));

    //分数清0
    score=0;

    //开始游戏
    StartGame();
}

void PlayScene::ResetBlock()
{
    //产生当前方块
    block_cpy(cur_block,next_block);
    GetBorder(cur_block,cur_border);

    //产生下一个方块
    int block_id=rand()%7;
    CreateBlock(next_block,block_id);

    //设置初始方块坐标,以方块左上角为锚点
    block_point start_point;
    start_point.pos_x=AREA_COL/2-2;
    start_point.pos_y=0;
    block_pos=start_point;
}

void PlayScene::StartGame()
{
    game_timer=startTimer(speed_ms); //开启游戏timer
    paint_timer=startTimer(refresh_ms); //开启界面刷新timer
    //产生初始下一个方块
    int block_id=rand()%7;
    CreateBlock(next_block,block_id);
    ResetBlock(); //产生方块
}

void PlayScene::GameOver()
{
    //游戏结束停止计时器
    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this,"普通模式失败","Game Over");

}

void PlayScene::restartgame()
{
     //销毁当前所有小方块
    InitGame();

}

void PlayScene::stoptimer()
{
    killTimer(game_timer);
    killTimer(paint_timer);
}

void PlayScene::playsound()
{
    //播放器
    QMediaPlayer *btn_1 = new QMediaPlayer();
    btn_1->setMedia(QUrl("qrc:/res/Btn.wav"));
    btn_1->setVolume(50);
    btn_1->play();
}

void PlayScene::BlockRotate(int block[4][4])
{
    int temp_block[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp_block[3-j][i]=block[i][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            block[i][j]=temp_block[i][j];
}

void PlayScene::ConvertStable(int x,int y)
{
    for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            if(cur_block[i][j]==1)
                game_area[y+i][x+j]=2; //x和y别搞反
}

bool PlayScene::IsCollide(int x,int y,Direction dir)
{
    //用拷贝的临时方块做判断
    int temp_block[4][4];
    block_cpy(temp_block,cur_block);
    Border temp_border;
    GetBorder(temp_block,temp_border);
    //先尝试按照某方向走一格
    switch(dir)
    {
    case UP:
        BlockRotate(temp_block);
        GetBorder(temp_block,temp_border); //旋转后要重新计算边界
        break;
    case DOWN:
        y+=1;
        break;
    case LEFT:
        x-=1;
        break;
    case RIGHT:
        x+=1;
        break;
    default:
        break;
    }
    for(int i=temp_border.ubound;i<=temp_border.dbound;i++)
        for(int j=temp_border.lbound;j<=temp_border.rbound;j++)
            if(game_area[y+i][x+j]==2&&temp_block[i][j]==1||x+temp_border.lbound<0||x+temp_border.rbound>AREA_COL-1)
                return true;
    return false;
}

void PlayScene::BlockMove(Direction dir)
{
    switch (dir) {
    case UP:
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,UP))
            break;
        //逆时针旋转90度
        BlockRotate(cur_block);
        //防止旋转后bug,i和j从0到4重新设置方块
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        //重新计算边界
        GetBorder(cur_block,cur_border);
        break;
    case DOWN:
        //方块到达边界则不再移动
        if(block_pos.pos_y+cur_border.dbound==AREA_ROW-1)
        {
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }
        //碰撞检测，只计算上下左右边界，先尝试走一格，如果碰撞则稳定方块后跳出
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            //只有最终不能下落才转成稳定方块
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }
        //恢复方块上场景,为了清除移动过程中的方块残留
        for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
            game_area[block_pos.pos_y][block_pos.pos_x+j]=0;
        //没有碰撞则下落一格
        block_pos.pos_y+=1;
        //方块下降一格，拷贝到场景,注意左右边界
        for(int i=0;i<4;i++) //必须是0到4而不是边界索引，考虑到旋转后边界重新计算
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2) //注意场景数组不越界,而且不会擦出稳定的方块
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case LEFT:
        //到左边界或者碰撞不再往左
        if(block_pos.pos_x+cur_border.lbound==0||IsCollide(block_pos.pos_x,block_pos.pos_y,LEFT))
            break;
        //恢复方块右场景,为了清除移动过程中的方块残留
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            game_area[block_pos.pos_y+i][block_pos.pos_x+3]=0;
        block_pos.pos_x-=1;
        //方块左移一格，拷贝到场景
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.pos_x+j>=0&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2) //注意场景数组不越界
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case RIGHT:
        if(block_pos.pos_x+cur_border.rbound==AREA_COL-1||IsCollide(block_pos.pos_x,block_pos.pos_y,RIGHT))
            break;
        //恢复方块左场景,为了清除移动过程中的方块残留
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            game_area[block_pos.pos_y+i][block_pos.pos_x]=0;
        block_pos.pos_x+=1;
        //方块右移一格，拷贝到场景
        for(int i=cur_border.ubound;i<=cur_border.dbound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.pos_x+j<=AREA_COL-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2) //注意场景数组不越界
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        break;
    case SPACE: //一次到底
        //一格一格下移，直到不能下移
        while(block_pos.pos_y+cur_border.dbound<AREA_ROW-1&&!IsCollide(block_pos.pos_x,block_pos.pos_y,DOWN))
        {
            //恢复方块上场景,为了清除移动过程中的方块残留
            for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                game_area[block_pos.pos_y][block_pos.pos_x+j]=0;
            //没有碰撞则下落一格
            block_pos.pos_y+=1;
            //方块下降一格，拷贝到场景,注意左右边界
            for(int i=0;i<4;i++) //必须是0到4
                for(int j=cur_border.lbound;j<=cur_border.rbound;j++)
                    if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2) //注意场景数组不越界,而且不会擦出稳定的方块
                        game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
        }
        ConvertStable(block_pos.pos_x,block_pos.pos_y);
        ResetBlock();
        break;
    default:
        break;
    }
    //处理消行，整个场景上面的行依次下移
    int i=AREA_ROW-1;
    int line_count=0; //记消行数
    while(i>=1)
    {
        bool is_line_full=true;
        for(int j=0;j<AREA_COL;j++)
            if(game_area[i][j]==0)
            {
                is_line_full=false;
                i--;
                break;
            }
        if(is_line_full)
        {
            for(int k=i;k>=1;k--)
                for(int j=0;j<AREA_COL;j++)
                    game_area[k][j]=game_area[k-1][j];
            line_count++;//每次增加消行的行数
        }
    }
    score+=line_count*10; //得分
    //判断游戏是否结束
    for(int j=0;j<AREA_COL;j++)
        if(game_area[0][j]==2) //最顶端也有稳定方块
            GameOver();
}
