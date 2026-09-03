#ifndef GETLANDLORDWORKER_H
#define GETLANDLORDWORKER_H
#include<QMutex>
#include<QWaitCondition>
#include <QObject>
class GetLandlordWorker : public QObject
{
    Q_OBJECT
public:
    explicit GetLandlordWorker(QWaitCondition *waitCondition=nullptr,QMutex *mutex=nullptr,QObject *parent = nullptr);
    explicit GetLandlordWorker(QWaitCondition *waitCondition,QMutex *mutex,QObject *parent,int landlordNum);
    void GetLandlord(int lastWinner,int playerNum);
    void GetWinner(int playerNum);
    bool waitFeedBack(int index);

    bool isTureReceive=false;
    bool isWantLandlord=false;
    int landlordNum=0;

    bool isFinishGame=false;
    int WinnerNum=0;

//    bool isHandOut=false;
    QString HandCardOut;

    QWaitCondition *mWaitCondition=nullptr;
    QMutex *mMutex=nullptr;
signals:
    void S_sendMessageToWho(int turn,QString info);
    void S_broadcastinfo(QString info);
    void S_WaitWhoMessage(int num);
    void S_LandlordNUm(int landlordnum);
    void S_WinnerNum(int num);
public slots:

};

#endif // GETLANDLORDWORKER_H
