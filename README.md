# MyQTproject
2 课题：方块大作战
2.1课题功能
课题来源：自拟题目
所解决的问题： 
俄罗斯方块和连连看都是经典的游戏，尽管两者都是方块游戏，界面也较为相似，但很少有人将两者结合起来。此外，用传统控制台运行的黑白界面，一定程度上影响游戏的交互与体验，一个好的游戏界面正是Qt图形界面设计的应有之义。
本课程设计正是基于这一需求，决定将两款游戏合二为一，融入不同的模式，并在呈现形式、界面设计以及玩法上有所创新，自行设计多场景的展示方式，自行设计按钮类动画，重新设计了连连看的游戏模式，希望能以全新趣味的交互界面，提高用户的游戏体验。
主要的功能：
方块大作战是一款基于方块的集俄罗斯方块和连连看于一体的方块游戏，普通模式为经典的俄罗斯方块，增添控制游戏速度的控件，增加游戏的可玩性。挑战模式为全新的连连看，无法调节游戏速度，更具挑战性。
此外，设置了排行榜可以记录和反映玩家的实时排名与分数。
表2-1-1功能介绍
2.2 开发平台
操作系统：Window 10
C++环境：Qt5.9.8
2.3 总体方案
2.3.1 总体流程图
   
图2-3-1总体流程图
本游戏采用多场景搭建的方案，设计有三个场景，分别为：游戏主界面、普通模式界面和挑战模式界面。主界面设计了排行榜、游戏介绍和开发者成员等自定义控件，与用户进行交互。

图2-3-2游戏设计图
方块由正方形的色块组成，存储在二维数组中，计算机随机产生七种不同类型的方块，程序根据这七种方块堆叠成各种不同的模型。
用户根据键盘的四个方向键控制图形进行翻转、向左、向右和向下操作，控制键的实现是由键盘的方向键的事件处理实现。

图2-3-3游戏实现图
模型产生方块，进行下落。
控制器通过keyPressEvent(QKeyEvent *event)函数响应键盘事件，
通过判断碰撞函数、计算边界函数重置、变动、旋转方块，处理游戏逻辑。 
视图通过paintEvent(QPaintEvent *event)函数绘制显示，
通过timerEvent(QTimerEvent *event) 函数刷新显示。

2.3.2 相关数据结构
（1）自定义MyPushButton类，作为游戏按钮控件的主要方法。
通过构造MyPushButton的重载版本，可以让MyPushButton提供正常显示的图片以及按下后显示的图片，并为MyPushButton扩展特效代码，通过控制事件发生，达到动画的效果。
mypushButton::mypushButton(QString normalImg, QString pressImg)  
{  
    //成员变量normalImgPath保存正常显示图片路径  
    normalImgPath = normalImg;  
    //成员变量pressedImgPath保存按下后显示的图片  
    pressedImgPath = pressImg;  
    ...  
}  
void mypushButton::mousePressEvent(QMouseEvent *e)  
{  
    if(this->pressedImgPath!="")  
    {  
     ...  
    }  
}  
void mypushButton::mouseReleaseEvent(QMouseEvent *e)  
{  
    if(this->pressedImgPath!="")  
    {  
     ...  
    }  
} 
本游戏按钮动画主要设置为QEasingCurve::OutBounce ，具有弹跳效果 
animation1->setEasingCurve(QEasingCurve::OutBounce);  

（2）方块的数据结构
使用一个4×4的方阵，用“0”和“1”来表示每个方格的填充状态，以此来模拟七种不同类型的方块，通过随机产生一个随机数，可以组合出多种图像。
因此，本游戏使用7个二维数组作为存储方块的数据结构。

图2-3-4方块生成的数据结构图
此外，方块的旋转是通过绘制四个方向的方块，重新计算边界，在无碰撞干扰的情况下，直接更新显示。


（3）排行榜更新的数据结构
排行榜利用 QVector<int> 容器存取分数，并利用了qSort()函数进行排序。
此外，通过迭代器写文件，将分数排名写入文件
//读取本局分数  
scores.append(s);  
//读取历史分数  
    while(!line.isNull())  
    {  
        int t = line.section('\t',0,0).toInt();  
        scores.append(t);  
        line = in.readLine();  
    }  
    //排序  
    qSort(scores.begin(),scores.end(),qGreater<int>());  
//迭代器写文件  
    QVector<int>::iterator it = scores.begin();  
    while(it != scores.end())  
    {  
        out << *it  << "\t\t"<< i <<endl;  
        i++;  
        it++;  
    }  
















2.4 详细设计
2.4.1函数
表2-4-1函数全体











2.4.2 main函数

图2-4-1主函数流程图

2.4.3 InitGame()函数

2.4.4 StartGame()函数

2.4.5 GameOver()函数

2.4.6 restartgame()函数

2.4.7 stoptimer()函数

2.4.8 ResetBlock()函数

2.4.9 BlockMove()函数

2.4.10 BlockRotate()函数

2.4.11 CreateBlock()函数

2.4.12 GetBorder()函数

2.4.13 ConvertStable()函数

2.4.14 IsCollide()函数

2.4.15 setRanking()函数

2.5 测试与运行
表2-5-1主界面测试

 
 
表2-5-2游戏界面测试


 
 

表2-5-3按键测试

表2-5-4方块固定与消除测试


经过测试，本游戏可以实现所有基本的功能，完成度较好。
2.6 调试记录
表2-6-1调试记录


2.7 分工情况
  表2-7-1小组基本信息

  表2-7-2分工情况


