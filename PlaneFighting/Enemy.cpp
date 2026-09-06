#include "Enemy.h"
//看我等会用多态给几个敌机封装设计
Enemy::Enemy(int type,QPoint pos)
{
    this->type=type;
    this->dead=false;
    switch (type) {
    case 1:
        this->setPixmap(QPixmap(":/img/src/images/enemy1.png"));this->pictureNum=4;
        break;
    case 2:
        this->setPixmap(QPixmap(":/img/src/images/enemy2.png"));this->pictureNum=4;
        break;
    case 3:
        this->setPixmap(QPixmap(":/img/src/images/enemy3_n1.png"));this->pictureNum=6;
        break;
    default:
        break;
    }
    this->setPos(pos);
}
Enemy::~Enemy()
{
	delete this->mTimerExplosion;
}
void Enemy::EnemyMove()
{
    this->moveBy(this->direction.x()*this->speed,this->direction.y()*this->speed);
}

QPoint Enemy::shootBullet()
{
    QPoint pos(this->x()+this->pixmap().width()/2,this->y()+this->pixmap().height());
    return pos;
}

void Enemy::explosion()
{
    if(this->dead==true&&this->pixmapNow<=this->pictureNum)
    {
        this->setPixmap(QPixmap(":/img/src/images/enemy"+QString::number(this->type)+"_down"+QString::number(this->pixmapNow)+".png"));
        this->pixmapNow++;
    }
}
