#include "Bullet.h"

Bullet::Bullet(QPoint pos,bulletType type)
{
    switch(type)
    {
    case myBullet:
        this->setPixmap(QPixmap(":/img/src/images/bullet2.png"));
        this->direction=QPoint(0,-1);
        break;

    case emnemyBullet:
        this->setPixmap(QPixmap(":/img/src/images/bullet1.png"));
        this->direction=QPoint(0,1);
        break;

    }
    this->type=type;
    this->setPos(pos);

}
Bullet::~Bullet()
{

}
void Bullet::bulletMove()
{
    this->moveBy(this->direction.x()*this->speed,this->direction.y()*this->speed);
}
