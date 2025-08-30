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
    isReleaseSkill = false;
    skillNum = 1;

    rateSkillsTimer = new QTimer(this);
    NormalActiveTimer = new QTimer(this);
    deButTimer = new QTimer(this);

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
    return QRect(x + 25,y,width - 50,height);
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
            qDebug()<<"释放一技能";
            break;
        default:
            break;
    }
}
