#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "collisionDetector.h"
#include <QPainter>
#include <QDebug>
#include <QRandomGenerator>
#include <QDateTime>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , bgX1(0)          // 第一张背景的X坐标
    , bgX2(0)          // 第二张背景的X坐标（初始在右侧）
{
    ui->setupUi(this);
    this->setFixedSize(1280,720);
    this->setWindowTitle("山河疾行");

    ruleWidgt = new RuleDialog();
    ruleWidgt->setRuleText("游戏规则说明：\n\n"
                           "1. 使用WASD方向键控制角色移动\n"
                           "2. 打败Boss完成关卡\n"
                           "3. 避开障碍物\n"
                           "4. 拾取忍币发射子弹攻击\n"
                           "5. 游戏结束按ESC返回主窗口");

    lineDown = QLine(0, 650, width(), 650);
    startUi = QPixmap(":/ren3/resources/start_ui.png");

    beginImg = QPixmap(":/ren3/resources/startbut_1.png");
    rulesImg = QPixmap(":/ren3/resources/butt_1.png");
    hurt = QPixmap(":/ren3/resources/hurt.png");
    deadPixmap = QPixmap(":/ren3/resources/death.png");
    winBackPixmap = QPixmap(":/ren3/resources/play_ground.jpg");
    winPixmap = QPixmap(":/ren3/resources/win.png");


    beginBnt = new QPushButton(this);
    rulesBnt = new QPushButton(this);

    beginBnt->setFixedSize(QSize(beginImg.width() - 5,beginImg.height() - 5));
    rulesBnt->setFixedSize(QSize(rulesImg.width() - 5,rulesImg.height() - 5));

    //设置图标大小等于图片尺寸
    beginBnt->setIconSize(beginImg.size());
    rulesBnt->setIconSize(rulesImg.size());

    //移除内边距和边框
    beginBnt->setStyleSheet("border: none; padding: 0px;");
    rulesBnt->setStyleSheet("border: none; padding: 0px;");

    beginBnt->setIcon(beginImg);
    rulesBnt->setIcon(rulesImg);

    beginBnt->move(500,400);
    rulesBnt->move(700,400);

    connect(beginBnt,&QPushButton::clicked,this,[=](){
        this->gameBegin();
    });

    connect(rulesBnt,&QPushButton::clicked,this,[=](){
        ruleWidgt->show();
    });

    // 加载背景图片并获取尺寸
    bgImage = QPixmap(":/ren3/back.jpg");
    bgWidth = bgImage.width();
    bgHeight = bgImage.height();
    //bgImage = bgImage.scaled(bgWidth*2, bgHeight, Qt::KeepAspectRatioByExpanding);
    bgX2 = bgWidth - 5; // 初始时第二张背景在第一张右侧

    hpBar = new HealthBar(this);
    hpBar->setValue(1000);
    hpBar->setMaxValue(1000);
    hpBar->setGeometry(50,680,700,30);

    bossHpBar = new HealthBar(this);
    bossHpBar->setValue(10000);
    bossHpBar->setMaxValue(10000);
    bossHpBar->setGeometry(100,10,1000,30);
    hpBar->hide();
    bossHpBar->hide();
}

MainWindow::~MainWindow()
{
    delete ruleWidgt;
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
    if(gameStatus == GAMESTATUS::noRunning)  //游戏未开始处于首页状态
    {
        painter.drawPixmap(0,0,width(),height(),startUi);
    }
    else if(gameStatus == GAMESTATUS::isRunning)
    {

        //绘制无缝滚动背景
        painter.drawPixmap(bgX1, 0, bgWidth, this->height(), bgImage); // 第一张背景
        painter.drawPixmap(bgX2, 0, bgWidth, this->height(), bgImage); // 第二张背景（衔接第一张）

        //绘制地面参考线
        painter.save();
        QPen pen;
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawLine(lineDown);
        painter.restore();

        //绘制人物
        drawPlayerRunning(&painter);

        //绘制boss
        drawBoss(&painter);

        //绘制障碍物
        drawBarrier(&painter);

        //绘制子弹
        drawBullet(&painter);

        if(hurtImgAlpha != 0)
        {
            drawHurtImg(&painter);
        }

    }
    else if(gameStatus == GAMESTATUS::isOver)
    {
        if(!isWin)
        {
            painter.drawPixmap(this->rect(),deadPixmap);
        }
        else
        {
            painter.drawPixmap(this->rect(),winBackPixmap);
            painter.drawPixmap(this->width() / 2 - 300,this->height() / 2 - 100,600,200,winPixmap);
        }
    }
    QMainWindow::paintEvent(event);
}

