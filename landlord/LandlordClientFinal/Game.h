#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include<QFileInfo>
#include"Card.h"
namespace Ui {
class Game;
}
class CardCombination
{
public:
    enum type{
        begin=0,//起始没牌状态
        Single=1,Pair,Three,Three_One,Three_two,Four_Two,Four_Two_Pair,Plane,PlaneSingle,PlanePair,//固定牌数
        Straight,StraightPair,
        Bomb};//最大

    CardCombination(CardCombination::type t,int length,int value);
    CardCombination(CardCombination &combin);
    void changeContent(CardCombination::type t,int length,int value);
    CardCombination::type t;
    int value;
    int length;
    CardCombination&operator=(const CardCombination& combination);

};
class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    QList<Card*>Hand;
    QList<Card*>choosed;
    QList<Card*>received;
    CardCombination last=CardCombination(CardCombination::type::begin,0,0);
    ~Game();
    int mMyNum=0;
    int mLandlordNum=0;
    int count_giveup=0;

    void GetHandCard(Card*card);//得到手牌

    void showThreeCard(Card*card);

    void sortHand();//理牌

    void showMyCard();

    void removeCard(Card*card);

    void startConquerLandlord();

    void enableLandlordButton();

    void disableLandlordButton();

    void startHandOut();

    void enableHandOutButton();

    void disableHandOutButton();

    bool outHand();

    void showHandOutCard(int who,QList<Card*>array);

    void showInitCardNum(int landlordNum);

    void showName_Image(QString info);

    void Changelast();

    void clearTable();

    void TimeCount(QString timeCount);
signals:
    void getLandlord();
    void giveUpLandlord();
    void handOut();
    void giveUpHandOut();

private:
    Ui::Game *ui;
};


#endif // GAME_H
