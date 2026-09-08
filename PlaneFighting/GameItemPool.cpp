#include "GameItemPool.h"

GameItemPool::GameItemPool()
{
	this->mEnemyListMap[Etype::Soldier];
	this->mEnemyListMap[Etype::Elite];
	this->mEnemyListMap[Etype::Boss];
	this->mEnemyWaitMap[Etype::Soldier];
	this->mEnemyWaitMap[Etype::Elite];
	this->mEnemyWaitMap[Etype::Boss];
}
GameItemPool::~GameItemPool() {
    for(int i=0;i<this->mBulletWaitList.size();i++)
    {
        delete this->mBulletWaitList[i];
    }
	this->mBulletWaitList.clear();
	for (QList<Enemy *> &list : this->mEnemyWaitMap)
	{
		for (int i = 0; i < list.size(); i++)
		{
			delete list[i];
		}
		list.clear();
	}
}
Enemy *GameItemPool::addEnemy()
{
	Enemy *enemy = nullptr;
	int type_int = QRandomGenerator::global()->bounded(1, 4);
	Etype type{};
	int posX = QRandomGenerator::global()->bounded(50, 480 - 50);
	int posY=QRandomGenerator::global()->bounded(-100,-50);
	switch (type_int)
	{
	case 1:
		type = Etype::Soldier;
		break;
	case 2:
		type = Etype::Elite;
		break;
	case 3:
		type = Etype::Boss;
		break;
	default:
		type = Etype::Soldier;
		break;
	}
	QList<Enemy *> &list = this->mEnemyWaitMap[type];
	if (list.isEmpty())
	{
		switch (type)
		{
		case Etype::Soldier:
			enemy = new Soldier(QPoint(posX, posY));
			break;
		case Etype::Elite:
			enemy = new Elite(QPoint(posX, posY));
			break;
		case Etype::Boss:
			enemy = new Boss(QPoint(posX, posY));
			break;
		default:
			enemy = new Soldier(QPoint(posX, posY));
			break;
		}
	}
	else
	{
		enemy = list.first();
		list.removeFirst();
		enemy->Reset(posX, posY);
	}
	this->mEnemyListMap[type].append(enemy);
	return enemy;
}

void GameItemPool::removeEnemy(Enemy*enemy)
{
	Etype type = enemy->GetType();
	this->mEnemyListMap[type].removeOne(enemy);
	this->mEnemyWaitMap[type].append(enemy);
}
Bullet*GameItemPool::addBullet(QPoint pos,Bullet::bulletType type)
{
    Bullet *bullet;
    if(this->mBulletWaitList.isEmpty())
    {
        bullet=new Bullet(pos,type);
    }
    else
    {
        bullet=this->mBulletWaitList.first();
        this->mBulletWaitList.removeFirst();
        bullet->setPos(pos);
        bullet->type=type;
        switch(type)
        {
        case Bullet::myBullet:
            bullet->setPixmap(QPixmap(":/img/src/images/bullet2.png"));
            bullet->direction=QPoint(0,-1);
            break;

        case Bullet::emnemyBullet:
            bullet->setPixmap(QPixmap(":/img/src/images/bullet1.png"));
            bullet->direction=QPoint(0,1);
            break;

        }


    }
    this->mBulletList.append(bullet);
    return bullet;
}

void GameItemPool::removeBullet(Bullet*bullet)
{
    this->mBulletList.removeOne(bullet);
    this->mBulletWaitList.append(bullet);
}
