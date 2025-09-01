#ifndef BOSS_H
#define BOSS_H

#include "global.h"
#include "collidable.h"
#include <QWidget>
#include <QTimer>
#include <cstdlib>
#include <QDateTime>

class Boss : public QWidget,public Collidable
{
    Q_PROPERTY(int hp READ getHp WRITE setHp NOTIFY hpChanged)
    Q_OBJECT
public:
    explicit Boss(QWidget *parent = nullptr);

    int x;
    int y;
    int width;
    int height;
    int hp;                     //血量
    int maxHp;                  //最大血量
    bool active;                //是否存活

    bool isReleaseSkill;        //是否正在释放技能
    bool isNormal;              //未释放技能
    bool isSkill1;              //正在释放一技能

    int curBossIdx;             //当前boss默认图片下标
    int curSkill1Idx;           //当前1技能图片下表

    int skillNum;               //技能数量
    int curSkill;               //当前技能
    int type = Type::BOSS;

    QPixmap boss[4];
    QPixmap Skill1[2];

    QTimer* NormalActiveTimer;             //默认行为定时器(定时播放boss动画帧)
    QTimer* Skill1Timer;                   //1技能行为定时器(定时播放动画帧)

    QTimer* deButTimer;                    //登场计时器
    QTimer* rateSkillsTimer;               //技能释放频率计时器

    void setHp(int val);                   //设置血量
    void setMaxHp(int val);                //设置最大血量

    int getHp();                           //huoqu血量
    int getMaxHp();                        //获取最大血量
    QPixmap bossPixmap();
    QRect getRect();                       //获取矩形
    QRect getDeadRect();                   //获取碰撞体型
    int getType();                         //获取类型

    void releaseSkills();                  //释放技能
    void timerStop();                      //定时器停止
    void timerStart();                     //定时器启动

signals:
    void release_1();                      //释放普通攻击
    void hpChanged(int newHp);
    void hpUp(int newHp);
    void hpDown(int newHp);
};

#endif // BOSS_H
