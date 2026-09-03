#include "Widget.h"
#include "./ui_Widget.h"
#include"protocolString.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //服务器启动
    this->prepareForLoading();

    //服务器开始监听
    connect(this->ui->buttonLaunchServer,&QPushButton::clicked,this,&Widget::launchServer);

    connect(mTcpServer,&QTcpServer::newConnection,this,&Widget::newPlayerComing);

    connect(this->mTimerCount,&QTimer::timeout,this,[this](){
        if(this->timeCount>0)
        {
            this->timeCount--;
        }
        QString info(protocolString::S_TIMECOUNT_C);
        info.append(";"+QString::number(timeCount));
        this->broadcastMessage(info);
    });
    connect(this,&Widget::signalTimerStart,this,&Widget::TimerStart);
    connect(this,&Widget::signalTimerStop,this,&Widget::TimerStop);

}
void Widget::TimerStart()
{
    this->mTimerCount->start(1000);
}
void Widget::TimerStop()
{
    this->mTimerCount->stop();
}
void Widget::GetLandlordNum(int num)
{
    this->GM->landlordNum=num;
    QString info(protocolString::S_FINISH_CONQUER_LANDLORD);
    info.append(";"+QString::number(num));
    this->broadcastMessage(info);
}
void Widget::GetWinner(int num)
{
    this->GM->lastWinner=num;
    QString info(protocolString::S_FINISH_HANDOUT);//结束出牌
    info.append(";"+QString::number(num));
    this->broadcastMessage(info);

    QString info2(protocolString::S_PlAY_AGAIN_C);
    info2.append(";");
    this->broadcastMessage(info2);
}

void Widget::waitWhoMessage(int num)
{
    this->waitWho=num;
    this->timeCount=11;//显示为10~1
}
void Widget::sendMessageToWho(int turn,QString info)
{
    this->sendMessage(this->mListSocket[turn],info);
}
Widget::~Widget()
{
    delete ui;
}
void Widget::prepareForLoading()
{
    //服务器基本元素初始化
    this->mTcpServer = new QTcpServer(this);
    this->GM=new GameManager(this);
    this->mTimerCount=new QTimer(this);
    //显示主机信息
    QString hostName=QHostInfo::localHostName();//获取本机姓名
    QHostInfo info=QHostInfo::fromName(hostName);//构建主机信息对象
    QList<QHostAddress>listAddress=info.addresses();//有多条，一般是ipv4和ipv6并存
    QString text;
    text.append(hostName);
    text.append("\n");
    foreach(QHostAddress ad,listAddress)
    {
        text.append("显示主机信息:\n");
        text.append(ad.toString()+"\n");
    }
    this->ui->textEditHostInfo->setText(text);


}
void Widget::launchServer()
{
    // 1. 获取用户输入的字符串
    QString portStr = this->ui->lineEdit->text().trimmed(); // 去除首尾空格

    // 2. 验证是否为空
    if (portStr.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "端口号不能为空！");
        return;
    }

    // 3. 验证是否为纯数字（避免字母、符号等无效字符）
    bool isNumber;
    int port = portStr.toInt(&isNumber); // isNumber为true表示转换成功
    if (!isNumber) {
        QMessageBox::warning(this, "输入错误", "请输入有效的数字！别搞，哥们");
        this->ui->lineEdit->clear(); // 清空错误输入
        return;
    }

    // 4. 验证端口是否在合法范围（1025~65535）
    if (port < 1025 || port > 65535) {
        QMessageBox::warning(this, "输入错误", "端口号必须在1025~65535之间！");
        this->ui->lineEdit->clear(); // 清空错误输入
        return;
    }

    // 5. 验证通过，启动服务器
    bool isListening = this->mTcpServer->listen(QHostAddress::Any, port);

    // 6. 检查监听是否成功（可能因端口被占用等原因失败）
    if (isListening) {
        this->ui->buttonLaunchServer->setEnabled(false);
    } else {
        delete this->mTcpServer; // 启动失败，释放资源
        this->mTcpServer = nullptr;
    }
}

//发送给客户端
void Widget::sendMessage(QTcpSocket *socket,QString& content)
{
    if(!socket||!socket->isOpen())
    {
        return;
    }
    int length=content.size();
    socket->write((char*)&length,sizeof(length));
    socket->write(content.toUtf8());
    socket->flush();
}

