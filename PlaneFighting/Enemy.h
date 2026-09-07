#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include"Plane.h"
enum class Etype
{
	Soldier,
	Elite,
	Boss
};
class Enemy :public Plane
{
    Q_OBJECT
public:
	Enemy(QPoint pos);
	~Enemy();
	QPoint direction = QPoint(0, 1);

	void EnemyMove();

	virtual QPoint shootBullet();
	void Reset(int posX, int posY);
	virtual void subReset() = 0;
	virtual Etype GetType() = 0;
public slots:
	virtual void explosion() = 0;
};

class Soldier : public Enemy
{
	Q_OBJECT
public:
	Soldier(QPoint pos);
	~Soldier();

	virtual QPoint shootBullet();
	virtual void subReset();
	virtual Etype GetType() { return Etype::Soldier; };
public slots:
	virtual void explosion();
};

class Elite : public Enemy
{
	Q_OBJECT
public:
	Elite(QPoint pos);
	~Elite();

	virtual QPoint shootBullet();
	virtual void subReset();
	virtual Etype GetType() { return Etype::Elite; };
public slots:
	virtual void explosion();
};

class Boss : public Enemy
{
	Q_OBJECT
public:
	Boss(QPoint pos);
	~Boss();

	virtual QPoint shootBullet();
	virtual void subReset();
	virtual Etype GetType() { return Etype::Boss; };
public slots:
	virtual void explosion();
};
#endif // ENEMY_H
