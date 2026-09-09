#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QWidget>

enum class sceneType
{
	SCENESTART,
	SCENEFIGHT,
	SCENEPAUSE
};
class MyScene : public QWidget
{
	Q_OBJECT
public:
	explicit MyScene(QWidget *parent = nullptr);
	QGraphicsScene *mScene;
	QGraphicsPixmapItem *mBackground;
	QGraphicsScene *GetScene() { return mScene; }
signals:
	void needchangeScene(sceneType type);
};

#endif // MYSCENE_H
