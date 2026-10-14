#include "scenefight.h"
#include "GameInitialConfig.h"
SceneFight::SceneFight() {}
void SceneFight::initScene_Fight() //战斗场景搭建
{
	//背景图
	this->initGraphicsItem();

	this->mScene->setSceneRect(QRect(0, 0, GameInitialConfig::MapWidth, GameInitialConfig::MapHeight));
	this->mScene->addItem(this->mBackground);
	this->mScene->addItem(this->mBackground2);

	this->initButton();

	//生命值显示

	this->mScene->addWidget(this->mButton_Pause);
	this->mScene->addItem(&this->gameItemPool.mPlane);
	this->mScene->addItem(this->life);
}
void SceneFight::initGraphicsItem()
{
	this->mBackground = new QGraphicsPixmapItem();
	this->mBackground->setPixmap(QPixmap(":/img/src/images/background.png"));
	this->mBackground->setPos(0, 0);
	this->mBackground2 = new QGraphicsPixmapItem();
	this->mBackground2->setPixmap(QPixmap(":/img/src/images/background.png"));
	this->mBackground2->setPos(0, -1 * GameInitialConfig::MapHeight);

	this->life = new QGraphicsPixmapItem();
	life->setPixmap(QPixmap(":/img/src/images/life.png"));
	life->setScale(0.5);
	life->setPos(0, 670);
	this->initLabel();
}
void SceneFight::initButton()
{
	this->mButton_Pause = new QToolButton(this);
	//暂停按钮
	this->mButton_Pause->resize(60, 45);
	this->mButton_Pause->move(420, 0);
	// 图标尺寸建议比按钮小2-4像素，避免边缘被截断
	this->mButton_Pause->setIconSize(QSize(56, 41)); //4657

	// 核心样式表：完全透明背景，无边框，仅在交互时有轻微反馈
	this->mButton_Pause->setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
        image: url(:/img/src/images/pause_nor.png);
    }
    QToolButton:pressed {

        image: url(:/img/src/images/pause_pressed.png);
    }
	)");
}
void SceneFight::initLabel()
{
	this->lifeNum_int = 3;
	this->lifeNum_label = new QLabel(this);
	QFont font;
	font.setFamily("Comic Sans MS"); // 黑体，适配手绘风格的硬朗感
	font.setPointSize(14);
	lifeNum_label->setFont(font);
	lifeNum_label->move(30, 670);
	lifeNum_label->setText("x" + QString::number(this->gameItemPool.mPlane.lifeNum));
	lifeNum_label->setStyleSheet("background-color: transparent;");
	this->mScene->addWidget(this->lifeNum_label);
}
void SceneFight::initTimer()
{
	this->timerRollBG = new QTimer(this);
	connect(this->timerRollBG, &QTimer::timeout, this, &SceneFight::BGroll);
}
void SceneFight::timer_Start()
{
	this->timerRollBG->start(GameInitialConfig::MapRollBGTime);
	this->timerBulletMove->start(GameInitialConfig::BulletMoveTime);
	this->timerGenerateEnemy->start(GameInitialConfig::MapGenerateEnemyTime);
	this->timerShootBullet->start(1000 / this->gameItemPool.mPlane.speedShootBullet);
	this->timerPlaneMove->start(30);
}
void SceneFight::timer_Pause()
{
	this->timerRollBG->stop();
	this->timerBulletMove->stop();
	this->timerGenerateEnemy->stop();
	this->timerShootBullet->stop();
	this->timerPlaneMove->stop();
}
void SceneFight::BGroll() //背景滚动槽函数
{
	this->mBackground->moveBy(0, 1);
	this->mBackground2->moveBy(0, 1);
	if (this->mBackground->y() >= GameInitialConfig::MapHeight)
	{
		this->mBackground->setY(-1 * GameInitialConfig::MapHeight);
	}
	else if (this->mBackground2->y() >= GameInitialConfig::MapHeight)
	{
		this->mBackground2->setY(-GameInitialConfig::MapHeight);
	}
}

