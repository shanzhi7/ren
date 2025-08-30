#include "barriergeneration.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <cstdlib>

BarrierGeneration::BarrierGeneration(QObject *parent,Boss* bos)
    : QObject{parent}
{
    srand(QDateTime::currentMSecsSinceEpoch());     //设置种子
    m_boss = bos;
    swordTimer = new QTimer(this);
    moveTimer = new QTimer(this);
    NormalAttackTimer = new QTimer(this);
    skill1Timer = new QTimer(this);

    connect(swordTimer,&QTimer::timeout,this,&BarrierGeneration::swordGenerate);
    connect(moveTimer,&QTimer::timeout,this,&BarrierGeneration::updateBarrierPos);

    connect(NormalAttackTimer,&QTimer::timeout,this,&BarrierGeneration::NormalAttackGenerate);

    connect(m_boss,&Boss::release_1,this,[=](){    //接受到boss释放1技能的信号
        qDebug()<<"release_1";
        NormalAttackTimer->start(1500);//间隔1.5s生成一个普通攻击
        skill1Timer->start(20000);     //技能1总时长
    });
    connect(skill1Timer,&QTimer::timeout,this,[=](){            //普通攻击技能时长结束
        skill1Timer->stop();
        NormalAttackTimer->stop();
        m_boss->isReleaseSkill = false;
    });
}

BarrierGeneration::~BarrierGeneration()
{

}

void BarrierGeneration::setBoss(Boss *bos)
{
    m_boss = bos;
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

void BarrierGeneration::NormalAttackGenerate()
{

    int y = rand() % (600 - 150) + 150;
    qDebug()<<"list.apend";
    list.append(new NormalAttack(1400,y,100,70));
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