//广播给客户端
void Widget::broadcastMessage(QString content)
{
    for(int i=0;i<this->mListSocket.size();i++)
    {
        sendMessage(this->mListSocket[i],content);
    }
}
//收到信息
void Widget::GetMessage(QTcpSocket *sender)
{
    if(!sender||!sender->isOpen())
    {
        QMessageBox::information(this,"提示","无法完成信息发送，请检查链接");
        return;
    }
    int length=0;
    while (sender->bytesAvailable() >= sizeof(int)) { // 至少能读取长度字段时才处理
        int length = 0;
        sender->read((char*)&length, sizeof(length));
        QByteArray data = sender->read(length);
        QString content = QString::fromUtf8(data);
        this->handleInfomation(content);
    }
}
void Widget::handleInfomation(QString &content)
{
    QStringList parts = content.split(";", Qt::KeepEmptyParts);
    if (parts.isEmpty()) return;

    QString type = parts[0]; // 获取类型标识
    QString contentNext = content.mid(type.length() + 1);
    if(type==protocolString::C_READY_GAME_S)//进完房间，下一步->发牌
    {
        if(this->mListAreYouReady[parts[1].toInt()-1]!=1)
        {
            this->mListAreYouReady[parts[1].toInt()-1]=1;
            QString info1(protocolString::S_NAME_AND_IMAGE_C);
            info1.append(";"+contentNext);
            this->broadcastMessage(info1);
            if(!this->mListAreYouReady.contains(0))//这里还要加上3人数的限制，为了方便测试就暂时不加
            {
                QString info2(protocolString::S_START_GAME_C);
                this->broadcastMessage(info2);
                for(int i=0;i<this->mListAreYouReady.size();i++)
                {
                    this->mListAreYouReady[i]=false;
                }
                this->giveInitialCard();
            }
        }

    }
    else if(type==protocolString::C_HAVE_INITIAL_CARD)//发完牌，下一步->叫地主
    {
        this->mListAreYouReady[parts[1].toInt()-1]=1;
        if(!this->mListAreYouReady.contains(0))
        {
            QString info(protocolString::S_START_CONQUER_LANDLORD_C);
            this->broadcastMessage(info);
            for(int i=0;i<this->mListAreYouReady.size();i++)
            {
                this->mListAreYouReady[i]=false;
            }
            // 启动子线程执行GetLandlord，避免阻塞主线程
            if (!mLandlordThread) {
                this->getLandlordWorker=new GetLandlordWorker(&this->mWaitCondition,&this->mMutex,nullptr);

                connect(this->getLandlordWorker,&GetLandlordWorker::S_sendMessageToWho,this,&Widget::sendMessageToWho);
                connect(this->getLandlordWorker,&GetLandlordWorker::S_broadcastinfo,this,&Widget::broadcastMessage);
                connect(this->getLandlordWorker,&GetLandlordWorker::S_WaitWhoMessage,this,&Widget::waitWhoMessage);
                connect(this->getLandlordWorker,&GetLandlordWorker::S_LandlordNUm,this,&Widget::GetLandlordNum);

                mLandlordThread = new QThread(this);
                this->getLandlordWorker->moveToThread(this->mLandlordThread);

                // 在线程启动后执行GetLandlord
                connect(mLandlordThread, &QThread::started, this->getLandlordWorker, [this](){
                    emit this->signalTimerStart();
                    this->getLandlordWorker->GetLandlord(this->GM->lastWinner,this->mListSocket.size());
                });

                // 线程结束后自动释放资源
                connect(mLandlordThread, &QThread::finished, mLandlordThread, &QThread::deleteLater);
                connect(mLandlordThread, &QThread::finished, getLandlordWorker,&GetLandlordWorker::deleteLater);
                connect(mLandlordThread, &QThread::finished, this, [this]() {
                    emit this->signalTimerStop();
                    mLandlordThread = nullptr; // 重置指针，允许下次创建
                    getLandlordWorker=nullptr;
                });

                mLandlordThread->start(); // 启动子线程
            }

        }
    }
    else if(type==protocolString::C_CONQUER_LANDLORD)
    {
        QMutexLocker locker(&this->mMutex);

        this->receiveWho=contentNext.toInt();
        if(this->receiveWho==this->waitWho)
        {
            this->waitWho=0;
            this->getLandlordWorker->isTureReceive=true;
            this->getLandlordWorker->isWantLandlord=true;
        }
        else
        {
            this->getLandlordWorker->isTureReceive=false;
            this->getLandlordWorker->isWantLandlord=true;
        }
        mWaitCondition.wakeOne(); // 只唤醒一个等待的线程
    }
    else if(type==protocolString::C_GIVEUP_CONQUER_LANDLORD)
    {
        QMutexLocker locker(&this->mMutex);
        this->receiveWho=contentNext.toInt();
        if(this->receiveWho==this->waitWho)
        {
            this->waitWho=0;
            this->getLandlordWorker->isTureReceive=true;
            this->getLandlordWorker->isWantLandlord=false;
        }
        else
        {
            this->getLandlordWorker->isTureReceive=false;
            this->getLandlordWorker->isWantLandlord=false;
        }
        mWaitCondition.wakeOne(); // 只唤醒一个等待的线程
    }
    else if(type==protocolString::C_KONW_WHO_IS_LANDLORD)//咱们都知道谁是地主了，下一步发三张牌
    {
        this->mListAreYouReady[contentNext.toInt()-1]=1;
        if(!this->mListAreYouReady.contains(0))
        {
            QString info(protocolString::S_THREECARD_C);
            this->broadcastMessage(info);
            for(int i=0;i<this->mListAreYouReady.size();i++)
            {
                this->mListAreYouReady[i]=false;
            }
            //QMessageBox::information(this,"提示","翻开三张牌吗");
            this->giveThreeCard();
        }
    }
    else if(type==protocolString::C_HAVE_THREE_CARD)//都知道三张牌了就开始出牌吧
    {
        this->mListAreYouReady[contentNext.toInt()-1]=1;
        if(!this->mListAreYouReady.contains(0))
        {
            QString info(protocolString::S_START_HANDOUT_C);
            this->broadcastMessage(info);
            for(int i=0;i<this->mListAreYouReady.size();i++)
            {
                this->mListAreYouReady[i]=false;
            }
            // 启动子线程执行GetLandlord，避免阻塞主线程
            if (!mLandlordThread) {

                this->getLandlordWorker=new GetLandlordWorker(&this->mWaitCondition,&this->mMutex,nullptr,this->GM->landlordNum);

                connect(this->getLandlordWorker,&GetLandlordWorker::S_sendMessageToWho,this,&Widget::sendMessageToWho);
                connect(this->getLandlordWorker,&GetLandlordWorker::S_broadcastinfo,this,&Widget::broadcastMessage);
                connect(this->getLandlordWorker,&GetLandlordWorker::S_WaitWhoMessage,this,&Widget::waitWhoMessage);
                connect(this->getLandlordWorker,&GetLandlordWorker::S_WinnerNum,this,&Widget::GetWinner);
                mLandlordThread = new QThread(this);
                this->getLandlordWorker->moveToThread(this->mLandlordThread);

                // 在线程启动后执行GetLandlord
                connect(mLandlordThread, &QThread::started, this->getLandlordWorker, [this](){
                    emit this->signalTimerStart();
                    this->getLandlordWorker->GetWinner(this->mListSocket.size());
                });

                // 线程结束后自动释放资源
                connect(mLandlordThread, &QThread::finished, mLandlordThread, &QThread::deleteLater);
                connect(mLandlordThread, &QThread::finished, getLandlordWorker,&GetLandlordWorker::deleteLater);
                connect(mLandlordThread, &QThread::finished, this, [this]() {
                    emit this->signalTimerStop();
                    mLandlordThread = nullptr; // 重置指针，允许下次创建
                    getLandlordWorker=nullptr;
                });

                mLandlordThread->start(); // 启动子线程
            }

        }
     }
    else if(type==protocolString::C_HANDOUT)
    {
        QMutexLocker locker(&this->mMutex);
        QStringList partsTemp = contentNext.split(";", Qt::KeepEmptyParts);
        if (partsTemp.isEmpty()) return;

        QString info(protocolString::S_WHO_HANDOUT_WHAT_C);
        info.append(";");
        info.append(contentNext);
        this->broadcastMessage(info);

        QString who = partsTemp[0]; // 获取类型标识
        QString handOutInfo = contentNext.mid(who.length() + 1);

        this->receiveWho=who.toInt();
        if(this->receiveWho==this->waitWho)
        {
            this->waitWho=0;
            this->getLandlordWorker->isTureReceive=true;
            this->getLandlordWorker->HandCardOut=handOutInfo;
        }
        else
        {
            this->getLandlordWorker->isTureReceive=false;
            this->getLandlordWorker->HandCardOut=handOutInfo;
        }
        mWaitCondition.wakeOne(); // 只唤醒一个等待的线程
    }
    else if(type==protocolString::C_GIVEUP_HANDOUT)
    {
         QMutexLocker locker(&this->mMutex);
         QStringList partsTemp = contentNext.split(";", Qt::KeepEmptyParts);
         if (partsTemp.isEmpty()) return;

         QString info(protocolString::S_WHO_HANDOUT_WHAT_C);
         info.append(";");
         info.append(contentNext);
         this->broadcastMessage(info);

         QString who = partsTemp[0]; // 获取类型标识

        this->receiveWho=who.toInt();
        if(this->receiveWho==this->waitWho)
        {
            this->waitWho=0;
            this->getLandlordWorker->isTureReceive=true;
            this->getLandlordWorker->HandCardOut="";
        }
        else
        {
            this->getLandlordWorker->isTureReceive=false;
            this->getLandlordWorker->isWantLandlord="";
        }
        mWaitCondition.wakeOne(); // 只唤醒一个等待的线程
    }

}