void SceneFight::planeMove(QSet<int> &KeySet)
{
	QSet<int>::iterator it;
	for (it = KeySet.begin(); it != KeySet.end(); it++)
	{
		switch (*it)
		{
		case Qt::Key_A:
			this->gameItemPool.mPlane.moveBy(-1 * this->gameItemPool.mPlane.speed, 0);
			break;
		case Qt::Key_S:
			this->gameItemPool.mPlane.moveBy(0, 1 * this->gameItemPool.mPlane.speed);
			break;
		case Qt::Key_D:
			this->gameItemPool.mPlane.moveBy(1 * this->gameItemPool.mPlane.speed, 0);
			break;
		case Qt::Key_W:
			this->gameItemPool.mPlane.moveBy(0, -1 * this->gameItemPool.mPlane.speed);
			break;
			break;
		default:
			break;
		}
	}
	if (KeySet.isEmpty())
	{
		this->gameItemPool.mPlane.setPixmap(QPixmap(":/img/src/images/me2.png"));
	}
	if (this->gameItemPool.mPlane.x() < 0)
	{
		this->gameItemPool.mPlane.setX(0);
	}
	else if (this->gameItemPool.mPlane.x() > this->width() - this->gameItemPool.mPlane.pixmap().width() / 2)
	{
		this->gameItemPool.mPlane.setX(this->width() - this->gameItemPool.mPlane.pixmap().width() / 2);
	}
	if (this->gameItemPool.mPlane.y() < 0)
	{
		this->gameItemPool.mPlane.setY(0);
	}
	else if (this->gameItemPool.mPlane.y() > this->height() - this->gameItemPool.mPlane.pixmap().height() / 2)
	{
		this->gameItemPool.mPlane.setY(this->height() - this->gameItemPool.mPlane.pixmap().height() / 2);
	}
}
void SceneFight::generateEnemy()
{
	if (this->gameItemPool.mEnemyListMap[Etype::Boss].size() < 1)
	{
		this->mScene->addItem(this->gameItemPool.addEnemy());
	}
}
//子弹与飞机的碰撞
void SceneFight::Collision()
{
	QSet<Bullet *> indexBullet;
	QSet<Enemy *> indexEnemy;
	for (int i = 0; i < this->gameItemPool.mBulletList.size(); i++)
	{
		for (QList<Enemy *> &list : this->gameItemPool.mEnemyListMap)
		{
			for (int j = 0; j < list.size(); j++)
			{
				if (list[j]->dead == false && this->gameItemPool.mBulletList[i]->type == Bullet::myBullet && this->gameItemPool.mBulletList[i]->collidesWithItem(list[j]))
				{
					if (list[j]->lifeNum <= 1)
					{
						indexBullet.insert(this->gameItemPool.mBulletList[i]);
						this->explosion(*list[j]);
					}
					else
					{
						list[j]->lifeNum--;
					}
				}
				else if (list[j]->dead == true && list[j]->pixmapNow > list[j]->pictureNum)
				{
					indexEnemy.insert(list[j]);
					//爆炸动画结束
					disconnect(list[j]->mTimerExplosion, &QTimer::timeout, list[j], &Enemy::explosion);
					list[j]->mTimerExplosion->stop();
				}
			}
		}
	}
	Plane &player = this->gameItemPool.mPlane;
	for (int i = 0; i < this->gameItemPool.mBulletList.size(); i++)
	{
		if (player.dead == false && this->gameItemPool.mBulletList[i]->type == Bullet::emnemyBullet && this->gameItemPool.mBulletList[i]->collidesWithItem(&player))
		{
			if (player.lifeNum <= 1)
			{
				indexBullet.insert(this->gameItemPool.mBulletList[i]);
				this->explosion(player);
				// disconnect(this->timerPlaneMove, &QTimer::timeout, this,
				// 		   [this]()
				// 		   {
				// 			   this->planeMove;
				// 		   });
			}
			else
			{
				player.lifeNum--;
				lifeNum_label->setText("x" + QString::number(this->gameItemPool.mPlane.lifeNum));
			}
		}
		else if (player.dead == true && player.pixmapNow > player.pictureNum)
		{
			disconnect(player.mTimerExplosion, &QTimer::timeout, &player, &Plane::explosion);
			player.mTimerExplosion->stop();

			this->mScene->removeItem(&this->gameItemPool.mPlane);
			this->timer_Pause();
			break;
		}
	}

	for (auto it = indexBullet.begin(); it != indexBullet.end(); it++)
	{
		this->gameItemPool.removeBullet(*it);
		this->mScene->removeItem(*it);
	}

	for (QSet<Enemy *>::iterator it = indexEnemy.begin(); it != indexEnemy.end(); it++)
	{
		this->gameItemPool.removeEnemy(*it);
		this->mScene->removeItem(*it);
	}
}
void SceneFight::keyPressFeedback()
{
	this->gameItemPool.mPlane.setPixmap(QPixmap(":/img/src/images/me1.png"));
}
void SceneFight::shootBullet()
{
	this->mScene->addItem(this->gameItemPool.addBullet(this->gameItemPool.mPlane.shootBullet(), Bullet::myBullet));
	for (QList<Enemy *> &list : this->gameItemPool.mEnemyListMap)
	{
		for (Enemy *em : std::as_const(list))
		{
			if (em->y() > 0 || em->dead != true)
			{
				this->mScene->addItem(this->gameItemPool.addBullet(em->shootBullet(), Bullet::emnemyBullet));
			}
		}
	}
}
void SceneFight::enemyMove()
{
	QSet<Enemy *> indexEnemy;
	for (QList<Enemy *> &list : this->gameItemPool.mEnemyListMap)
	{
		for (Enemy *em : std::as_const(list))
		{
			if (em->dead != true)
			{
				em->EnemyMove();
			}
			if (em->y() > GameInitialConfig::MapHeight)
			{
				indexEnemy.insert(em);
			}
		}
	}
	for (QSet<Enemy *>::iterator it = indexEnemy.begin(); it != indexEnemy.end(); it++)
	{
		this->gameItemPool.removeEnemy(*it);
		this->mScene->removeItem(*it);
	}
}
void SceneFight::bulletMove()
{
	this->Collision();
	for (int i = 0; i < this->gameItemPool.mBulletList.size(); i++)
	{
		Bullet *bullet = this->gameItemPool.mBulletList[i];
		bullet->bulletMove();
		if (bullet->y() < 0 || bullet->y() > GameInitialConfig::MapHeight)
		{
			this->mScene->removeItem(bullet);
			this->gameItemPool.removeBullet(bullet);
		}
	}
}
void SceneFight::replay()
{
	for (Bullet *bullet : std::as_const(this->gameItemPool.mBulletList))
	{
		this->gameItemPool.removeBullet(bullet);
		this->mScene->removeItem(bullet);
	}
	for (QList<Enemy *> &list : this->gameItemPool.mEnemyListMap)
	{
		for (Enemy *enemy : std::as_const(list))
		{
			this->gameItemPool.removeEnemy(enemy);
			this->mScene->removeItem(enemy);
		}
	}
	this->initScene_Fight();
	this->gameItemPool.mPlane.Reset();
}
void SceneFight::explosion(Plane &enemy)
{
	enemy.dead = true;
	enemy.mTimerExplosion->start(100);
	connect(enemy.mTimerExplosion, &QTimer::timeout, &enemy, &Plane::explosion);
}