void MainWindow::firePlay()
{
    if(fireMeida->isPlaying())
        fireMeida->stop();
    this->fireMeida->play();
}

void MainWindow::fireStop()
{
    fireMeida->stop();
}

void MainWindow::hurtPlay()
{
    if(hurtMeida->isPlaying())
        hurtMeida->stop();
    hurtMeida->play();
}

void MainWindow::initAttribute()
{
    hurtImgAlpha = 0;
    isWin = false;
    if(!m_player)
        m_player = new player(this);
    if(!m_boss)
        m_boss = new Boss(this);

    if(!generation)
        generation = new BarrierGeneration(this,m_boss,m_player);
    generation->setBoss(m_boss);
    generation->setParent(this);

    if(!backgroundMusic)
        backgroundMusic = new QMediaPlayer(this);   //创建播放器实例
    if((!audioOutPut))
        audioOutPut = new QAudioOutput(this);

    backgroundMusic->setAudioOutput(audioOutPut); //设置音频源

    QUrl audioUrl("qrc:/ren3/sound/longyuexing.mp3");
    backgroundMusic->setSource(audioUrl);
    // 设置音量
    audioOutPut->setVolume(0.8);

    //循环播放
    connect(backgroundMusic, &QMediaPlayer::playbackStateChanged, this, [=](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::StoppedState && gameStatus == GAMESTATUS::isRunning) {
            backgroundMusic->play();
        }
    });

    //连接人物生命发生减少时的信号槽
    connect(m_player,&player::hpDown,this,&MainWindow::playerHpChangeDownSlot);

    //连接boss血量减少的槽函数
    connect(m_boss,&Boss::hpDown,this,&MainWindow::bossHpChangDownSolt);

    //游戏失败
    connect(this,&MainWindow::gameFail,this,&MainWindow::gameFailSolt);

    //游戏胜利
    connect(this,&MainWindow::gameWin,this,&MainWindow::gameWinSolt);

    if(!hurtMeida)
        hurtMeida = new QMediaPlayer(this);
    if(!hurtAudio)
        hurtAudio = new QAudioOutput(this);
    hurtMeida->setAudioOutput(hurtAudio);
    QUrl hurtUrl("qrc:/ren3/sound/shoushang.wav");
    hurtMeida->setSource(hurtUrl);
    //设置音量
    hurtAudio->setVolume(1.0);

    if(!fireMeida)
        fireMeida = new QMediaPlayer(this);
    if(!fireAudio)
        fireAudio = new QAudioOutput(this);
    fireMeida->setAudioOutput(fireAudio);
    QUrl firetUrl("qrc:/ren3/sound/fire.wav");
    fireMeida->setSource(firetUrl);
    //设置音量
    fireAudio->setVolume(1.0);
    initConnectTimer();  //初始化定时器连接
}

void MainWindow::drawPlayerRunning(QPainter *painter)
{
    painter->drawPixmap(m_player->getRect(),m_player->getCurRunningPixmap());
    //painter->drawRect(m_player->getDeadRect());
}

void MainWindow::drawBarrier(QPainter *painter)
{
    auto& list  = generation->getList();
    for(int i = 0;i < list.size();)
    {
        auto it = list[i];
        if(!it)
        {
            list.removeAt(i);
        }
        else
        {
            painter->drawPixmap(it->getRect(),it->getPixmap());
            //wdpainter->drawRect(it->getDeadRect());
            //qDebug()<<it->getRect().x()<<" "<<it->getRect().y()<<" "<<it->getWidth()<<" "<<it->getheight();
            //qDebug()<<it->getDeadRect().x()<<" "<<it->getDeadRect().y()<<" "<<it->getWidth()<<" "<<it->getheight();
            i++;
        }
    }
}

