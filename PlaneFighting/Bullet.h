#ifndef BULLET_H
#define BULLET_H

#include "GameItem.h"
class Bullet : public GameItem
{

public:
    enum bulletType
    {
        myBullet,
        emnemyBullet
    };

    Bullet(QPoint pos,bulletType type);
    float speed=5;
    int type=bulletType::myBullet;
    QPoint direction=QPoint(0,-1) ;

    ~Bullet();

    void bulletMove();
};

#endif // BULLET_H
