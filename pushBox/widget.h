#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPainter>
#include<QTimer>
#include"GameMap.h"
#include"Player.h"
#include<QPushButton>
#include<QLabel>
#include<QApplication>
#include <QDir>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    QPainter*mPainter=nullptr;
    GameMap *gm=nullptr;
    Player*p=nullptr;
    QLabel *label=nullptr;
    QLabel *levelLabel=nullptr;
    int mapNum=0;
    int level=1;
//    QTimer*timer;

    int countFiles(const QString &path);

};
#endif // WIDGET_H
