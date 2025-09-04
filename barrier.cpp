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
    isBarrier = true;
    type = -1;
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

int barrier::getHeight()
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

int barrier::getType()
{
    return type;
}

QRect barrier::getRect()
{
    return QRect(this->x,this->y,this->width,this->height);
}

QPixmap barrier::getPixmap()
{
    return this->pixmap;
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
    isBarrier = true;
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    type = Type::SWORD;
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(3);
    this->setAttack(50);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}



QRect Sword::getDeadRect()
{
    return QRect(this->getX(),this->getY() + 25,this->getWidth(),this->getHeight() - 32);
}

void Sword::move()
{
    this->setX(this->getX() - this->getSpeed());
    //qDebug()<<this->getX();
}

NormalAttack::NormalAttack(int x, int y, int width, int height)
{
    this->pixmap = QPixmap(":/ren3/resources/pugong.png");
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    type = Type::NORMALATTACT;
    this->setAttack(30);
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(4);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}

QRect NormalAttack::getDeadRect()
{
    return QRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}

void NormalAttack::move()
{
    this->setX(this->getX() - this->getSpeed());
}

Coin::Coin(int x, int y, int width, int height)
{
    this->pixmap = QPixmap(":/ren3/resources/coin.png");
    isBarrier = false;
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    type = Type::COIN;
    type = Type::COIN;
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(2);
    this->setAttack(0);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}

QRect Coin::getDeadRect()
{
    return QRect(this->getX() + 10,this->getY() + 10,this->getWidth() - 20,this->getHeight() - 15);
}

void Coin::move()
{
    this->setX(this->getX() - this->getSpeed());
}

Bullet::Bullet(int x, int y, int width, int height)
{
    this->pixmap = QPixmap(":/ren3/zidantexiao/bulletHead.png");
    isBarrier = false;
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    type = Type::BULLET;
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(15);
    this->setAttack(100);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}

QRect Bullet::getDeadRect()
{
    return QRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}

void Bullet::move()
{
    this->setX(this->getX() + this->getSpeed());
}

Darts::Darts(int x, int y, int width, int height)
{
    this->pixmap = QPixmap(":/ren3/resources/feibiao.png");
    isBarrier = true;
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    type = Type::DARTS;
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(5);
    this->setAttack(70);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());
}

QRect Darts::getDeadRect()
{
    return QRect(this->getX() + 10,this->getY() + 10,this->getWidth() - 20,this->getHeight() - 20);
}

void Darts::move()
{
    this->setX(this->getX() - this->getSpeed());
}

Flash::Flash(int x, int y, int width, int height)
{
    //this->pixmap = QPixmap(":/ren3/resources/feibiao.png");
    isBarrier = true;
    setX(x);
    setY(y);
    setWidth(width);
    setHeight(height);
    type = Type::FLASH;
    //qDebug()<<this->getWidth()<<" "<<this->getheight();
    this->setSpeed(2);
    this->setAttack(150);
    setRect(this->getX(),this->getY(),this->getWidth(),this->getHeight());

    curPixmapIdx = 0;
    QString str = ":/ren3/resources/1";
    for(int i = 1;i <= 4;i++)
    {
        flashPixmap[i - 1] = QPixmap(QString(str + "%1" + ".png").arg(i));
        if (flashPixmap[i - 1].isNull()) {
            qDebug() << "图片加载失败！路径";
        }
    }

    connect(&pixmapTimer,&QTimer::timeout,this,[=](){
        curPixmapIdx++;
        if(curPixmapIdx > 3)
        {
            curPixmapIdx = 0;
        }
    });
    pixmapTimer.start(250);
}

QRect Flash::getDeadRect()
{
    return QRect(this->getX() + 30,this->getY() + 30,this->getWidth() - 30,this->getHeight() - 60);
}

void Flash::move()
{
    if(getX() <= 900)
        setSpeed(20);
    this->setX(this->getX() - this->getSpeed());
}

QPixmap Flash::getPixmap()
{
    return this->flashPixmap[curPixmapIdx];
}
