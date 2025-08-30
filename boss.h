#ifndef BOSS_H
#define BOSS_H

#include <QWidget>
#include <QTimer>
#include <cstdlib>
#include <QDateTime>

class Boss : public QWidget
{
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
    int curBossIdx;             //当前boss默认图片下标
    int skillNum;               //技能数量
    int curSkill;               //当前技能

    QPixmap boss[4];

    QTimer* NormalActiveTimer;             //默认行为定时器(定时播放boss动画帧)
    QTimer* deButTimer;                    //登场计时器
    QTimer* rateSkillsTimer;               //技能释放频率计时器

    QPixmap bossPixmap();
    QRect getRect();                       //获取矩形
    QRect getDeadRect();                   //获取碰撞体型

    void releaseSkills();                  //释放技能

signals:
    void release_1();                        //释放普通攻击
};

#endif // BOSS_H
