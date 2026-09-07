#include "GameItemPool.h"

GameItemPool::GameItemPool() {}
GameItemPool::~GameItemPool() {
    for(int i=0;i<this->mBulletWaitList.size();i++)
    {
        delete this->mBulletWaitList[i];
    }
	for (int i = 0; i < this->mSoldierWaitList.size(); i++)
	{
		delete this->mSoldierWaitList[i];
	}
	for (int i = 0; i < this->mEliteWaitList.size(); i++)
	{
		delete this->mEliteWaitList[i];
	}
	for (int i = 0; i < this->mBossWaitList.size(); i++)
	{
		delete this->mBossWaitList[i];
	}
}
Enemy *GameItemPool::addEnemy()
{
    Enemy *enemy;
	int type = QRandomGenerator::global()->bounded(1, 3);
	int posX = QRandomGenerator::global()->bounded(50, 480 - 50);
	int posY=QRandomGenerator::global()->bounded(-100,-50);
	switch (type)
	{
	case 1:
		if (this->mSoldierWaitList.isEmpty())
		{
			enemy = new Soldier(QPoint(posX, posY));
		}
		else
		{
			enemy = this->mSoldierWaitList.first();
			this->mSoldierWaitList.removeFirst();
			enemy->Reset(posX, posY);
			this->mSoldierList.append(enemy);
		}
		break;
	case 2:
		if (this->mEliteWaitList.isEmpty())
		{
			enemy = new Elite(QPoint(posX, posY));
		}
		else
		{
			enemy = this->mEliteWaitList.first();
			this->mEliteWaitList.removeFirst();
			enemy->Reset(posX, posY);
			this->mEliteList.append(enemy);
		}
		break;
	case 3:
		if (this->mBossWaitList.isEmpty())
		{
			enemy = new Boss(QPoint(posX, posY));
		}
		else
		{
			enemy = this->mBossWaitList.first();
			this->mBossWaitList.removeFirst();
			enemy->Reset(posX, posY);
			this->mBossList.append(enemy);
		}

		break;
	default:
		break;
	}
	return enemy;
}
void GameItemPool::removeEnemy(Enemy*enemy)
{
	switch (enemy->GetType())
	{
	case Etype::Soldier:
		this->mSoldierList.removeOne(enemy);
		this->mSoldierWaitList.append(enemy);
		break;
	case Etype::Elite:
		this->mEliteList.removeOne(enemy);
		this->mEliteWaitList.append(enemy);
		break;
	case Etype::Boss:
		this->mBossList.removeOne(enemy);
		this->mBossWaitList.append(enemy);
		break;
	}
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
