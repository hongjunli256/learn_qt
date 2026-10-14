#ifndef SCENEFIGHT_H
#define SCENEFIGHT_H
#include <QLabel>
#include <QToolButton>
#include "GameItemPool.h"
#include "myscene.h"
class SceneFight : public MyScene
{
public:
	SceneFight();

	QToolButton *mButton_Pause;
	void initButton();

	int lifeNum_int;
	QLabel *lifeNum_label;
	void initLabel();

	QGraphicsPixmapItem *life;
	QGraphicsPixmapItem *mBackground2;
	void initGraphicsItem();
	//QSet<int> mKeySet;
	GameItemPool gameItemPool;

	void timer_Start();
	void timer_Pause();
	QTimer *timerRollBG; //背景滚动
	QTimer *timerPlaneMove;
	QTimer *timerShootBullet;
	QTimer *timerBulletMove;
	QTimer *timerGenerateEnemy;
	void initTimer();

	void Collision();
	void explosion(Plane &enemy);
	void initScene_Fight();

public slots:
	void BGroll();
	void planeMove(QSet<int> &KeySet);
	void bulletMove();
	void enemyMove();
	void shootBullet();
	void generateEnemy();
	void gameOver();
	void replay();
	void keyPressFeedback();
};

#endif // SCENEFIGHT_H
