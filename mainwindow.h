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

#include "player.h"
#include "barriergeneration.h"
#include "healthbar.h"
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
    player *m_player;     //玩家对象

private:
    Ui::MainWindow *ui;
    QTimer timer;
    QTimer playerTimer;     //人物动画定时器
    QTimer bgTimer;         //背景滚动定时器
    QPixmap bgImage;        //背景图片
    QPixmap startUi;        //首页背景
    QPixmap hurt;           //受伤图片
    QLine lineDown;         //路面线段
    HealthBar *hpBar;       //人物血量条
    QHash<barrier*,qint64> playerHash; //存储5秒内与人物碰撞过的障碍物

    QPushButton* beginBnt;      //开始按钮
    QPushButton* rulesBnt;      //规则按钮
    QPixmap beginImg;
    QPixmap rulesImg;

    int bgWidth;         // 背景图片宽度
    int bgHeight;        // 背景图片高度
    int bgX1;            // 第一张背景X坐标
    int bgX2;            // 第二张背景X坐标

    QMediaPlayer *backgroundMusic;  //背景音乐
    QAudioOutput *audioOutPut;      //音频输出对象

    QMediaPlayer *hurtMeida;        //受伤音效
    QAudioOutput *hurtAudio;        //音频输出对象

    BarrierGeneration* generation;  //用于生成与管理障碍物的类

    int gameStatus = GAMESTATUS::noRunning;

private:
    void initAttribute();                                       //初始化属性
    void drawPlayerRunning(QPainter* painter);                  //绘制玩家
    void drawBarrier(QPainter* painter);                        //绘制障碍物
    void initConnectTimer();                                    //初始化定时器连接
    void keyPressEvent(QKeyEvent* event) override;              //键盘按下事件
    void keyReleaseEvent(QKeyEvent* event) override;            //键盘松开事件
    void gameBegin();                                           //开始游戏
    void handleTimerSolt();                                     //总定时器槽函数
    void checkCollision();                                      //碰撞检测
    void handlerCollision(barrier* barr);                                    //处理碰撞
};
#endif // MAINWINDOW_H
