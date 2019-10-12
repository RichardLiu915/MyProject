#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include<QPainter>
#include<QPushButton>
#include <QtMultimedia/qmediaplayer.h>
#include <QMediaPlaylist>
#include <QTextEdit>
#include<QTextCodec>
#include <QLineEdit>
#include <QTextStream>
#include"QFile"
#include"mypushbutton.h"
#include"playscene.h"
#include"QTimer"
#include"playscene_llk.h"

namespace Ui {
class mainscene;
}

class mainscene : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainscene(QWidget *parent = nullptr);
    ~mainscene();


    void paintEvent(QPaintEvent *);
    void playsound();

    PlayScene* playScene;
    playscene_llk*playScene_llk;

    bool flag;

    QMediaPlayer *BGM;
    QMediaPlayer *btn_2;
    QMediaPlaylist *playlist;

    QTextEdit *textEdit;
    QLineEdit *lineEdit;
private:
    Ui::mainscene *ui;
signals:
    void comein();
};

#endif // MAINSCENE_H
