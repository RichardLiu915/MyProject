#ifndef PLAYSCENE_LLK_H
#define PLAYSCENE_LLK_H

#include <QMainWindow>
#include<QLabel>
#include<QTimer>
#include <QDateTime>
#include<QMenuBar>
#include<QPainter>
#include<mypushbutton.h>
#include<QtMultimedia/qmediaplayer.h>
#include <QKeyEvent>
#include<QPushButton>
#include<playscene.h>
#include<QMessageBox>

class playscene_llk : public QMainWindow
{
    Q_OBJECT
public:
    explicit playscene_llk(QWidget *parent = nullptr);

    void InitGame_llk();
    void StartGame_llk();
    void ResetBlock_llk();
    void BlockMove_llk(Direction dir);
    void ConvertStable_llk(int x, int y);
    bool IsCollide_llk(int x, int y, Direction dir);
    void Remove_llk();
    void GameOver(); //游戏结束
    void block_pos_update_llk();
    void playsound_llk();
    void restartgame();
    void stoptimer_llk();
    void setRanking_llk(int s);//设置排行榜

    virtual void paintEvent(QPaintEvent *event_llk);
    virtual void timerEvent(QTimerEvent *event_llk);
    virtual void keyPressEvent(QKeyEvent *event_llk);

private:
    int game_area[AREA_ROW][AREA_COL]; //场景区域，1表示活动的方块，2表示稳定的方块，0表示空
    int game_area_color[AREA_ROW][AREA_COL];


    block_point block_pos_llk;
    block_point block_last_pos_llk;
    int cur_block_llk;
    int next_block_llk;

    int score;  //游戏分数
    int game_timer; //方块下落计时器
    int paint_timer; //渲染刷新计时器
    int speed_ms; //下落时间间隔
    int refresh_ms; //刷新时间间隔
    QMediaPlayer *btn_2;

    QVector<int> scores;//存放排行榜的分数，用来排序
signals:
    void playSceneback();
public slots:
};

#endif // PLAYSCENE_LLK_H
