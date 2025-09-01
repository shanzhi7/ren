#include "barriergeneration.h"
#include "mainwindow.h"
#include <QRandomGenerator>
#include <QDateTime>
#include <cstdlib>

BarrierGeneration::BarrierGeneration(QWidget *parent,Boss* bos,player* player)
    : QWidget{parent}
{
    srand(QDateTime::currentMSecsSinceEpoch());     //设置种子
    m_boss = bos;
    m_player = player;
    swordTimer = new QTimer(this);
    dartsTimer = new QTimer(this);
    moveTimer = new QTimer(this);
    NormalAttackTimer = new QTimer(this);
    skill1Timer = new QTimer(this);
    coinTimer = new QTimer(this);
    fireTimer = new QTimer(this);
    bullteFire = new QLabel(parent);
    firePixmap = QPixmap(":/ren3/zidantexiao/bullet1.png").scaled(100, 100, Qt::KeepAspectRatio);
    bullteFire->setPixmap(firePixmap);
    bullteFire->resize(100,100);

    connect(swordTimer,&QTimer::timeout,this,&BarrierGeneration::swordGenerate);
    connect(fireTimer,&QTimer::timeout,this,[=](){
        fireTimer->stop();
        bullteFire->hide();
    });
    connect(moveTimer,&QTimer::timeout,this,[=](){
        updateBulletPos();
        updateBarrierPos();
    });
    connect(NormalAttackTimer,&QTimer::timeout,this,&BarrierGeneration::NormalAttackGenerate);
    connect(coinTimer,&QTimer::timeout,this,&BarrierGeneration::CoinGenerate);
    connect(m_player,&player::fired,this,&BarrierGeneration::bulletGenerate);
    connect(dartsTimer,&QTimer::timeout,this,&BarrierGeneration::dartsGenerate);

    connect(m_boss,&Boss::release_1,this,[=](){    //接受到boss释放1技能的信号
        qDebug()<<"release_1";
        NormalAttackTimer->start(1500);//间隔1.5s生成一个普通攻击
        skill1Timer->start(20000);     //技能1总时长
    });
    connect(skill1Timer,&QTimer::timeout,this,[=](){            //普通攻击技能时长结束
        skill1Timer->stop();
        NormalAttackTimer->stop();
        m_boss->isReleaseSkill = false;
        m_boss->isNormal = true;
        m_boss->isSkill1 = false;
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
    moveTimer->start(17);            //60帧移动aa
    coinTimer->start(500);          //生成忍币
    dartsTimer->start(1000);         //生成飞镖
}

void BarrierGeneration::swordGenerate()
{

    int y = rand() % (600 - 150) + 150;

    list.append(new Sword(1400,y,100,50));
    //qDebug()<<"list_size = "<<list.size();
}

void BarrierGeneration::CoinGenerate()
{
    int y = rand() % (600 - 150) + 150;
    list.append(new Coin(1400,y));
}

void BarrierGeneration::NormalAttackGenerate()
{

    int y = rand() % (600 - 150) + 150;
    list.append(new NormalAttack(1400,y,100,70));
}

void BarrierGeneration::dartsGenerate()
{
    int y = rand() % (600 - 150) + 150;
    list.append(new Darts(1400,y));
}

void BarrierGeneration::bulletGenerate()
{
    int x = m_player->getx() + m_player->getWidth() + 5;
    int y = m_player->gety() + m_player->getHeight() / 2;
    bullteFire->move(x - 50,y - 50);
    bullteFire->raise();      // 确保在最上层
    bullteFire->show();
    fireTimer->start(200);
    bulletList.append(new Bullet(x,y));

    this->m_parent->firePlay();
}

void BarrierGeneration::setParent(MainWindow* parent)
{
    m_parent = parent;
}

void BarrierGeneration::deleteList()
{
    for(int i = 0;i < list.size();)
    {
        auto it = list[i];
        list.removeAt(i);
        delete it;
    }

    for(int i = 0;i < bulletList.size();)
    {
        auto it = bulletList[i];
        bulletList.removeAt(i);
        delete it;
    }
    qDebug()<<"list = "<<list.size();
    qDebug()<<"bulletList = "<<bulletList.size();
}

void BarrierGeneration::timerStop()
{
    swordTimer->stop();                         //生成飞剑定时器
    coinTimer->stop();                          //生成忍币定时器
    skill1Timer->stop();                        //一技能(普通攻击释放总时长)
    NormalAttackTimer->stop();                  //生成boos普通攻击计时器
    dartsTimer->stop();                         //生成飞镖定时器

    moveTimer->stop();                          //控制障碍物移动的定时器
    fireTimer->stop();                          //开火特效定时器
}

void BarrierGeneration::timerStart()
{
    swordTimer->start(1000);         //1000ms生成一个飞剑
    moveTimer->start(17);            //60帧移动aa
    coinTimer->start(500);          //生成忍币
    dartsTimer->start(1000);         //生成飞镖
}

void BarrierGeneration::bullteFireHide()
{
    bullteFire->hide();
}

QList<barrier *>& BarrierGeneration::getList()
{
    return this->list;
}

QList<Bullet *> &BarrierGeneration::getBulletList()
{
    return bulletList;
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

void BarrierGeneration::updateBulletPos()
{
    for(auto it = bulletList.begin();it != bulletList.end();)
    {
        barrier* cur = *it;
        if(!cur->isActive())
        {
            delete cur;
            it = bulletList.erase(it);
        }
        else
        {
            cur->move();
            it++;
        }
    }
}
