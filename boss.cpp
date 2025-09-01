#include "boss.h"
#include "qtimer.h"

Boss::Boss(QWidget *parent)
    : QWidget{parent}
{
    srand(QDateTime::currentMSecsSinceEpoch());     //设置种子

    x=1030;
    y=160;
    width=340;
    height=500;
    maxHp = 10000;
    hp = 10000;
    active = true;
    curBossIdx = 0;
    curSkill1Idx = 0;

    isReleaseSkill = false;
    isNormal = true;
    isSkill1 = false;

    skillNum = 1;
    type = Type::BOSS;

    rateSkillsTimer = new QTimer(this);
    NormalActiveTimer = new QTimer(this);
    deButTimer = new QTimer(this);
    Skill1Timer = new QTimer(this);

    QString str2= ":/ren3/resources/pugong";
    for(int i = 1;i <= 2;i++)
    {
        Skill1[i - 1] = QPixmap(QString(str2 + "%1" + ".png").arg(i));
        if (Skill1[i - 1].isNull()) {
            qDebug() << "图片加载失败！路径";
        }
    }

    QString str= ":/ren3/resources/jingzhi";
    for(int i = 1;i <= 4;i++)
    {
        boss[i - 1] = QPixmap(QString(str + "%1" + ".png").arg(i));
        if (boss[i - 1].isNull()) {
            qDebug() << "图片加载失败！路径";
        }
    }

    connect(NormalActiveTimer,&QTimer::timeout,this,[=](){
        curBossIdx++;
        if(curBossIdx > 3)
        {
            curBossIdx = 0;
        }
    });
    NormalActiveTimer->start(500);

    connect(Skill1Timer,&QTimer::timeout,this,[=](){
        curSkill1Idx++;
        if(curSkill1Idx > 1)
        {
            curSkill1Idx = 0;
        }
    });
    Skill1Timer->start(500);

    connect(deButTimer,&QTimer::timeout,this,[=](){
        deButTimer->stop();
        releaseSkills();
        qDebug()<<"deButTimer::TimerOut";
    });
    deButTimer->start(5000);

    connect(rateSkillsTimer,&QTimer::timeout,this,&Boss::releaseSkills);
    rateSkillsTimer->start(20000);
    qDebug()<<"boss 生成";
}

void Boss::setHp(int hp)
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

void Boss::setMaxHp(int val)
{
    this->maxHp = val;
}

int Boss::getHp()
{
    return this->hp;
}

int Boss::getMaxHp()
{
    return this->maxHp;
}

QPixmap Boss::bossPixmap()
{
    return boss[curBossIdx];
}

QRect Boss::getRect()
{
    return QRect(x,y,width,height);
}

QRect Boss::getDeadRect()
{
    return QRect(x + 100,y,width - 100,height);
}

int Boss::getType()
{
    return this->type;
}

void Boss::releaseSkills()
{
    if(isReleaseSkill)  //正在释放技能无法释放下一个技能
        return;

    curSkill = rand() % (skillNum - 1 + 1) + 1;
    switch (curSkill)
    {
        case 1:
            emit release_1();                           //释放普通攻击
            isReleaseSkill = true;
            isNormal = false;
            isSkill1 = true;
            qDebug()<<"释放一技能";
            break;
        default:
            break;
    }
}

void Boss::timerStop()
{
    NormalActiveTimer->stop();             //默认行为定时器(定时播放boss动画帧)
    Skill1Timer->stop();                   //1技能行为定时器(定时播放动画帧)

    deButTimer->stop();                    //登场计时器
    rateSkillsTimer->stop();               //技能释放频率计时器
}

void Boss::timerStart()
{
    NormalActiveTimer->start(500);
    Skill1Timer->start(500);
    deButTimer->start(5000);
    rateSkillsTimer->start(20000);
}
