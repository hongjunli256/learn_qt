#include "GameManager.h"
#include "./ui_widget.h"
#include<QFile>
#include<QMessageBox>
#include"GameInitialConfig.h"
GameManager::GameManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->initView();
    //媒体初始化
    this->mAudioOutputMusic = new QAudioOutput(this);
    this->mAudioOutputMusic->setVolume(GameInitialConfig::Volume);  // 音量范围0.0-1.0
    this->mAudioOutputSound = new QAudioOutput(this);
    this->mAudioOutputSound->setVolume(GameInitialConfig::Volume);  // 音量范围0.0-1.0

    this->initScene_Start();

    this->mGameView.setScene(&this->mScene_Start);
    this->mGameView.show();
    this->initScene_Fight();
    this->initScene_Pause();

    //背景滚动
    this->timerRollBG=new QTimer(this);
    connect(this->timerRollBG,&QTimer::timeout,this,&GameManager::BGroll);

    //飞机移动
    this->timerPlaneMove=new QTimer(this);
    connect(this->timerPlaneMove,&QTimer::timeout,this,&GameManager::planeMove);
    connect(this->timerPlaneMove,&QTimer::timeout,this,[this](){
        for(Enemy*em:std::as_const(this->gameItemPool.mEnemyList))
        {
            if(em->dead!=true)
            {
                em->EnemyMove();
            }
            if(em->y()>GameInitialConfig::MapHeight)
            {
                this->gameItemPool.removeEnemy(em);
                this->mScene_Fight.removeItem(em);
            }
        }
    });


    //子弹发射
    this->timerShootBullet=new QTimer(this);
    connect(this->timerShootBullet,&QTimer::timeout,this,[this](){
        this->mScene_Fight.addItem(this->gameItemPool.addBullet(this->gameItemPool.mPlane.shootBullet(),Bullet::myBullet));

        this->mMediaShoot.setAudioOutput(this->mAudioOutputSound);
        this->mMediaShoot.stop();
        this->mMediaShoot.setSource(QUrl("./sound/bullet.wav"));
        // 播放前检查媒体状态
        this->mMediaShoot.play();
        for(Enemy*em:std::as_const(this->gameItemPool.mEnemyList))
        {
            if(em->y()>0||em->dead!=true)
            {
                this->mScene_Fight.addItem(this->gameItemPool.addBullet(em->shootBullet(),Bullet::emnemyBullet));
            }
        }
    });

    //子弹移动
    this->timerBulletMove=new QTimer(this);
    // this->timerBulletMove->start(30);
    connect(this->timerBulletMove,&QTimer::timeout,this,[this](){
        this->Collision();
        for(int i=0;i<this->gameItemPool.mBulletList.size();i++)
        {
            Bullet*bullet=this->gameItemPool.mBulletList[i];
            bullet->bulletMove();
            if(bullet->y()<0||bullet->y()>GameInitialConfig::MapHeight)
            {
                this->mScene_Fight.removeItem(bullet);
                this->gameItemPool.removeBullet(bullet);
            }
        }
    });

    //产生敌人
    this->timerGenerateEnemy=new QTimer(this);
    connect(this->timerGenerateEnemy,&QTimer::timeout,this,&GameManager::generateEnemy);


    //从开始界面到游戏界面
    connect(&this->mButton_Start,&QPushButton::clicked,this,[this](){

        //定时器启动
        this->timer_Start();
        //场景切换
        this->mGameView.setScene(&this->mScene_Fight);
        this->mGameView.show();

        //背景音乐启动
        this->mMediaBG.setAudioOutput(this->mAudioOutputMusic);
        this->mMediaBG.stop();
        this->mMediaBG.setSource(QUrl("./sound/game_music.ogg"));
        // 播放前检查媒体状态
        this->mMediaBG.play();

    });
    //从开始界面到暂停界面
    connect(&this->mButton_Pause,&QPushButton::clicked,this,[this](){

        //定时器暂停
        this->timer_Pause();
        //场景切换
        this->mGameView.setScene(&this->mScene_Pause);
        this->mGameView.show();
        this->mMediaBG.stop();

    });
    //从暂停界面到战斗界面
    connect(&this->mButton_Resume,&QPushButton::clicked,this,[this](){

        //定时器暂停
        this->timer_Start();
        //场景切换
        this->mGameView.setScene(&this->mScene_Fight);
        this->mGameView.show();
        this->mMediaBG.play();

    });
    connect(&this->mButton_gameover,&QPushButton::clicked,this,&GameManager::gameOver);
    connect(&this->mButton_again,&QPushButton::clicked,this,&GameManager::replay);


}
void GameManager::replay()
{

    this->timer_Pause();
	for (Bullet *bullet : std::as_const(this->gameItemPool.mBulletList))
	{
		this->gameItemPool.removeBullet(bullet);
        this->mScene_Fight.removeItem(bullet);
	}
	for (Enemy *enemy : std::as_const(this->gameItemPool.mEnemyList))
	{
        this->gameItemPool.removeEnemy(enemy);
        this->mScene_Fight.removeItem(enemy);
    }
    this->initScene_Fight();
    this->mGameView.setScene(&this->mScene_Fight);
    this->mGameView.show();
    this->mMediaBG.play();
    this->timer_Start();

}
void GameManager::gameOver()
{
    auto reply=QMessageBox::question(this, "退出游戏", "确定要退出游戏吗？",
                                       QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}
void GameManager::initView()
{
    // 设置视图对齐方式，使其从场景的左上角开始显示
    this->mGameView.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->mGameView.setParent(this);
    // 禁用滚动条
    this->mGameView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->mGameView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //窗口初始化
    this->resize(GameInitialConfig::MapWidth,GameInitialConfig::MapHeight);//700

    //视图初始化
    this->mGameView.setSceneRect(QRect(0,0,GameInitialConfig::MapWidth,GameInitialConfig::MapHeight));//显示的初始区域
    this->mGameView.setFixedSize(GameInitialConfig::MapWidth,GameInitialConfig::MapHeight);//视野大小
}
void GameManager::initScene_Start()
{
    //起始场景搭建
    this->mBackground_Start.setPixmap(QPixmap(":/img/src/images/background.png"));
    this->mBackground_Start.setPos(0,0);

    this->mButton_Start.resize(160, 50);
    this->mButton_Start.setText("游戏开始");
    this->mButton_Start.move(160, 500);
    // 字体设置
    QFont font;
    font.setFamily("Comic Sans MS");
    font.setPointSize(14);
    font.setBold(true);
    this->mButton_Start.setFont(font);

    // 样式表设置
    this->mButton_Start.setStyleSheet(R"(
    QPushButton {
        color: #333333; /* 深灰色文字，和背景协调 */
        background-color: rgba(255, 255, 255, 80); /* 白色半透明背景，柔和不突兀 */
        border: 2px solid #cccccc; /* 浅灰边框，模拟手绘线条感 */
        padding: 0px;
    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 120); /* 悬停时白色更明显 */
        border-color: #aaaaaa; /* 边框稍深，突出交互 */
    }
    QPushButton:pressed {
        background-color: rgba(240, 240, 240, 120); /* 按下时浅灰，有按压反馈 */
        padding: 9px 14px 7px 16px; /* 轻微位移，增强按压感 */
    }
    QPushButton:focus {
        outline: none; /* 去除焦点虚线，保持简洁 */
    }
)");
    this->mScene_Start.setSceneRect(QRect(0,0,GameInitialConfig::MapWidth,GameInitialConfig::MapHeight));
    this->mScene_Start.addItem(&this->mBackground_Start);
    this->mScene_Start.addWidget(&this->mButton_Start);
}
void GameManager::initScene_Fight()    //战斗场景搭建
{
    //背景图
    this->mBackground1.setPixmap(QPixmap(":/img/src/images/background.png"));
    this->mBackground2.setPixmap(QPixmap(":/img/src/images/background.png"));
    this->mBackground1.setPos(0,0);
    this->mBackground2.setPos(0,-1*GameInitialConfig::MapHeight);
    this->mScene_Fight.setSceneRect(QRect(0,0,GameInitialConfig::MapWidth,GameInitialConfig::MapHeight));
    this->mScene_Fight.addItem(&this->mBackground1);
    this->mScene_Fight.addItem(&this->mBackground2);

    //暂停按钮
    this->mButton_Pause.resize(60,45);
    this->mButton_Pause.move(420,0);
    // 图标尺寸建议比按钮小2-4像素，避免边缘被截断
    this->mButton_Pause.setIconSize(QSize(56,41));//4657

    // 核心样式表：完全透明背景，无边框，仅在交互时有轻微反馈
    this->mButton_Pause.setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
        image: url(:/img/src/images/pause_nor.png);
    }
    QToolButton:pressed {

        image: url(:/img/src/images/pause_pressed.png);
    }
)");

    //生命值显示
    life.setPixmap(QPixmap(":/img/src/images/life.png"));
    life.setScale(0.5);
    life.setPos(0,670);
    QFont font;
    font.setFamily("Comic Sans MS"); // 黑体，适配手绘风格的硬朗感
    font.setPointSize(14);
    life_Num.setFont(font);
    life_Num.move(30,670);
    life_Num.setText("x"+QString::number(1));
    life_Num.setStyleSheet("background-color: transparent;");

    this->mScene_Fight.addWidget(&this->life_Num);
    this->mScene_Fight.addWidget(&this->mButton_Pause);
    this->mScene_Fight.addItem(&this->gameItemPool.mPlane);
    this->mScene_Fight.addItem(&this->life);
}

