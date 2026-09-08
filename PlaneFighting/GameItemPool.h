#ifndef GAMEITEMPOOL_H
#define GAMEITEMPOOL_H
#include <QMap>
#include <QRandomGenerator>
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
class GameItemPool
{
public:
    Player mPlane;
	QList<Bullet *> mBulletList;
	QList<Bullet *> mBulletWaitList; //墓地

	QMap<Etype, QList<Enemy *>> mEnemyListMap;
	QMap<Etype, QList<Enemy *>> mEnemyWaitMap;

	GameItemPool();
	~GameItemPool();

	Enemy *addEnemy();
	void removeEnemy(Enemy *enemy);
	Bullet*addBullet(QPoint pos,Bullet::bulletType type);
    void removeBullet(Bullet*bullet);
};

#endif // GAMEITEMPOOL_H
