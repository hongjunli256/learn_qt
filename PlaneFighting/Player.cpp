#include "Player.h"

Player::Player()
{
    this->setPixmap(QPixmap(":/img/src/images/me2.png"));
    this->setScale(0.5);
    this->setPos(189,600);
}

QPoint Player::shootBullet()
{
    QPoint pos(this->x()+this->pixmap().width()/4-2,this->y()-10);
    return pos;
}
void Player::explosion()
{
    if(this->dead==true&&this->pixmapNow<=this->pictureNum)
    {
        this->setPixmap(QPixmap(":/img/src/images/me_destroy_"+QString::number(this->pixmapNow)+".png"));
        this->pixmapNow++;
    }
}
