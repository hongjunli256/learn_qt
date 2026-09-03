#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include"Plane.h"
class Enemy :public Plane
{
    Q_OBJECT
public:
    int type=1;

    Enemy(int type,QPoint pos);
    ~Enemy();
    QPoint direction=QPoint(0,1);

    void EnemyMove();

    QPoint shootBullet();

public slots:
    void explosion();


};

#endif // ENEMY_H
