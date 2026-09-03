#include "GetLandlordWorker.h"
#include "protocolString.h"
#include<QThread>
GetLandlordWorker::GetLandlordWorker(QWaitCondition *waitCondition,QMutex *mutex,QObject *parent)
    : QObject{parent}
{
    this->mWaitCondition=waitCondition;
    this->mMutex=mutex;
}
GetLandlordWorker::GetLandlordWorker(QWaitCondition *waitCondition,QMutex *mutex,QObject *parent,int landlordNum)
    : QObject{parent}
{
    this->mWaitCondition=waitCondition;
    this->mMutex=mutex;
    this->landlordNum=landlordNum;
}

bool GetLandlordWorker::waitFeedBack(int index)
{
    emit this->S_WaitWhoMessage(index);
    this->isTureReceive=false;
    QMutexLocker locker(this->mMutex);
    while (!this->isTureReceive) {
        bool timeout = !mWaitCondition->wait(this->mMutex, 10000);
        if (timeout) {
            return false;
        }
    }
    return true;
}

void GetLandlordWorker::GetLandlord(int lastWinner,int playerNum)
{
    QString information(protocolString::S_TURN_CONQUER_LANDLORD_C);//
    int turn=lastWinner;
    for(int i=0;i<4;i++)
    {
        this->isWantLandlord=false;
        if(turn>=playerNum)
        {
            turn=0;
        }
        emit this->S_sendMessageToWho(turn,information);
        bool isRight=this->waitFeedBack(turn+1);

        if(isRight&&this->isWantLandlord==true)
        {
            this->landlordNum=turn+1;
        }
        else
        {
        }
        QString info(protocolString::S_WHO_CONQUER_LANDLORD_C);//谁干了什么

        info.append(";"+QString::number(turn)+";"+QString::number(this->isWantLandlord));

        emit this->S_broadcastinfo(info);
        turn++;
    }
    emit this->S_LandlordNUm(this->landlordNum);
    QThread::currentThread()->quit();
    return;
}
void GetLandlordWorker::GetWinner(int playerNum)
{
    QString information(protocolString::S_TURN_HANDOUT_C);//
    information.append(";");
    int cardNumArray[3]={17,17,17};
    int turn=landlordNum-1;
    cardNumArray[turn]=20;
    int tempWinner=turn;
    int giveUpCount=0;
    while(!this->isFinishGame)
    {
        giveUpCount=0;
        while(giveUpCount<=1&&!this->isFinishGame)
        {
            this->HandCardOut="";
            if(turn>=playerNum)
            {
                turn=0;
            }
            emit this->S_sendMessageToWho(turn,information);
            bool isRight=this->waitFeedBack(turn+1);
            if(isRight&&!this->HandCardOut.isEmpty())
            {
                giveUpCount=0;
            }
            else
            {
                if(giveUpCount==0)
                {
                    giveUpCount=1;
                }
                else if(giveUpCount==1)
                {
                    tempWinner=turn+1;
                    if(tempWinner>=playerNum)
                    {
                        tempWinner=0;
                    }
                    break;
                }
            }
            cardNumArray[turn]-=this->HandCardOut.count(";");
            if(cardNumArray[turn]==0)
            {
                this->isFinishGame=true;
                this->WinnerNum=turn+1;
            }
            turn++;
        }
        turn=tempWinner;
    }
    emit this->S_WinnerNum(this->WinnerNum);
    QThread::currentThread()->quit();
    return;
}

