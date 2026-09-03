#ifndef GETLANDLORD_WORKER_H
#define GETLANDLORD_WORKER_H
#include<QString>
class GetLandlord_Worker:
{
public:
    GetLandlord_Worker();
    //抢地主啦
    void GetLandlord(int lastwinner);
    bool waitFeedBack(int index);
    bool
};

#endif // GETLANDLORD_WORKER_H
