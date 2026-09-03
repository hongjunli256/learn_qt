#ifndef PLANE_H
#define PLANE_H
#include<QGraphicsPixmapItem>
#include<QObject>
#include"GameItem.h"
#include<QTimer>
class Plane :public GameItem
{
    Q_OBJECT
public:
    Plane();
    ~Plane();
    virtual QPoint shootBullet()=0;

    float speed=2.0;
    float speedShootBullet=1;

    bool dead=false;//生命值为0后死亡dead=true，死亡后不立即删除，执行爆炸后移入对象池待命

    //爆炸辅助变量
    QTimer*mTimerExplosion;
    int pictureNum=0;
    int pixmapNow=1;

public slots:
    virtual void explosion()=0;
};

#endif // PLANE_H
