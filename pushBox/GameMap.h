#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QWidget>
#include<QPainter>
#include<QImage>
#include<QList>
#include<QFile>
#include<QMessageBox>
/*
0,0
64x63

0,383
32x34

320,126
41x60
*/
class GameMap : public QWidget
{
    Q_OBJECT
public:
    ~GameMap();
    explicit GameMap(QWidget *parent = nullptr);
    void drawBackground(QPainter*painter);
    int**mapArr=nullptr;
    bool initMap(int level=1);
    int gridX;
    int gridY;
    void drawMap(QPainter*painter);
    QPoint boxStyle=QPoint(3,0);
    QPoint groundStyle=QPoint(1,2);
    QPoint wallStyle=QPoint(0,0);
    int dotStyle=0;
    void releaseMap();
    bool initFail=true;
signals:
};

#endif // GAMEMAP_H
