#include "scenestart.h"
#include "GameInitialConfig.h"
SceneStart::SceneStart(QWidget *parent)
	: MyScene{parent}
{
	this->initScene();
}

void SceneStart::initScene()
{ //起始场景搭建
	this->mBackground = new QGraphicsPixmapItem();
	this->mBackground->setPixmap(QPixmap(":/img/src/images/background.png"));
	this->mBackground->setPos(0, 0);

	this->mScene = new QGraphicsScene(this);
	this->mScene->setSceneRect(QRect(0, 0, GameInitialConfig::MapWidth, GameInitialConfig::MapHeight));
	this->mScene->addItem(this->mBackground);

	this->initButton();
	this->mScene->addWidget(&this->mButton_Start);
}

void SceneStart::initButton()
{
	this->mButton_Start.resize(160, 50);
	this->mButton_Start.setText("游戏开始");
	this->mButton_Start.move(160, 500);
	// 字体设置
	QFont font;
	font.setFamily("Comic Sans MS");
	font.setPointSize(14);
	font.setBold(true);
	this->mButton_Start.setFont(font);
	// 样式表设置
	this->mButton_Start.setStyleSheet(R"(
    QPushButton {
        color: #333333; /* 深灰色文字，和背景协调 */
        background-color: rgba(255, 255, 255, 80); /* 白色半透明背景，柔和不突兀 */
        border: 2px solid #cccccc; /* 浅灰边框，模拟手绘线条感 */
        padding: 0px;
    }
    QPushButton:hover {
        background-color: rgba(255, 255, 255, 120); /* 悬停时白色更明显 */
        border-color: #aaaaaa; /* 边框稍深，突出交互 */
    }
    QPushButton:pressed {
        background-color: rgba(240, 240, 240, 120); /* 按下时浅灰，有按压反馈 */
        padding: 9px 14px 7px 16px; /* 轻微位移，增强按压感 */
    }
    QPushButton:focus {
        outline: none; /* 去除焦点虚线，保持简洁 */
    })");
	connect(&this->mButton_Start, &QPushButton::clicked, this,
			[this]()
			{
				emit needchangeScene(sceneType::SCENEFIGHT);
			});
}
