#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include"Card.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QObject *parent = nullptr);
    QList<Card*>Deck;
    void shuffleDeck();
    int lastWinner=0;
    int landlordNum=0;


signals:
};

#endif // GAMEMANAGER_H
