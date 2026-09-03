#include "Card.h"

Card::Card(int suit,int rank,QObject *parent)
    : QObject{parent}
{
    this->suit=suit;
    this->rank=rank;
}
