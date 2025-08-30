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
#include "barrier.h"
#include "boss.h"
#include <QObject>
#include <QTimer>
class BarrierGeneration : public QObject
{
    Q_OBJECT
public:
    explicit BarrierGeneration(QObject *parent = nullptr,Boss* bos = nullptr);
    ~BarrierGeneration();

    void setBoss(Boss* bos);                             //设置boss对象
    void barrierGenerate();                     //所有障碍物生成
    void swordGenerate();                       //生成飞剑
    void NormalAttackGenerate();                //生成boss普通攻击
    QList<barrier* >& getList();                 //获取障碍物列表

public slots:
    void updateBarrierPos();                    //更新每一帧障碍物的位置
private:
    QList<barrier* > list;                      //存储活跃的障碍物
    QTimer* swordTimer;                         //生成飞剑定时器
    QTimer* skill1Timer;                        //一技能(普通攻击释放总时长)
    QTimer* NormalAttackTimer;                  //生成boos普通攻击计时器
    QTimer* moveTimer;                          //控制障碍物移动的定时器
    Boss* m_boss;                               //boss对象
signals:
};

#endif // BARRIERGENERATION_H
