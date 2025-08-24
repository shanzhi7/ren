#include "barrier.h"

barrier::barrier(QWidget *parent)
    : QWidget{parent}
{

}

barrier::barrier(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->attack = 0;
}

int barrier::getX()
{
    return x;
}

int barrier::getY()
{
    return y;
}

int barrier::getWidth()
{
    return width;
}

int barrier::getheight()
{
    return height;
}

int barrier::getSpeed()
{
    return this->speed;
}

int barrier::getAttack()
{
    return this->attack;
}

QRect barrier::getRect()
{
    return QRect(this->x,this->y,this->width,this->height);
}

void barrier::setRect(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

bool barrier::isActive()
{
    if(x < 0)
        return false;
    return true;
}


void barrier::setX(int x)
{
    this->x = x;
}

void barrier::setY(int y)
{
    this->y = y;
}

void barrier::setWidth(int width)
{
    this->width = width;
}

void barrier::setHeight(int height)
{
    this->height = height;
}

void barrier::setAttack(int act)
{
    this->attack = act;
}

void barrier::setSpeed(int spd)
{
    this->speed = spd;
}





Sword::Sword(int x, int y, int width, int height)
    :barrier(x, y, width, height)
{
    this->pixmap = QPixmap(":/ren3/resources/feijian.png");
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(3);
    this->setAttack(50);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getheight());
}

QPixmap Sword::getPixmap()
{
    return this->pixmap;
}

QRect Sword::getDeadRect()
{
    return QRect(this->getX(),this->getY() + 25,this->getWidth(),this->getheight() - 32);
}

void Sword::move()
{
    this->setX(this->getX() - this->getSpeed());
    //qDebug()<<this->getX();
}