void GameManager::initScene_Pause()
{
    this->mBackground_Pause.setPixmap(QPixmap(":/img/src/images/background.png"));
    this->mBackground_Pause.setPos(0,0);
    //暂停场景搭建
    this->mScene_Pause.setSceneRect(QRect(0,0,GameInitialConfig::MapWidth,GameInitialConfig::MapHeight));
    this->mScene_Pause.addItem(&this->mBackground_Pause);
    this->mButton_Resume.resize(60,45);
    this->mButton_Resume.move(420,0);
    // 图标尺寸建议比按钮小2-4像素，避免边缘被截断
    this->mButton_Resume.setIconSize(QSize(56,41));

    // 核心样式表：完全透明背景，无边框，仅在交互时有轻微反馈
    this->mButton_Resume.setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
        image: url(:/img/src/images/resume_nor.png);
    }
    QToolButton:pressed {

        image: url(:/img/src/images/resume_pressed.png);
    }
)");
    this->mButton_again.resize(300,41);
    this->mButton_again.setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
    }
)");
    this->mButton_again.setIcon(QIcon(":/img/src/images/again.png"));
    this->mButton_again.move(100,400);
    this->mButton_again.setIconSize(QSize(300,41));
    this->mButton_gameover.setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
    }
)");
    this->mButton_gameover.setIcon(QIcon(":/img/src/images/gameover.png"));
    this->mButton_gameover.setIconSize(QSize(300,41));
    this->mButton_gameover.resize(300,41);
    this->mButton_gameover.move(100,500);
    this->mScene_Pause.addWidget(&this->mButton_again);
    this->mScene_Pause.addWidget(&this->mButton_gameover);
    this->mScene_Pause.addWidget(&this->mButton_Resume);
}
void GameManager::timer_Start()
{
    this->timerRollBG->start(GameInitialConfig::MapRollBGTime);
    this->timerBulletMove->start(GameInitialConfig::BulletMoveTime);
    this->timerGenerateEnemy->start(GameInitialConfig::MapGenerateEnemyTime);
    this->timerShootBullet->start(1000/this->gameItemPool.mPlane.speedShootBullet);
    this->timerPlaneMove->start(30);
}
void GameManager::timer_Pause()
{
    this->timerRollBG->stop();
    this->timerBulletMove->stop();
    this->timerGenerateEnemy->stop();
    this->timerShootBullet->stop();
    this->timerPlaneMove->stop();
}
GameManager::~GameManager()
{
    delete ui;
}

