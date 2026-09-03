#ifndef GAMEITEM_H
#define GAMEITEM_H

#include<QGraphicsPixmapItem>
//上层的游戏物体类，注意析构链使用场景自动，不要用对象树


class GameItem :  public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    GameItem();

};

#endif // GAMEITEM_H
