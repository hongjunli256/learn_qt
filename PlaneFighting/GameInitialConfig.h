#ifndef GAMEINITIALCONFIG_H
#define GAMEINITIALCONFIG_H

class GameInitialConfig//基本定义数值类
{
    GameInitialConfig();
public:
    static const int MapRollBGTime=30;
    static const int MapGenerateEnemyTime=1000;
    static const int MapWidth=480;
    static const int MapHeight=700;

    static const int BulletMoveTime=30;

    static const int PlayerShootBulletTime=1000;
    static const int PlayerMoveTime=30;

    static constexpr double Volume=0.8;

};

#endif // GAMEINITIALCONFIG_H
