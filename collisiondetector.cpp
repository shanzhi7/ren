#include "collisiondetector.h"

CollisionDetector::CollisionDetector() {}

bool CollisionDetector::checkCollision(Collidable &obj1, Collidable &obj2)
{
    return obj1.getDeadRect().intersects(obj2.getDeadRect());
}
