#include "playscene_llk.h"
#include"QDebug"

playscene_llk::playscene_llk(QWidget *parent) : QMainWindow(parent)
{
    //设置固定大小
    this->setFixedSize(588,520);
    //设置标题
    this->setWindowTitle("连连看模式");
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

        this->close();

    });


    //返回按钮创建
    mypushButton*backbtn=new mypushButton(":/res/Back btn.png",":/res/Back btn.png");
    backbtn->setParent(this);
    backbtn->move( this->width()-backbtn->width()-30,this->height()-backbtn->height()-30);

    connect(backbtn,&mypushButton::clicked,[=](){
        playsound_llk();
        //qDebug()<<"返回到主界面";
        backbtn->Zoom1();
        backbtn->Zoom2();

        this->setRanking_llk(score);

        //隐藏自身

        QTimer::singleShot(500,this,[=](){
            this->restartgame();
            emit this->playSceneback();
        });
        //回到主场景
    });


    InitGame_llk();
}

void playscene_llk::setRanking_llk(int s)
{

   scores.clear();//scores是一个vector数组，存放所有的分数，以便排序
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForLocale(codec);

    QFile file("D:/QT/QT FILE/QT LEARNING/Qt_Tetrix_3/res/ranking.txt");
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
//
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

void playscene_llk::InitGame_llk()
{
    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
        {
            game_area[i][j]=0;
            game_area_color[i][j] = -1;
        }

    speed_ms=400;
    refresh_ms=30;


    //初始化随机数种子
    srand(time(0));

    //分数清0
    score=0;

    //开始游戏
    StartGame_llk();
    this->setFocus();

}

void playscene_llk::StartGame_llk()
{
    next_block_llk = rand() % 7;
    ResetBlock_llk(); //产生方块
    game_timer = startTimer(speed_ms); //开启游戏timer
    paint_timer = startTimer(refresh_ms); //开启界面刷新timer

}

void playscene_llk::ResetBlock_llk()
{
    //产生当前方块
    cur_block_llk = next_block_llk;

    //产生下一个方块
    next_block_llk = rand() % 7;

    //设置初始方块坐标,以方块左上角为锚点
    block_point start_point;
    start_point.pos_x = AREA_COL / 2 - 2;
    start_point.pos_y = 0;
    block_pos_llk = start_point;

    game_area[block_pos_llk.pos_y][block_pos_llk.pos_x] = 1;

}

void playscene_llk::restartgame()
{
     //销毁当前所有小方块
    InitGame_llk();

}

void playscene_llk::paintEvent(QPaintEvent *event_llk)
{
    //qDebug()<<"tupian";
    QPainter painter(this);

    QPixmap pix;
    pix.load(":/res/mainbg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);


    //画游戏场景边框
    painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    painter.drawRect(MARGIN+150, MARGIN, AREA_COL * BLOCK_SIZE, AREA_ROW * BLOCK_SIZE);
    //画方块预告
    switch (next_block_llk)
    {
    case 0:
    {
        painter.setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);

    }
        break;
    case 1:
    {
        painter.setBrush(QBrush(Qt::darkYellow, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);

    }
        break;
    case 2:
    {
        painter.setBrush(QBrush(Qt::darkCyan, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);

    }
        break;
    case 3:
    {
        painter.setBrush(QBrush(Qt::darkMagenta, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);

    }
        break;
    case 4:
    {
        painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);

    }
        break;
    case 5:
    {
        painter.setBrush(QBrush(Qt::darkBlue, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);
    }
        break;
    case 6:
    {
        painter.setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
        painter.drawRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+180, MARGIN +150, BLOCK_SIZE, BLOCK_SIZE);
    }
        break;

    }
    //绘制得分
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(QRect(MARGIN * 3 + AREA_COL * BLOCK_SIZE+150, MARGIN * 2 + 4 * BLOCK_SIZE+120, BLOCK_SIZE * 4, BLOCK_SIZE * 4), Qt::AlignCenter, "Score: " + QString::number(score));

    for(int i=0;i<AREA_ROW;i++)
        for(int j=0;j<AREA_COL;j++)
        {
            //绘制活动方块
            if(game_area[i][j] == 1 && cur_block_llk == 0)
            {
                painter.setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if (game_area[i][j] == 1 && cur_block_llk == 1)
            {
                painter.setBrush(QBrush(Qt::darkYellow, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);

            }
            else if (game_area[i][j] == 1 && cur_block_llk == 2)
            {
                painter.setBrush(QBrush(Qt::darkCyan, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);

            }
            else if (game_area[i][j] == 1 && cur_block_llk == 3)
            {
                painter.setBrush(QBrush(Qt::darkMagenta, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);

            }
            else if (game_area[i][j] == 1 && cur_block_llk == 4)
            {
                painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if (game_area[i][j] == 1 && cur_block_llk == 5)
            {
                painter.setBrush(QBrush(Qt::darkBlue, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if (game_area[i][j] == 1 && cur_block_llk == 6)
            {
                painter.setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            //绘制稳定方块
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 0)
            {
                painter.setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 1)
            {
                painter.setBrush(QBrush(Qt::darkYellow, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 2)
            {
                painter.setBrush(QBrush(Qt::darkCyan, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 3)
            {
                painter.setBrush(QBrush(Qt::darkMagenta, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 4)
            {
                painter.setBrush(QBrush(Qt::darkGreen, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 5)
            {
                painter.setBrush(QBrush(Qt::darkBlue, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }
            else if(game_area[i][j] == 2 && game_area_color[i][j] == 6)
            {
                painter.setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
                painter.drawRect(j * BLOCK_SIZE + MARGIN+150, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE, BLOCK_SIZE);
            }

        }
}

void playscene_llk::timerEvent(QTimerEvent *event_llk)
{
    //方块下落
    if(event_llk->timerId() == game_timer)
    {
        block_last_pos_llk = block_pos_llk;
        block_pos_llk.pos_y += 1;
        block_pos_update_llk();
        ConvertStable_llk(block_pos_llk.pos_x, block_pos_llk.pos_y);
        for(int j=0;j<AREA_COL;j++)
            if(game_area[0][j] == 2) //最顶端也有稳定方块
                GameOver();
    }
    //刷新画面
    if(event_llk->timerId() == paint_timer)
    {   update();
    }
}

void playscene_llk::keyPressEvent(QKeyEvent *event_llk)
{
    switch(event_llk->key())
    {
    case Qt::Key_Down:
        BlockMove_llk(DOWN);
        break;
    case Qt::Key_Left:
        BlockMove_llk(LEFT);
        break;
    case Qt::Key_Right:
        BlockMove_llk(RIGHT);
        break;
    case Qt::Key_Space:
        BlockMove_llk(SPACE);
        break;
    case Qt::Key_Escape:
        GameOver();
        break;
    default:
        break;
    }
}

bool playscene_llk::IsCollide_llk(int x, int y, Direction dir)//可移动则返回True
{
    switch (dir)
    {
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    default:
        break;
    }
    if(game_area[y][x] == 2 || x >= AREA_COL || x < 0 || y < 0 || y >= AREA_ROW)
        return false;
    if(game_area[y][x] == 0)
        return true;
    return false;
}



void playscene_llk::ConvertStable_llk(int x, int y)
{
    if(x + 1 >= AREA_COL || x - 1 < 0 || y + 1 >= AREA_ROW || y - 1 < 0 || game_area[y+1][x] == 2)
    {
        game_area[y][x] = 2;
        game_area_color[y][x] = cur_block_llk;
        ResetBlock_llk();
        Remove_llk();
    }
}

void playscene_llk::BlockMove_llk(Direction dir)
{
    switch (dir)
    {
    case DOWN:
        if (IsCollide_llk(block_pos_llk.pos_x, block_pos_llk.pos_y, DOWN))
        {
            block_last_pos_llk = block_pos_llk;
            block_pos_llk.pos_y += 1;
            block_pos_update_llk();
        }
        break;
    case LEFT:
        if(IsCollide_llk(block_pos_llk.pos_y, block_pos_llk.pos_y, LEFT))
        {
            block_last_pos_llk = block_pos_llk;
            block_pos_llk.pos_x -= 1;
            block_pos_update_llk();
        }
        break;
    case RIGHT:
        if(IsCollide_llk(block_pos_llk.pos_x, block_pos_llk.pos_y, RIGHT))
        {
            block_last_pos_llk = block_pos_llk;
            block_pos_llk.pos_x += 1;
            block_pos_update_llk();
        }
        break;
     }
    ConvertStable_llk(block_pos_llk.pos_x, block_pos_llk.pos_y);

}

void playscene_llk::Remove_llk()

{
    for (int i = 0;i < AREA_ROW;i++)
    {
        for (int j = 0;j < AREA_COL;j++)
        {
            if(i + 2 >= AREA_ROW)
                break;
            if(game_area_color[i][j] == game_area_color[i + 1][j] && game_area_color[i][j] == game_area_color[i + 2][j] && game_area_color[i][j] != -1)
            {
                playsound_llk();
                game_area[i][j] = 0;
                game_area[i + 1][j] = 0;
                game_area[i +2][j] = 0;
                game_area_color[i][j] = -1;
                game_area_color[i + 1][j] = -1;
                game_area_color[i + 2][j] = -1;
                score += 10;
            }
            if(j + 2 >= AREA_COL)
                break;
            if(game_area_color[i][j] == game_area_color[i][j + 1] && game_area_color[i][j] == game_area_color[i][j + 2] && game_area_color[i][j] != -1)
            {
                playsound_llk();
                game_area[i][j] = 0;
                game_area[i][j + 1] = 0;
                game_area[i][j + 2] = 0;
                game_area_color[i][j] = -1;
                game_area_color[i][j + 1] = -1;
                game_area_color[i][j + 2] = -1;
                score += 10;
            }

        }

    }
}



void playscene_llk::GameOver()
{
    //游戏结束停止计时器
    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this,"挑战模式失败","Game Over");
    QMediaPlayer *btn_2 = new QMediaPlayer();
    btn_2->setMedia(QUrl("qrc:/res/gmae.wav"));
    btn_2->setVolume(50);
    btn_2->play();

}

void playscene_llk::block_pos_update_llk()
{
    game_area[block_pos_llk.pos_y][block_pos_llk.pos_x] = 1;
    game_area[block_last_pos_llk.pos_y][block_last_pos_llk.pos_x] = 0;

}

void playscene_llk::playsound_llk()
{
    //播放器
    QMediaPlayer *btn_2 = new QMediaPlayer();
    btn_2->setMedia(QUrl("qrc:/res/Btn.wav"));
    btn_2->setVolume(50);
    btn_2->play();
}




