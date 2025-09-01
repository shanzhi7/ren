#ifndef BARRIER_H
#define BARRIER_H
/***********************************************************************************
*
* @file         barrier.h
* @brief        所有障碍物的基类
*
* @author       shanzhi-ph
* @date         2025/08/18
* @history
***********************************************************************************/
#include "collidable.h"
#include "global.h"
#include <QWidget>

class barrier : public QWidget,public Collidable
{
    Q_OBJECT
public:
    explicit barrier(QWidget *parent = nullptr);

    barrier(int x,int y,int width,int height);

    QPixmap pixmap;
    bool isBarrier;                 //是否是有伤害属性的障碍物

    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getSpeed();
    int getAttack();
    int getType();

    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setAttack(int act);
    void setSpeed(int spd);

    int type;
    int speed;
    QRect getRect() override;                                    //获取矩形
    QPixmap getPixmap();
    void setRect(int x,int y,int width,int height);
    bool isActive();                                    //是否存活
    virtual QRect getDeadRect() = 0;                    //获取碰撞矩形
    virtual void move() = 0;

private:
    int x,y,width,height;
    int attack;

signals:
};

//派生飞剑
class Sword : public barrier
{
private:

public:
    Sword(int x,int y,int width = 100,int height = 50);


    QRect getDeadRect() override;                       //获取碰撞矩形
    void move() override;                               //移动
};

//派生boss普通攻击
class NormalAttack : public barrier
{
private:

public:
    NormalAttack(int x,int y,int width = 100,int height = 70);


    QRect getDeadRect() override;                       //获取碰撞体型
    void move() override;                               //移动

};

class Coin : public barrier
{
private:

public:
    Coin(int x,int y,int width = 50,int height = 50);


    QRect getDeadRect() override;                       //获取碰撞矩形
    void move() override;                               //移动
};

class Bullet : public barrier
{
private:

public:
    Bullet(int x,int y,int width = 50,int height = 40);


    QRect getDeadRect() override;                       //获取碰撞矩形
    void move() override;                               //移动
};

class Darts : public barrier
{
private:

public:
    Darts(int x,int y,int width = 70,int height = 70);

    QRect getDeadRect() override;                      //获取碰撞矩形
    void move() override;                              //移动
};


#endif // BARRIER_H
