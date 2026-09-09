#ifndef SCENEPAUSE_H
#define SCENEPAUSE_H
#include <QLabel>
#include <QToolButton>
#include "myscene.h"
class ScenePause : public MyScene
{
	Q_OBJECT
public:
	explicit ScenePause(QWidget *parent = nullptr);
	void initScene();
	QToolButton *mButton_resume;
	QToolButton *mButton_replay;
	QToolButton *mButton_gameover;
	void initButton();

	QLabel *highest_score;
	int score = 0;
	void initLabel();
signals:
	void needReplay();
	void needGameOver();
};

#endif // SCENEPAUSE_H
