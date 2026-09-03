#ifndef CARD_H
#define CARD_H

#include <QObject>

class Card : public QObject
{
    Q_OBJECT
public:
    explicit Card(int suit=0,int rank=3,QObject *parent = nullptr);

    enum Suit {DIAMOND=0,CLUB,HEART,SPADE, JOKER };
    enum Rank {R3=3, R4, R5, R6, R7, R8, R9, R10, J, Q, K, A, R2 ,SJOKER, BJOKER};
    int suit;
    int rank;

signals:
};

#endif // CARD_H
