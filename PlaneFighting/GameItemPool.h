#ifndef GAMEITEMPOOL_H
#define GAMEITEMPOOL_H
#include"Player.h"
#include"Enemy.h"
#include"Bullet.h"
#include <QRandomGenerator>
class GameItemPool
{
public:
    Player mPlane;
    QList<Bullet*>mBulletList;
    QList<Bullet*>mBulletWaitList;//墓地

    QList<Enemy*>mEnemyList;
    QList<Enemy*>mEnemyWaitList;//墓地

    GameItemPool();
    ~GameItemPool();

    Enemy *addEnemy();
    void removeEnemy(Enemy*enemy);
    Bullet*addBullet(QPoint pos,Bullet::bulletType type);
    void removeBullet(Bullet*bullet);
};

#endif // GAMEITEMPOOL_H
