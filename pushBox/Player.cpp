#include "Player.h"
Player::Player(QWidget *parent)
    : QWidget{parent}
{
    this->initPlayer();
}
void Player:: drawPlayer(QPainter*painter)
{
    //320,126
    //37x60
    QRect imgPos;
    switch (this->lookForward) {
    case Direction::Up:
        imgPos=QRect(361,128,41,59);
        break;
    case Direction::Left:
        imgPos=QRect(320,186,41,59);
        break;
    case Direction::Down:
        imgPos=QRect(356,362,44,62);
        break;
    case Direction::Right:
        imgPos=QRect(320,128,41,59);
        break;
    }
    painter->drawImage(QRect(grid_width*x,grid_height*y+50,grid_width,grid_height),QImage("://src/gameMap.png"),imgPos);

}
bool Player:: initPlayer(int level)
{
    QString mapFileName="playerPos";
    QFile file("./maplevel/"+mapFileName+QString::number(level)+".txt");

    if(!file.open(QFile::ReadOnly))
    {
//        QMessageBox::information(this,"提示","角色文件不存在");
        this->initFail=true;
        return false;
    }
    QByteArray arr="";
    QList<QByteArray>pos;
    arr=file.readAll();
    pos=arr.split(',');
    this->x=pos[0].toInt();
    this->y=pos[1].toInt();
    file.close();
    this->initFail=false;
    return true;

}
