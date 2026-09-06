#include "GameItemPool.h"

GameItemPool::GameItemPool() {}
GameItemPool::~GameItemPool() {
    for(int i=0;i<this->mBulletWaitList.size();i++)
    {
        delete this->mBulletWaitList[i];
    }
    for(int i=0;i<this->mEnemyWaitList.size();i++)
    {
        delete this->mEnemyWaitList[i];
    }
}
Enemy *GameItemPool::addEnemy()
{
    Enemy *enemy;
	int type = QRandomGenerator::global()->bounded(1, 3);
	int posX = QRandomGenerator::global()->bounded(50, 480 - 50);
	int posY=QRandomGenerator::global()->bounded(-100,-50);
    if(this->mEnemyWaitList.isEmpty())
    {

        enemy=new Enemy(type,QPoint(posX,posY));
    }
    else
    {
        enemy=this->mEnemyWaitList.first();
        this->mEnemyWaitList.removeFirst();

        enemy->setPos(posX,posY);
        enemy->type=type;
        enemy->dead=false;
        enemy->pixmapNow=1;
        switch (type) {
        case 1:
            enemy->setPixmap(QPixmap(":/img/src/images/enemy1.png"));enemy->pictureNum=4;
            break;
        case 2:
            enemy->setPixmap(QPixmap(":/img/src/images/enemy2.png"));enemy->pictureNum=4;
            break;
        case 3:
            enemy->setPixmap(QPixmap(":/img/src/images/enemy3_n1.png"));enemy->pictureNum=6;
            break;
        default:
            break;
        }
    }
    this->mEnemyList.append(enemy);
    return enemy;
}
void GameItemPool::removeEnemy(Enemy*enemy)
{

    this->mEnemyList.removeOne(enemy);
    this->mEnemyWaitList.append(enemy);
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
