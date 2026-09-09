#include "scenepause.h"
#include "GameInitialConfig.h"
ScenePause::ScenePause(QWidget *parent)
	: MyScene{parent}
{
	this->initScene();
}
void ScenePause::initScene()
{
	this->mBackground = new QGraphicsPixmapItem();
	this->mBackground->setPixmap(QPixmap(":/img/src/images/background.png"));
	this->mBackground->setPos(0, 0);
	//暂停场景搭建
	this->mScene = new QGraphicsScene(this);
	this->mScene->setSceneRect(QRect(0, 0, GameInitialConfig::MapWidth, GameInitialConfig::MapHeight));
	this->mScene->addItem(this->mBackground);

	this->initButton();

	this->initLabel();
	this->mScene->addWidget(this->highest_score);
}
void ScenePause::initButton()
{
	this->mButton_resume = new QToolButton();
	this->mButton_resume->resize(60, 45);
	this->mButton_resume->move(420, 0);
	// 图标尺寸建议比按钮小2-4像素，避免边缘被截断
	this->mButton_resume->setIconSize(QSize(56, 41));
	this->mButton_resume->setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
        image: url(:/img/src/images/resume_nor.png);
    }
    QToolButton:pressed {

        image: url(:/img/src/images/resume_pressed.png);
    }
	)");
	connect(this->mButton_resume, &QToolButton::clicked, this,
			[this]()
			{
				emit needchangeScene(sceneType::SCENEFIGHT);
			});
	this->mScene->addWidget(this->mButton_resume);

	this->mButton_replay = new QToolButton();
	this->mButton_replay->resize(300, 41);
	this->mButton_replay->setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
    }
	)");
	this->mButton_replay->setIcon(QIcon(":/img/src/images/again.png"));
	this->mButton_replay->move(100, 400);
	this->mButton_replay->setIconSize(QSize(300, 41));
	this->mScene->addWidget(this->mButton_replay);
	connect(this->mButton_replay, &QToolButton::clicked, this, &ScenePause::needReplay);

	this->mButton_gameover = new QToolButton();
	this->mButton_gameover->setStyleSheet(R"(
    QToolButton {
        border: none;               /* 去除边框 */
        background: transparent;    /* 透明背景 */
        padding: 0px;               /* 去除内边距 */
    }
	)");
	this->mButton_gameover->setIcon(QIcon(":/img/src/images/gameover.png"));
	this->mButton_gameover->setIconSize(QSize(300, 41));
	this->mButton_gameover->resize(300, 41);
	this->mButton_gameover->move(100, 500);
	this->mScene->addWidget(this->mButton_gameover);
	connect(this->mButton_gameover, &QToolButton::clicked, this, &ScenePause::needGameOver);
}
void ScenePause::initLabel()
{
	this->highest_score = new QLabel(this);
	QFont font;
	font.setFamily("Comic Sans MS"); // 黑体，适配手绘风格的硬朗感
	font.setPointSize(14);
	this->highest_score->setFont(font);
	this->highest_score->move(100, 300);
	this->highest_score->setStyleSheet("background-color: transparent;");
	this->highest_score->setText("最高分:" + QString::number(this->score));
}
