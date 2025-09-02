#ifndef COLLIDABLE_H
#define COLLIDABLE_H
/***********************************************************************************
*
* @file         collidable.h
* @brief        所有可碰撞物体的基类
*
* @author       shanzhi-ph
* @date         2025/08/18
* @history
***********************************************************************************/
#include <QRect>
class Collidable
{
public:
    Collidable();
    // 纯虚函数：返回自身的QRect（包含位置和尺寸）
    virtual QRect getRect() = 0;
    virtual QRect getDeadRect() = 0;
    virtual ~Collidable() = default;
};

#endif // COLLIDABLE_H
