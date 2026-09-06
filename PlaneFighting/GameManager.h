#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QWidget>
#include<QGraphicsPixmapItem>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QTimer>
#include<QKeyEvent>
#include<QList>
#include<QSet>

#include <QLabel>
#include <QPushButton>
#include <QToolButton>

#include"Enemy.h"
#include <QDateTime>
#include<QMediaPlayer>
#include<QAudioOutput>
#include"GameItemPool.h"
//元素-在>场景-有>视图
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class GameManager : public QWidget
{
    Q_OBJECT

public:
    GameManager(QWidget *parent = nullptr);
    ~GameManager();
    //视图与场景
    QGraphicsView mGameView;
    QGraphicsScene mScene_Fight;
    QGraphicsScene mScene_Start;
    QGraphicsScene mScene_Pause;

    //地图元素
    QGraphicsPixmapItem mBackground_Start;
    QPushButton mButton_Start;
    QGraphicsPixmapItem mBackground_Pause;
    QToolButton mButton_Pause;
    QToolButton mButton_Resume;
    QToolButton mButton_again;
    QToolButton mButton_gameover;
	QLabel highest_score;
	int score = 0;

	QLabel life_Num;
	QGraphicsPixmapItem life;

	QGraphicsPixmapItem mBackground1;
	QGraphicsPixmapItem mBackground2;

	QSet<int> mKeySet;
	GameItemPool gameItemPool;

	//定时器
    void timer_Start();
    void timer_Pause();
    QTimer*timerRollBG;//背景滚动
    QTimer*timerPlaneMove;
    QTimer*timerShootBullet;
    QTimer*timerBulletMove;
    QTimer*timerGenerateEnemy;

    //事件
    void keyPressEvent(QKeyEvent*event);
    void keyReleaseEvent(QKeyEvent*event);

    QMediaPlayer mMediaBG;
    QAudioOutput *mAudioOutputMusic;

    QMediaPlayer mMediaShoot;
    QAudioOutput *mAudioOutputSound;

    void Collision();
    void explosion(Enemy*enemy);
    void initView();
    void initScene_Fight();
    void initScene_Start();
    void initScene_Pause();
public slots:
    void BGroll();
    void planeMove();
    void generateEnemy();
    void gameOver();
    void replay();

private:
    Ui::Widget *ui;
};
#endif // GAMEMANAGER_H