void GameManager::keyPressEvent(QKeyEvent*event)
{
    this->gameItemPool.mPlane.setPixmap(QPixmap(":/img/src/images/me1.png"));
    switch (event->key()) {
    case Qt::Key_A:
    case Qt::Key_S:
    case Qt::Key_D:
    case Qt::Key_W:

        this->mKeySet.insert(event->key());
        break;
    default:
        break;
    }
}

void GameManager:: keyReleaseEvent(QKeyEvent*event)
{

    if(this->mKeySet.contains(event->key()))
    {
        this->mKeySet.remove(event->key());
    }
}

void GameManager::BGroll()//背景滚动槽函数
{
        this->mBackground1.moveBy(0,1);
        this->mBackground2.moveBy(0,1);
        if(this->mBackground1.y()>=GameInitialConfig::MapHeight)
        {
            this->mBackground1.setY(-1*GameInitialConfig::MapHeight);

        }
        else if(this->mBackground2.y()>=GameInitialConfig::MapHeight)
        {
            this->mBackground2.setY(-GameInitialConfig::MapHeight);

        }

}

void GameManager::planeMove()
{
    QSet<int>::iterator it;
    for(it=this->mKeySet.begin();it!=this->mKeySet.end();it++)
    {
        switch (*it) {
        case Qt::Key_A:this->gameItemPool.mPlane.moveBy(-1*this->gameItemPool.mPlane.speed,0);break;
        case Qt::Key_S:this->gameItemPool.mPlane.moveBy(0,1*this->gameItemPool.mPlane.speed);break;
        case Qt::Key_D:this->gameItemPool.mPlane.moveBy(1*this->gameItemPool.mPlane.speed,0);break;
        case Qt::Key_W:this->gameItemPool.mPlane.moveBy(0,-1*this->gameItemPool.mPlane.speed);break;
            break;
        default:
            break;
        }
    }
    if(this->mKeySet.isEmpty())
    {
        this->gameItemPool.mPlane.setPixmap(QPixmap(":/img/src/images/me2.png"));
    }
    if(this->gameItemPool.mPlane.x()<0)
    {
        this->gameItemPool.mPlane.setX(0);
    }
    else if(this->gameItemPool.mPlane.x()>this->width()-this->gameItemPool.mPlane.pixmap().width()/2)
    {
        this->gameItemPool.mPlane.setX(this->width()-this->gameItemPool.mPlane.pixmap().width()/2);
    }
    if(this->gameItemPool.mPlane.y()<0)
    {
        this->gameItemPool.mPlane.setY(0);
    }
    else if(this->gameItemPool.mPlane.y()>this->height()-this->gameItemPool.mPlane.pixmap().height()/2)
    {
        this->gameItemPool.mPlane.setY(this->height()-this->gameItemPool.mPlane.pixmap().height()/2);
    }
}

