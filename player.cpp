#include "player.h"
#include <QDebug>
player::player(QWidget  *parent)
    : QWidget {parent}
{
    initPlayer();
    initPlayerRun();
    runTimer = new QTimer(this);

    curRunPixmap = playerRun[0];

    curPixmapIdx = 0;

    connect(runTimer,&QTimer::timeout,this,&player::playerRunning);

}

player::~player()
{
    //hpBar->deleteLater();
}

QPixmap player::getCurRunningPixmap()
{
    return curRunPixmap;
}


void player::move()
{
    if(this->y == 600)
    { // 落地状态
        downSpeed = 0;
        twice_ready = true; // 允许二次跳跃
    }
    if(up)
    {
        //一次跳跃
        if(this->y == 600)  //在地面才能一次跳跃
        {
            //跳跃处理
            jump_once = false;  //一次跳跃未完成
            jumpTimer->start();//(单次定时器)
        }
        //二次跳跃判断
        if(this->y < 600 && twice_ready && jump_once)
        {
            jumpTimer->start();
            downSpeed = 0;
            twice_ready = false;
        }
        if(jumpTimer->isActive())
        {
            this->y -= upSpeed * jumpTimer->remainingTime() / 400;
        }
    }
    else
    {
        jumpTimer->stop();
        jump_once=true;
    }
    //下落
    this->y += downSpeed;

    //位置限定
    if(this->y > 600)
    {
        this->y = 600;
        downSpeed = 0;  // 添加这行
    }
    if(this->y < 150)
    {
        this->y = 150;
    }
    if(this->x < 100)
    {
        this->x = 100;
    }
    if(this->x > 1000)
    {
        this->x = 1000;
    }

    //下落判断
    if(!jumpTimer->isActive())
    {
        //qDebug()<<jumpTimer->isActive();
        downSpeed += downSpeed_a;
    }
    else
    {
        downSpeed = 0;
    }

    //加速下落
    if(down)
    {
        if(this->y < 600 && !jumpTimer->isActive())
        {
            downSpeed += downSpeed_a * 2;
        }
    }

    //向右加速跑
    if(right)
    {
        this->x += 5;
    }
    //向左
    if(left)
    {
        this->x -= 5;
    }
}


int player::getx()
{
    return x;
}

int player::gety()
{
    return y;
}

int player::getWidth()
{
    return this->width;
}

int player::getHeight()
{
    return this->height;
}

int player::getHp()
{
    return hp;
}

QRect player::getRect()
{
    //this->setRect(this->x,this->y,this->width,this->height);
    return QRect(this->x,this->y,this->width,this->height);
}

QRect player::getDeadRect()
{
    //this->setRect(this->x,this->y,this->width,this->height);
    return QRect(this->x + 20,this->y,this->width - 25,this->height);
}

void player::setRect(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void player::setHp(int hp)
{
    if(this->hp == hp)
    {
        return;
    }
    else if(this->hp < hp)
    {
        this->hp = hp;
        emit hpChanged(hp);
        emit hpUp(hp);
    }
    else if(this->hp > hp)
    {
        this->hp = hp;
        emit hpChanged(hp);
        emit hpDown(hp);
    }
}

void player::playerRunning()
{
    curPixmapIdx = (curPixmapIdx + 1) % playerRun.size();   //更新动画帧下标
    curRunPixmap = playerRun[curPixmapIdx];                 //更新动画帧图片
}

void player::initPlayer()
{
    jumpTimer = new QTimer(this);
    jumpTimer->setInterval(400);
    jumpTimer->setSingleShot(true);     //设置为单次定时器

    up = false;
    down = false;
    left = false;
    right = false;

    x = 200;
    y = 600;
    width = 50;
    height = 50;
    maxHp = 1000;
    hp = 1000;
    attack = 100;
    maxButtetNum = 10;


    jump_once = false;          //已经跳跃一次
    jump_twice = false;         //已经跳跃两次
    twice_ready = false;        //能否二次跳跃
    isDown = false;             //是否处于下落状态
    jump_maxH = 100;            //最大跳跃高度
    downSpeed = 0;              //下落速度
    upSpeed = 20;               //上升速度
    horSpeed = 30;              //水平速度
    downSpeed_a = 1;            //向下加速度
}

//初始化人物跑酷图片
void player::initPlayerRun()
{
    if(!playerRun.isEmpty())
        return ;
    for(int i = 1;i <= 12;i++)
    {
        playerRun.push_back(QPixmap(QString(":/ren3/xiaoshuai/%1.png").arg(i)));
    }
}
