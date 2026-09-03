#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QHostInfo>
#include<QTcpServer>
#include<QTcpSocket>
#include<QMessageBox>
#include<QList>
#include<QMap>
#include"GameManager.h"
#include<QMutex>
#include<QWaitCondition>
#include<QThread>
#include"GetLandlordWorker.h"
#include<QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    GetLandlordWorker*getLandlordWorker;
    GameManager*GM=nullptr;
    //服务器
    QTcpServer*mTcpServer=nullptr;

    QTimer*mTimerCount=nullptr;
    int timeCount=10;

    //显示服务器主机地址
    void prepareForLoading();
    //发送给客户端
    void sendMessage(QTcpSocket *socket,QString& content);
    //广播给客户端
    void broadcastMessage(QString content);
    //读取客户端信息
    void GetMessage(QTcpSocket *sender);
    void handleInfomation(QString &content);


    QMutex mMutex;
    QMutex mMutexForData;
    QWaitCondition mWaitCondition;  // 等待条件
    int waitWho=0;
    int receiveWho=0;

    QList<QTcpSocket*>mListSocket;
    QList<bool>mListAreYouReady;
    //新客户链接服务器
    void newPlayerComing();
    //客户端断开链接
    void playerLeave(QTcpSocket *sender);

    //发牌啦
    void giveInitialCard();
    void giveThreeCard();
signals:
    void signalTimerStart();
    void signalTimerStop();
public slots:
    //启动服务器的监听
    void launchServer();
    void sendMessageToWho(int turn,QString info);
    void waitWhoMessage(int num);
    void GetLandlordNum(int num);
    void GetWinner(int num);
    void TimerStart();
    void TimerStop();
private:
    QThread*mLandlordThread=nullptr;
    Ui::Widget *ui;
};
#endif // WIDGET_H
