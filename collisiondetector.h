#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H
/***********************************************************************************
*
* @file         collisiondetector.h
* @brief        碰撞检测者，主要用于物体碰撞检测
*
* @author       shanzhi-ph
* @date         2025/08/18
* @history
***********************************************************************************/
#include "collidable.h"
#include <QWidget>
class CollisionDetector
{
public:
    CollisionDetector();

    //物体碰撞检测函数
    static bool checkCollision(Collidable& obj1,Collidable& obj2);
};

#endif // COLLISIONDETECTOR_H
