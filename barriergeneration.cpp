#include "barriergeneration.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <cstdlib>

BarrierGeneration::BarrierGeneration(QObject *parent)
    : QObject{parent}
{

    srand(QDateTime::currentMSecsSinceEpoch()); // 设置种子

    swordTimer = new QTimer(this);
    moveTimer = new QTimer(this);
    connect(swordTimer,&QTimer::timeout,this,&BarrierGeneration::swordGenerate);
    connect(moveTimer,&QTimer::timeout,this,&BarrierGeneration::updateBarrierPos);
}

BarrierGeneration::~BarrierGeneration()
{

}

void BarrierGeneration::barrierGenerate()
{
    swordTimer->start(1000);         //1000ms生成一个飞剑
    moveTimer->start(17);
}

void BarrierGeneration::swordGenerate()
{
    int y = rand() % (600 - 150) + 150;

    list.append(new Sword(1400,y,100,50));
    //qDebug()<<"list_size = "<<list.size();
}

QList<barrier *>& BarrierGeneration::getList()
{
    return this->list;
}

void BarrierGeneration::updateBarrierPos()
{
    for(auto it = list.begin();it != list.end();)
    {
        barrier* cur = *it;
        if(!cur->isActive())
        {
            delete cur;
            it = list.erase(it);
        }
        else
        {
            cur->move();
            it++;
        }
    }
}