void GameManager::generateEnemy()
{
    if(this->gameItemPool.mEnemyList.size()<5)
    {
        this->mScene_Fight.addItem(this->gameItemPool.addEnemy());
    }

}

void GameManager::Collision()
{
    QSet<Bullet*>indexBullet;
    QSet<Enemy*>indexEnemy;
    for(int i=0;i<this->gameItemPool.mBulletList.size();i++)
    {
        for(int j=0;j<this->gameItemPool.mEnemyList.size();j++)
        {
            if(this->gameItemPool.mEnemyList[j]->dead==false&&this->gameItemPool.mBulletList[i]->type==Bullet::myBullet&&this->gameItemPool.mBulletList[i]->collidesWithItem(this->gameItemPool.mEnemyList[j]))
            {

                indexBullet.insert(this->gameItemPool.mBulletList[i]);
                this->explosion(this->gameItemPool.mEnemyList[j]);
            }

            else if(this->gameItemPool.mEnemyList[j]->dead==true&&this->gameItemPool.mEnemyList[j]->pixmapNow>this->gameItemPool.mEnemyList[j]->pictureNum)
            {

                indexEnemy.insert(this->gameItemPool.mEnemyList[j]);
                //爆炸动画结束
                disconnect(gameItemPool.mEnemyList[j]->mTimerExplosion,&QTimer::timeout,gameItemPool.mEnemyList[j],&Enemy::explosion);
                gameItemPool.mEnemyList[j]->mTimerExplosion->stop();
            }
        }
    }
    for(auto it=indexBullet.begin();it!=indexBullet.end();it++)
    {
        this->gameItemPool.removeBullet(*it);
        this->mScene_Fight.removeItem(*it);
    }

    for(auto it=indexEnemy.begin();it!=indexEnemy.end();it++)
    {
        this->gameItemPool.removeEnemy(*it);
        this->mScene_Fight.removeItem(*it);
    }

}
void GameManager:: explosion(Enemy*enemy)
{
    enemy->dead=true;
    enemy->mTimerExplosion->start(100);
    connect(enemy->mTimerExplosion,&QTimer::timeout,enemy,&Enemy::explosion);
}

