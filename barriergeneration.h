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
#include <QObject>
#include <QTimer>
class BarrierGeneration : public QObject
{
    Q_OBJECT
public:
    explicit BarrierGeneration(QObject *parent = nullptr);
    ~BarrierGeneration();

    void barrierGenerate();                     //所有障碍物生成
    void swordGenerate();                       //生成飞剑/
    QList<barrier* >& getList();                 //获取障碍物列表

public slots:
    void updateBarrierPos();                    //更新每一帧障碍物的位置
private:
    QList<barrier* > list;                      //存储活跃的障碍物
    QTimer* swordTimer;                         //生成飞剑定时器
    QTimer* moveTimer;                          //控制障碍物移动的定时器
signals:
};

#endif // BARRIERGENERATION_H
