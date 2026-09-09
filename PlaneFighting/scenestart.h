#ifndef SCENESTART_H
#define SCENESTART_H

#include <QPushButton>
#include "myscene.h"
class SceneStart : public MyScene
{
public:
	explicit SceneStart(QWidget *parent = nullptr);
	void initScene();
	QPushButton mButton_Start;
	void initButton();
};

#endif // SCENESTART_H
