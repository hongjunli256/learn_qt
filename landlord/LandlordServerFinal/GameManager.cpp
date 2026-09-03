#include "GameManager.h"
#include<QRandomGenerator>
GameManager::GameManager(QObject *parent)
    : QObject{parent}
{
    //1-4花色,5王
    //3-15 3-K,1,2，16小王，17大王
    for(int j=3;j<=15;j++)
    {
        for(int i=1;i<=4;i++)
        {
            Card*card=new Card(i,j,this);//花色，3-2
            this->Deck.append(card);
        }
    }
    Card*card=new Card(5,16,this);//小王
    this->Deck.append(card);
    card=new Card(5,17,this);
    this->Deck.append(card);
}

// 通用的 Qt 容器打乱函数（支持 QVector、QList 等）
template <typename Container>
void shuffleContainer(Container& container) {
    // Fisher-Yates 洗牌算法
    for (int i = container.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(container[i], container[j]);
    }
}

void GameManager::shuffleDeck()
{
    shuffleContainer(this->Deck);
}
