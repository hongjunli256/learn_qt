#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include<QPainter>
#include<QKeyEvent>
#include<QFile>
#include<QMessageBox>
#include"publicEnum.h"
class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    int x=1;
    int y=1;
    void drawPlayer(QPainter*painter);
    int lookForward=Direction::Down;
    bool initPlayer(int level=1);
    bool initFail=true;
};

#endif // PLAYER_H
