#ifndef PLAYER_H
#define PLAYER_H
#include<QGraphicsPixmapItem>
#include"Plane.h"
class Player:public Plane
{
public:
    Player();
    QPoint shootBullet();
    void explosion();
};

#endif // PLAYER_H