//有新玩家接入
void Widget::newPlayerComing()
{
    QTcpSocket*newPlayer=this->mTcpServer->nextPendingConnection();
    if(this->mListSocket.size()<3)
    {
        this->mListSocket.append(newPlayer);
        this->mListAreYouReady.append(0);
        //QMessageBox::information(this,"提示","玩家"+QString::number(this->mListSocket.size())+"进入房间");
        connect(newPlayer,&QTcpSocket::readyRead,this,[this,newPlayer](){
            this->GetMessage(newPlayer);
        });
        connect(newPlayer,&QTcpSocket::disconnected,this,[this,newPlayer](){
            this->playerLeave(newPlayer);
        });

        QString feedback(protocolString::S_WELCOME_TO_C);
        feedback.append(";"+QString::number(this->mListSocket.size()));
        sendMessage(newPlayer,feedback);

    }
    else
    {
        QString info(protocolString::S_ROOMFULL_C);
        info.append(";");
        this->sendMessage(newPlayer,info);
    }
}
//玩家离开
void Widget::playerLeave(QTcpSocket*socket)
{
    // 2. 检查该客户端是否在我们的连接列表中
    if (mListSocket.contains(socket))
    {
        // 计算该客户端的“玩家编号”（方便提示）
        int playerIndex = mListSocket.indexOf(socket) + 1;

        // 3. 从列表中移除（避免后续操作野指针）
        mListSocket.removeOne(socket);
        mListAreYouReady.removeAt(playerIndex-1);
        // 4. 释放资源（deleteLater()：延迟释放，避免信号处理中崩溃）
        socket->deleteLater();
        QString leaveMsg(protocolString::S_PLAYER_LEAVE_C); // 自定义协议
        leaveMsg.append(";" + QString::number(playerIndex));
        broadcastMessage(leaveMsg);
    }
}
void Widget::giveInitialCard()
{
    this->GM->shuffleDeck();//洗牌
    for(int j=0;j<this->mListSocket.size();j++)
    {
        QString CardInfo(protocolString::S_GIVE_INITAL_CARD_TO_C);
        CardInfo.append(";");
        for(int i=0;i<17;i++)//临时单人版本，发给socket[0]
        {
            CardInfo.append(QString::number(this->GM->Deck[i+j*17]->suit));
            CardInfo.append(",");

            CardInfo.append(QString::number(this->GM->Deck[i+j*17]->rank));
            CardInfo.append(";");
        }
        this->sendMessage(this->mListSocket[j],CardInfo);
    }

}
void Widget::giveThreeCard()
{
    QString CardInfo(protocolString::S_THREECARD_C);
    CardInfo.append(";");

    for(int i=51;i<54;i++)//临时单人版本，发给socket[0]
    {
        CardInfo.append(QString::number(this->GM->Deck[i]->suit));
        CardInfo.append(",");

        CardInfo.append(QString::number(this->GM->Deck[i]->rank));
        CardInfo.append(";");
    }
    this->broadcastMessage(CardInfo);
}