void MainWindow::drawBoss(QPainter *painter)
{
    if(m_boss == nullptr)
    {
        qDebug()<<"m_boss为空指针";
        return;
    }
    if(!m_boss->isReleaseSkill) //未释放技能
    {
        painter->drawPixmap(m_boss->getRect(),m_boss->boss[m_boss->curBossIdx]);
        //painter->drawRect(m_boss->getDeadRect());
    }
    else                        //释放技能
    {
        if(m_boss->isSkill1)
            painter->drawPixmap(m_boss->getRect(),m_boss->Skill1[m_boss->curSkill1Idx]);
        else if(m_boss->isSkill2)
            painter->drawPixmap(m_boss->getRect(),m_boss->Skill2[m_boss->curSkill2Idx]);
        //painter->drawRect(m_boss->getDeadRect());
    }
}

void MainWindow::drawBullet(QPainter *painter)
{
    auto& list  = generation->getBulletList();
    for(int i = 0;i < list.size();)
    {
        auto it = list[i];
        if(!it)
        {
            list.removeAt(i);
        }
        else
        {
            painter->drawPixmap(it->getRect(),it->getPixmap());
            //dpainter->drawRect(it->getDeadRect());
            //qDebug()<<it->getRect().x()<<" "<<it->getRect().y()<<" "<<it->getWidth()<<" "<<it->getheight();
            //qDebug()<<it->getDeadRect().x()<<" "<<it->getDeadRect().y()<<" "<<it->getWidth()<<" "<<it->getheight();
            i++;
        }
    }
}

void MainWindow::drawHurtImg(QPainter* m_painter)
{
    //创建临时透明图片
    QPixmap tempImg(hurt.size());
    tempImg.fill(Qt::transparent);

    QPainter painter(&tempImg);

    //设置源模式,用源像素hurtImg覆盖透明目标
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0,0,hurt);

    //设置目标模式,用hurt的Alpha调整temp的alpha(透明度)
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(tempImg.rect(),QColor(0,0,0,hurtImgAlpha));

    m_painter->drawPixmap(0,0,this->width(),this->height(),tempImg);

    hurtImgAlpha -= 5;
}

