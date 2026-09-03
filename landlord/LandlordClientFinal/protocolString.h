#ifndef PROTOCOLSTRING_H
#define PROTOCOLSTRING_H
class protocolString
{
public:
    //客户端发给服务器
    const static char C_READY_GAME_S='a';//准备好了

    const static char C_HAVE_INITIAL_CARD='b';//收到了初始卡牌

    const static char C_CONQUER_LANDLORD='c';//抢地主

    const static char C_GIVEUP_CONQUER_LANDLORD='d';//不抢地主

    const static char C_KONW_WHO_IS_LANDLORD='e';//收到地主结果反馈

    const static char C_HAVE_THREE_CARD='f';//收到了三张卡牌

    const static char C_HANDOUT='g';//出牌;牌

    const static char C_GIVEUP_HANDOUT='h';//不出

    const static char C_KONW_WHO_IS_WINNER='i';//收到胜者结果

    //服务器发给客户端
    const static char S_WELCOME_TO_C='A';//被连接后进行反馈

    const static char S_ROOMFULL_C='B';//房间满了

    const static char S_PLAYER_LEAVE_C='E';//房间内玩家离开

    const static char S_START_GAME_C='F';//广播游戏开始信号，客户端切换场景

    const static char S_GIVE_INITAL_CARD_TO_C='G';//发牌



    const static char S_START_CONQUER_LANDLORD_C='H';//广播开始抢地主信号，客户端显示抢地主按钮

    const static char S_TURN_CONQUER_LANDLORD_C='I';//到客户端抢地主信号

    const static char S_WHO_CONQUER_LANDLORD_C='J';//广播某个抢地主结果;谁;抢不抢

    const static char S_FINISH_CONQUER_LANDLORD='K';//抢地主最终结果


    const static char S_THREECARD_C='L';//发三张牌了

    const static char S_START_HANDOUT_C='M';//开始出牌

    const static char S_TURN_HANDOUT_C='N';//到客户端出牌了

    const static char S_WHO_HANDOUT_WHAT_C='O';//广播某个出牌结果;谁;出了什么

    const static char S_FINISH_HANDOUT='P';//游戏最终结果

    const static char S_NAME_AND_IMAGE_C='Q';//初始化名字，图像

    const static char S_PlAY_AGAIN_C='R';//再来一局信号，要初始化

    const static char S_TIMECOUNT_C='S';//倒计时显示



};
#endif // PROTOCOLSTRING_H
