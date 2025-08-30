#ifndef PLAYER_H
#define PLAYER_H

/***********************************************************************************
*
* @file         player.h
* @brief        玩家类，主要定义了玩家的属性以及行为
*
* @author       shanzhi-ph
* @date         2025/08/18
* @history
***********************************************************************************/
#include "collidable.h"
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QWidget>
class player : public QWidget,public Collidable
{
    Q_PROPERTY(int hp READ getHp WRITE setHp NOTIFY hpChanged)
    Q_OBJECT
public:
    explicit player(QWidget  *parent = nullptr);
    ~player();

//公共成员变量
public:
    QVector<QPixmap> playerRun;
    QTimer *runTimer;               //人物跑步动画定时器

    bool up;
    bool down;
    bool left;
    bool right;

    bool jump_once;                 //已经跳了一次，判断能不能二段跳的
    bool jump_twice;                //已经跳了两次
    double horSpeed;                //水平速度
    double upSpeed;                 //上升速度
    double downSpeed;               //下落速度
    double downSpeed_a;             //下落加速度
    int jump_maxH;                  //一次最大跳跃高度
    bool isDown;                    //正在下落
    bool twice_ready;               //可以二次跳


//私有成员变量
private:
    int x,y,width,height;
    int hp;                             //人物血量
    int maxHp;                          //人物最大血量

    QPixmap curRunPixmap;               //人物当前动画帧图片
    int curPixmapIdx;                   //人物当前动画帧图片索引
    QTimer* jumpTimer;                  //跳跃定时器

//公共函数
public:
    QPixmap getCurRunningPixmap();  //获取当前跑步图片帧
    void move();                    //移动
    int getx();
    int gety();
    int getWidth();
    int getHeight();
    int getHp();
    QRect getRect() override;                   //获取人物矩形
    QRect getDeadRect();                        //获取人物可碰撞矩形

    void setRect(int x,int y,int width,int height);
    void setHp(int hp);
//私有函数
private:
    void initPlayerRun();
    void playerRunning();
    void initPlayer();
signals:
    void hpChanged(int newHp);
    void hpUp(int newHp);
    void hpDown(int newHp);
};

#endif // PLAYER_H
