#ifndef BARRIERGENERATION_H
#define BARRIERGENERATION_H
/***********************************************************************************
*
* @file         barriergeneration.h
* @brief        障碍物生成管理类
*
* @author       shanzhi-ph
* @date         2025/08/19
* @history
***********************************************************************************/
#pragma once
#include "barrier.h"
#include "player.h"
#include "boss.h"
#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QPixmap>
class MainWindow;
class BarrierGeneration : public QWidget
{
    Q_OBJECT
public:
    explicit BarrierGeneration(QWidget *parent = nullptr,Boss* bos = nullptr,player* player = nullptr);
    ~BarrierGeneration();

    void setBoss(Boss* bos);                    //设置boss对象
    void barrierGenerate();                     //所有障碍物生成
    void swordGenerate();                       //生成飞剑
    void CoinGenerate();                        //生成忍币
    void NormalAttackGenerate();                //生成boss普通攻击
    void skill2AttackkGenerate();               //生成boss二技能攻击
    void dartsGenerate();                       //生成飞镖
    void bulletGenerate();                      //生成子弹
    void setParent(MainWindow* parent);         //设置父对象
    void deleteList();                          //清除list

    void timerStop();                           //定时器停止
    void timerStart();                          //定时器启动

    void bullteFireHide();                      //隐藏特效
    QList<barrier* >& getList();                //获取障碍物列表
    QList<Bullet* > &getBulletList();           //获取子弹列表

public slots:
    void updateBarrierPos();                    //更新每一帧障碍物的位置
    void updateBulletPos();                     //更新每一帧子弹的位置
private:
    QList<barrier* > list;                      //存储活跃的障碍物
    QList<Bullet* > bulletList;                 //存储活跃的子弹
    QTimer* swordTimer;                         //生成飞剑定时器
    QTimer* coinTimer;                          //生成忍币定时器
    QTimer* skill1Timer;                        //一技能(普通攻击释放总时长)
    QTimer* skill2Timer;                        //二技能释放总时长

    QTimer* NormalAttackTimer;                  //生成boos普通攻击计时器
    QTimer* skill2AttackTimer;                  //生成二技能计时器
    QTimer* dartsTimer;                         //生成飞镖定时器

    QTimer* moveTimer;                          //控制障碍物移动的定时器
    QTimer* fireTimer;                          //开火特效定时器

    Boss* m_boss;                               //boss对象
    player* m_player;                           //角色对象
    QLabel* bullteFire;                         //开枪特效
    MainWindow* m_parent;                       //父对象(窗口)
    QPixmap firePixmap;                         //特效图片
signals:

};

#endif // BARRIERGENERATION_H