void MainWindow::initConnectTimer()
{
    // 总定时器
    connect(&playerTimer, &QTimer::timeout, this, [=]() {
        handleTimerSolt();  //处理定时器timeout
    });

    // 背景滚动定时器
    connect(&bgTimer, &QTimer::timeout, this, [=]() {
        // 两张背景同时向左移动（速度5像素/帧）
        bgX1 -= 5;
        bgX2 -= 5;

        // 当第一张背景完全移出屏幕左侧时，重置到右侧
        if (bgX1 <= -bgWidth) {
            bgX1 = bgX2 + bgWidth - 5;
        }
        // 当第二张背景完全移出屏幕左侧时，重置到右侧
        if (bgX2 <= -bgWidth) {
            bgX2 = bgX1 + bgWidth - 5;
        }
        update(); // 触发重绘
    });

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(gameStatus == GAMESTATUS::noRunning)
    {
        QMainWindow::keyPressEvent(event);
        return;
    }
    else if(gameStatus == GAMESTATUS::isOver)
    {
        if(event->key() == Qt::Key_Escape)
        {
            gameStatus = GAMESTATUS::noRunning;
            beginBnt->show();
            rulesBnt->show();
            update();
        }
    }
    else if(event->key() == Qt::Key_W)
    {
        m_player->up = true;
    }
    else if(event->key() == Qt::Key_S)
    {
        m_player->down = true;
    }
    else if(event->key() == Qt::Key_D)
    {
        m_player->right = true;
    }
    else if(event->key() == Qt::Key_A)
    {
        m_player->left = true;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(gameStatus == GAMESTATUS::noRunning)
    {
        QMainWindow::keyReleaseEvent(event);
        return;
    }
    else if(event->key() == Qt::Key_W)
    {
        m_player->up = false;
    }
    else if(event->key() == Qt::Key_S)
    {
        m_player->down = false;
    }
    else if(event->key() == Qt::Key_D)
    {
        m_player->right = false;
    }
    else if(event->key() == Qt::Key_A)
    {
        m_player->left = false;
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::gameBegin()
{
    isWin = false;
    gameStatus = GAMESTATUS::isRunning;
    beginBnt->hide();
    rulesBnt->hide();

    hpBar->show();
    bossHpBar->show();

    initAttribute();    //初始化属性

    hpBar->setValue(1000);
    hpBar->setMaxValue(1000);

    bossHpBar->setValue(10000);
    bossHpBar->setMaxValue(10000);

    m_player->setHp(1000);
    m_boss->setHp(10000);

    playerTimer.start(17);          //总定时器
    bgTimer.start(30);              //背景图定时器
    m_player->runTimer->start(50); //人物跑动定时器
    // 开始播放
    if(backgroundMusic->isPlaying())
        backgroundMusic->stop();
    backgroundMusic->play();

    generation->barrierGenerate();
    timerStart();
    update();
}

void MainWindow::handleTimerSolt()
{
    update();                               //触发重绘
    generation->updateBarrierPos();         //更新障碍物位置
    m_player->move();                       //更新人物位置
    checkCollision();                       //碰撞检测
    checkBulletCollision();
}

void MainWindow::checkCollision()
{
    auto& list = generation->getList();
    const int MAX_CHECK_SIZE = 100;          // 每帧最大检测此数,解决每帧检测次数过多导致程序界面卡死bug (障碍物碰撞导致无限触发碰撞检测处理
                                            // 无法执行i++，程序死循环)
    int cnt = 0;                            //当前检测此数
    for(int i = 0;i < list.size();)
    {
        if(cnt > MAX_CHECK_SIZE)
            return;
        cnt++;
        //qDebug()<<cnt;
        auto it = list[i];
        if(!it)
        {
            list.removeAt(i);
            playerHash.remove(it);
            if (playerHash.contains(it))
                qDebug() << "哈希表中存在空指针！";
        }
        else
        {
            if(CollisionDetector::checkCollision(*m_player,*it))        //两物体碰撞
            {
                qint64 curTime = QDateTime::currentMSecsSinceEpoch();
                if(it->getType() == Type::COIN)
                {
                    list.removeAt(i);
                    emit m_player->fired();                           //发送信号
                    continue;
                }
                if(playerHash.contains(it))                             //检测之前是否碰撞过
                {
                    qint64 lastTime = playerHash[it];
                    if(curTime - lastTime < 5000)                       //距离上次碰撞是否超过5秒
                        continue;
                    else
                    {
                        //超过5秒,处理碰撞，更新最新碰撞时间
                        handlerCollision(it);
                        playerHash[it] = curTime;
                    }
                }
                else
                {
                    //第一次碰撞,处理碰撞,将障碍物加入哈希表
                    playerHash[it] = curTime;
                    handlerCollision(it);
                }
            }
            i++;
        }
    }
}

void MainWindow::checkBulletCollision()
{
    auto& bulletList = generation->getBulletList();

    auto& barrierList = generation->getList();
    const int MAX_CHECK_SIZE = 100;          // 每帧最大检测此数,解决每帧检测次数过多导致程序界面卡死bug (障碍物碰撞导致无限触发碰撞检测处理
    // 无法执行i++，程序死循环)
    int cnt = 0;                            //当前检测此数
    for(int i = 0;i < bulletList.size();)
    {

        if(cnt > MAX_CHECK_SIZE)
            return;
        cnt++;
        auto bulletIt = bulletList[i];
        bool bulletRemoved = false;
        if(!bulletIt)
        {
            bulletList.removeAt(i);
            continue;
        }
        else
        {
            if(cnt > MAX_CHECK_SIZE)
                return;
            cnt++;
            //检测子弹与障碍物碰撞
            for(int j = 0;j < barrierList.size();)
            {
                auto barrierIt = barrierList[j];
                cnt++;
                if(!barrierIt)
                {
                    barrierList.removeAt(j);
                    continue;
                }
                else
                {
                    if(CollisionDetector::checkCollision(*bulletIt,*barrierIt)
                        && barrierIt->getType()
                               !=Type::COIN && barrierIt->getType() != Type::FLASH && bulletIt && barrierIt)
                    {
                        bulletList.removeAt(i);
                        barrierList.removeAt(j);
                        bulletRemoved = true;
                        break;
                    }
                    else
                    {
                        j++;
                    }
                }
            }
            // 如果子弹已被障碍物移除，跳过BOSS检测
            if (bulletRemoved)
            {
                continue; // 保持i不变（下次循环会处理新位置i的子弹）
            }

            // BOSS碰撞检测
            if (cnt >= MAX_CHECK_SIZE)
                return;
            cnt++; // BOSS碰撞检测计数

            if(CollisionDetector::checkCollision(*bulletIt,*m_boss) && bulletIt)
            {
                qDebug()<<"pengzhuang";
                bulletList.removeAt(i);
                handlerBossCollision(bulletIt);
            }
            i++;
        }
    }
}

void MainWindow::handlerBossCollision(Bullet *bullet)
{
    qDebug()<<"boss";
    m_boss->setHp(m_boss->getHp() - bullet->getAttack());

    int newBarValue = m_boss->getHp();
    //动画平滑更新血条
    QPropertyAnimation *animation = new QPropertyAnimation(bossHpBar,"value",this);
    animation->setDuration(500);
    animation->setStartValue(bossHpBar->value());
    animation->setEndValue(newBarValue);
    animation->setEasingCurve(QEasingCurve::OutCurve);
    animation->start(QAbstractAnimation::DeleteWhenStopped);        //动画结束自动销毁
}

void MainWindow::handlerCollision(barrier* barr)
{
    m_player->setHp(m_player->getHp() - barr->getAttack());
    //hpBar->setValue(m_player->getHp());

    int newBarValue = m_player->getHp();
    //动画平滑更新血条
    QPropertyAnimation *animation = new QPropertyAnimation(hpBar,"value",this);
    animation->setDuration(500);
    animation->setStartValue(hpBar->value());
    animation->setEndValue(newBarValue);
    animation->setEasingCurve(QEasingCurve::OutCurve);
    animation->start(QAbstractAnimation::DeleteWhenStopped);        //动画结束自动销毁
}

void MainWindow::timerStop()
{
    timer.stop();
    playerTimer.stop();                     //人物动画定时器
    bgTimer.stop();                         //背景滚动定时器
    m_player->runTimer->stop();
    m_boss->timerStop();
    generation->timerStop();
}

void MainWindow::timerStart()
{
    m_boss->timerStart();
    generation->timerStart();
}

void MainWindow::playerHpChangeDownSlot()
{
    qDebug()<<m_player->getHp();
    hurtPlay();
    hurtImgAlpha = 255;
    if(m_player->getHp() <= 0)
    {
        emit gameFail();
        isWin = false;
        gameStatus = GAMESTATUS::isOver;
    }

}

void MainWindow::bossHpChangDownSolt()
{
    if(m_boss->getHp() <= 0)
    {
        emit gameWin();
        isWin = true;
        gameStatus = GAMESTATUS::isOver;
    }
}

void MainWindow::gameFailSolt()
{
    isWin = false;
    timerStop();
    backgroundMusic->stop();
    hpBar->hide();
    disconnect(&playerTimer,nullptr,this,nullptr);
    disconnect(&bgTimer,nullptr,this,nullptr);

    m_boss->isReleaseSkill = false;
    m_boss->isNormal = true;
    m_boss->isSkill1 = false;
    generation->bullteFireHide();

    m_player->setRect(200,600,50,50);

    bossHpBar->hide();
    generation->deleteList();
}

void MainWindow::gameWinSolt()
{
    isWin = true;
    timerStop();
    backgroundMusic->stop();
    hpBar->hide();
    disconnect(&playerTimer,nullptr,this,nullptr);
    disconnect(&bgTimer,nullptr,this,nullptr);

    m_boss->isReleaseSkill = false;
    m_boss->isNormal = true;
    m_boss->isSkill1 = false;
    generation->bullteFireHide();

    m_player->setRect(200,600,50,50);

    bossHpBar->hide();
    generation->deleteList();
}
