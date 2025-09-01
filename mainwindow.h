#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/***********************************************************************************
*
* @file         mainwindow.h
* @brief        主窗口类兼游戏管理类,负责窗口的绘制以及管理游戏内对象
*
* @author       shanzhi-ph
* @date         2025/08/18
* @history
***********************************************************************************/
#pragma once

#include "player.h"
#include "barriergeneration.h"
#include "global.h"
#include "healthbar.h"
#include "boss.h"
#include <QMainWindow>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QLabel>
#include <QLine>
#include <QPushButton>
#include <QKeyEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QHash>
#include <QGraphicsOpacityEffect>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent* event) override;

public:
    enum GAMESTATUS{
        noRunning = 0,
        isRunning,
        isOver
    };
    player *m_player = nullptr;     //玩家对象
    Boss *m_boss = nullptr;         //boss对象

    void firePlay();
    void fireStop();

    void hurtPlay();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    QTimer playerTimer;                     //人物动画定时器
    QTimer bgTimer;                         //背景滚动定时器
    QPixmap bgImage;                        //背景图片
    QPixmap startUi;                        //首页背景
    QPixmap hurt;                           //受伤图片
    QPixmap deadPixmap;                     //死亡图片
    QLabel hurtLabel;                       //受伤图片标签
    QGraphicsOpacityEffect* hurtOpacity = nullptr;    //hurtLabel透明度
    QLine lineDown;                         //路面线段
    HealthBar *hpBar = nullptr;                       //人物血量条
    HealthBar* bossHpBar = nullptr;                   //boss血量条
    QHash<barrier*,qint64> playerHash;      //存储5秒内与人物碰撞过的障碍物

    QPushButton* beginBnt = nullptr;                  //开始按钮
    QPushButton* rulesBnt = nullptr;                  //规则按钮
    QPixmap beginImg;
    QPixmap rulesImg;

    int bgWidth;                            // 背景图片宽度
    int bgHeight;                           // 背景图片高度
    int bgX1;                               // 第一张背景X坐标
    int bgX2;                               // 第二张背景X坐标
    bool isWin;                             //是否获胜

    int hurtImgAlpha;                       //图片透明度

    QMediaPlayer *backgroundMusic = nullptr;  //背景音乐
    QAudioOutput *audioOutPut = nullptr;      //音频输出对象

    QMediaPlayer *hurtMeida = nullptr;        //受伤音效
    QAudioOutput *hurtAudio = nullptr;        //音频输出对象

    QMediaPlayer* fireMeida = nullptr;        //开火音效
    QAudioOutput* fireAudio = nullptr;        //音频输出对象

    BarrierGeneration* generation = nullptr;  //用于生成与管理障碍物的类

    int gameStatus = GAMESTATUS::noRunning;

private:
    void initAttribute();                                       //初始化属性
    void drawPlayerRunning(QPainter* painter);                  //绘制玩家
    void drawBarrier(QPainter* painter);                        //绘制障碍物
    void drawBoss(QPainter* painter);                           //绘制boss
    void drawBullet(QPainter* painter);                         //绘制子弹
    void drawHurtImg(QPainter* m_painter);                      //绘制受伤图片
    void initConnectTimer();                                    //初始化定时器连接
    void keyPressEvent(QKeyEvent* event) override;              //键盘按下事件
    void keyReleaseEvent(QKeyEvent* event) override;            //键盘松开事件
    void gameBegin();                                           //开始游戏
    void handleTimerSolt();                                     //总定时器槽函数
    void checkCollision();                                      //碰撞检测
    void checkBulletCollision();                                //子弹碰撞检测
    void handlerBossCollision(Bullet* bullet);                  //处理Boss碰撞
    void handlerCollision(barrier* barr);                       //处理碰撞
    void timerStop();                                           //停止定时器
    void timerStart();                                          //定时器启动


private slots:
    void playerHpChangeDownSlot();                              //槽函数玩家血量减少的
    void bossHpChangDownSolt();                                 //boss血量减少槽函数
    void gameFailSolt();                                        //游戏结束槽函数

signals:
    void gameFail();

};
#endif // MAINWINDOW_H
