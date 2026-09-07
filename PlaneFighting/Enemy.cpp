#include "Enemy.h"
//看我等会用多态给几个敌机封装设计
Enemy::Enemy(QPoint pos)
{
    this->dead=false;
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
void Enemy::Reset(int posX, int posY)
{
	this->setPos(posX, posY);
	this->dead = false;
	this->pixmapNow = 1;
	this->subReset();
}

Soldier::Soldier(QPoint pos)
	: Enemy(pos)
{
	this->setPixmap(QPixmap(":/img/src/images/enemy1.png"));
	this->pictureNum = 4;
}
void Soldier::explosion()
{
	if (this->dead == true && this->pixmapNow <= this->pictureNum)
	{
		this->setPixmap(QPixmap(":/img/src/images/enemy1_down" + QString::number(this->pixmapNow) + ".png"));
		this->pixmapNow++;
	}
}
void Soldier::subReset()
{
	this->setPixmap(QPixmap(":/img/src/images/enemy1.png"));
	this->pictureNum = 4;
}

Elite::Elite(QPoint pos)
	: Enemy(pos)
{
	this->setPixmap(QPixmap(":/img/src/images/enemy2.png"));
	this->pictureNum = 4;
}
void Elite::explosion()
{
	if (this->dead == true && this->pixmapNow <= this->pictureNum)
	{
		this->setPixmap(QPixmap(":/img/src/images/enemy2_down" + QString::number(this->pixmapNow) + ".png"));
		this->pixmapNow++;
	}
}
void Elite::subReset()
{
	this->setPixmap(QPixmap(":/img/src/images/enemy2.png"));
	this->pictureNum = 4;
}
Boss::Boss(QPoint pos)
	: Enemy(pos)
{
	this->setPixmap(QPixmap(":/img/src/images/enemy3_n1.png"));
	this->pictureNum = 6;
}
void Boss::explosion()
{
	if (this->dead == true && this->pixmapNow <= this->pictureNum)
	{
		this->setPixmap(QPixmap(":/img/src/images/enemy3_down" + QString::number(this->pixmapNow) + ".png"));
		this->pixmapNow++;
	}
}
void Boss::subReset()
{
	this->setPixmap(QPixmap(":/img/src/images/enemy3_n1.png"));
	this->pictureNum = 6;
}
