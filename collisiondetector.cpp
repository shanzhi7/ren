#include "collisiondetector.h"

CollisionDetector::CollisionDetector() {}

bool CollisionDetector::checkCollision(Collidable &obj1, Collidable &obj2)
{
    return obj1.getRect().intersects(obj2.getRect());
}
