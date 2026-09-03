#include "Widget.h"
#include "./ui_Widget.h"
#include"protocolString.h"

//收尾工作
//一、分数书写
//一、文件，指针，数据，安全性防护
//二、冗余代码函数封装
//三、室友电脑测试
//五、readme书写，并封装为程序
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->launchClient();
    //链接服务器按钮
    connect(this->mloadingWidget,&Loading::buttonConnectServerClicked,this,&Widget::LinktoServer);

    connect(this->mloadingWidget,&Loading::buttonReadyClicked,this,[this](){
        QString info(protocolString::C_READY_GAME_S);
        info.append(";"+QString::number(this->mGameWidget->mMyNum));
        info.append(";"+this->mloadingWidget->name);
        info.append(";"+this->mloadingWidget->imagePath);
        this->sendMessage(info);
    });

    connect(this->mSocket,&QTcpSocket::readyRead,this,[this](){this->GetMessage(this->mSocket);});

    connect(this->mGameWidget,&Game::getLandlord,this,[this](){
        this->mGameWidget->disableLandlordButton();
        QString info(protocolString::C_CONQUER_LANDLORD);
        info.append(";");
        info.append(QString::number(this->mGameWidget->mMyNum));

        this->sendMessage(info);
    });
    connect(this->mGameWidget,&Game::giveUpLandlord,this,[this](){
        this->mGameWidget->disableLandlordButton();
        QString info(protocolString::C_GIVEUP_CONQUER_LANDLORD);
        info.append(";");
        info.append(QString::number(this->mGameWidget->mMyNum));


        this->sendMessage(info);
    });

    QString CardInfo(protocolString::S_GIVE_INITAL_CARD_TO_C);
    CardInfo.append(";");

    connect(this->mGameWidget,&Game::handOut,this,[this](){
        if(this->mGameWidget->outHand())
        {
            this->mGameWidget->disableHandOutButton();
            for(int i=0;i<this->mGameWidget->choosed.size();i++)
            {
                this->mGameWidget->Hand.removeOne(this->mGameWidget->choosed[i]);
                this->mGameWidget->removeCard(this->mGameWidget->choosed[i]);
            }
            this->mGameWidget->showMyCard();
            QString info(protocolString::C_HANDOUT);
            info.append(";");
            info.append(QString::number(this->mGameWidget->mMyNum));

            info.append(";");
            foreach(Card*card,this->mGameWidget->choosed)
            {
                info.append(QString::number(card->suit));
                info.append(",");

                info.append(QString::number(card->rank));
                info.append(";");
            }

            this->sendMessage(info);
        }

    });
    connect(this->mGameWidget,&Game::giveUpHandOut,this,[this](){
        this->mGameWidget->disableHandOutButton();
        QString info(protocolString::C_GIVEUP_HANDOUT);
        info.append(";");
        info.append(QString::number(this->mGameWidget->mMyNum));
        info.append(";");

        this->sendMessage(info);
    });
}

Widget::~Widget()
{
    delete ui;
}

//初始化loading和game场景以及mSocket
void Widget::launchClient()//
{
    //场景以及部件的初始化
    this->mloadingWidget=new Loading(this);
    this->mGameWidget=new Game(this);
    this->mStackedWidget=new QStackedWidget(this);
    this->mStackedWidget->addWidget(this->mloadingWidget);
    this->mStackedWidget->addWidget(this->mGameWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(this); // 创建垂直布局
    mainLayout->setContentsMargins(0, 0, 0, 0); // 去除布局边缘间距（可选）
    mainLayout->addWidget(this->mStackedWidget); // 将栈窗口添加到布局
    // 设置Widget的布局（此时stackedWidget会自动填充整个Widget）
    this->setLayout(mainLayout);

    //mSocket的初始化
    this->mSocket=new QTcpSocket(this);
}
void Widget::LinktoServer(QHostAddress addr,int portNum)
{
    this->mSocket->connectToHost(addr,portNum);
    //QMessageBox::information(this,"提示","尝试与"+addr.toString()+"的服务器建立链接");
}
void Widget::GetMessage(QTcpSocket *sender)
{
    if(!sender||!sender->isOpen())
    {
        //QMessageBox::information(this,"提示","无法完成信息接受，请检查链接");
        return;
    }
    int length=0;
    while (sender->bytesAvailable() >= sizeof(int)) { // 至少能读取长度字段时才处理
        // 1. 读取长度
        int length = 0;
        sender->read((char*)&length, sizeof(length));
        // 3. 读取内容
        QByteArray data = sender->read(length);
        QString content = QString::fromUtf8(data);

        // 4. 处理消息
        this->handleInfomation(content);
    }
}
void Widget::handleInfomation(QString &content)
{
    QStringList parts = content.split(";", Qt::KeepEmptyParts);
    if (parts.isEmpty()) return;

    QString type = parts[0]; // 获取类型标识
    QString contentNext = content.mid(type.length() + 1);
    if(type==protocolString::S_ROOMFULL_C)
    {
        QMessageBox::information(this,"提示","房间满了");
    }
    else if(type==protocolString::S_WELCOME_TO_C)
    {
        //QMessageBox::information(this,"提示","成功进入房间，你是"+contentNext+"号玩家");
        this->mGameWidget->mMyNum=contentNext.toInt();
        this->mloadingWidget->setEnableConnectButton(false);
    }
    else if(type==protocolString::S_NAME_AND_IMAGE_C)
    {
        //QMessageBox::information(this,"开始游戏","全员确认完毕，让我们开始吧");
        this->mGameWidget->showName_Image(contentNext);

    }
    else if(type==protocolString::S_START_GAME_C)
    {
        //QMessageBox::information(this,"开始游戏","全员确认完毕，让我们开始吧");
        this->mStackedWidget->setCurrentIndex(1);

    }
    else if(type==protocolString::S_GIVE_INITAL_CARD_TO_C)
    {
        QStringList CardInfo = contentNext.split(";", Qt::KeepEmptyParts);
        // 2. 遍历每张牌的字符串（过滤空字符串）
        foreach (QString cardStr, CardInfo) {
            cardStr = cardStr.trimmed(); // 去除空格
            if (cardStr.isEmpty()) continue; // 跳过空值
            QStringList parts = cardStr.split(",");
            if (parts.size() != 2) continue; // 格式错误则跳过

            // 4. 转换为整数,两bool保证数字正确
            bool suitOk, rankOk;
            int suit = parts[0].toInt(&suitOk);
            int rank = parts[1].toInt(&rankOk);

            // 5. 验证转换结果，创建Card对象
            if (suitOk && rankOk) {
                Card* card = new Card(suit,rank); // 假设Card有默认构造函数
                this->mGameWidget->GetHandCard(card);
            }
        }
        if(this->mGameWidget->Hand.size()==17)
        {
            QString info(protocolString::C_HAVE_INITIAL_CARD);
            info.append(";");
            info.append(QString::number(this->mGameWidget->mMyNum));
            this->sendMessage(info);
            this->mGameWidget->showMyCard();
            //QMessageBox::information(this,"提示","收到起始卡片");
        }
    }
    else if(type==protocolString::S_START_CONQUER_LANDLORD_C)
    {
        //QMessageBox::information(this,"提示","开始抢地主");
        this->mGameWidget->startConquerLandlord();
    }
    else if(type==protocolString::S_TURN_CONQUER_LANDLORD_C)
    {
        //QMessageBox::information(this,"提示","抢不抢");
        this->mGameWidget->enableLandlordButton();

    }
    else if(type==protocolString::S_WHO_CONQUER_LANDLORD_C)
    {
        //谁抢了地主，可以做个动画表示
    }
    else if(type==protocolString::S_FINISH_CONQUER_LANDLORD)
    {
        this->mGameWidget->mLandlordNum=contentNext.toInt();
        this->mGameWidget->showInitCardNum(this->mGameWidget->mLandlordNum);
        QString info(protocolString::C_KONW_WHO_IS_LANDLORD);
        info.append(";"+QString::number(this->mGameWidget->mMyNum));
        this->sendMessage(info);
    }
    else if(type==protocolString::S_THREECARD_C)
    {

        QStringList CardInfo = contentNext.split(";", Qt::KeepEmptyParts);
        // 2. 遍历每张牌的字符串（过滤空字符串）
        foreach (QString cardStr, CardInfo) {
            cardStr = cardStr.trimmed(); // 去除空格
            if (cardStr.isEmpty()) continue; // 跳过空值
            QStringList parts = cardStr.split(",");
            if (parts.size() != 2) continue; // 格式错误则跳过

            // 4. 转换为整数,两bool保证数字正确
            bool suitOk, rankOk;
            int suit = parts[0].toInt(&suitOk);
            int rank = parts[1].toInt(&rankOk);

            // 5. 验证转换结果，创建Card对象
            if (suitOk && rankOk) {
                // 假设Card有默认构造函数
                if(this->mGameWidget->mMyNum==this->mGameWidget->mLandlordNum)
                {
                    Card* card = new Card(suit,rank);
                    this->mGameWidget->GetHandCard(card);
                    Card* cardThree = new Card(suit,rank);
                    this->mGameWidget->showThreeCard(cardThree);
                }
                else
                {
                    Card* card = new Card(suit,rank);
                    this->mGameWidget->showThreeCard(card);
                }

            }
        }
        if(this->mGameWidget->mMyNum==this->mGameWidget->mLandlordNum)
        {
            this->mGameWidget->showMyCard();
        }

        QString info(protocolString::C_HAVE_THREE_CARD);
        info.append(";");
        info.append(QString::number(this->mGameWidget->mMyNum));
        this->sendMessage(info);
    }
    else if(type==protocolString::S_START_HANDOUT_C)
    {
        //QMessageBox::information(this,"提示","开始抢地主");
        this->mGameWidget->startHandOut();
    }
    else if(type==protocolString::S_TURN_HANDOUT_C)
    {
        //QMessageBox::information(this,"提示","抢不抢");
        this->mGameWidget->enableHandOutButton();

    }
    else if(type==protocolString::S_WHO_HANDOUT_WHAT_C)
    {
        //contemtNext=QString::number(turn)+";"+QString::number(this->isWantLandlord,index和bool值进行动画处理就行
        //根据这个结果做动画就行
        QStringList partsTemp = contentNext.split(";", Qt::KeepEmptyParts);
        if (partsTemp.isEmpty()) return;

        QString who = partsTemp[0]; // 获取类型标识
        QString handOutInfo = contentNext.mid(who.length() + 1);
        if(handOutInfo.isEmpty())
        {
                switch(this->mGameWidget->count_giveup)
                {
                case 0:
                    this->mGameWidget->count_giveup++;
                    break;
                case 1:
                    this->mGameWidget->count_giveup=0;
                    this->mGameWidget->last.changeContent(CardCombination::type::begin,0,0);
                    break;
                }
            this->mGameWidget->received.clear();
            this->mGameWidget->showHandOutCard(who.toInt(),this->mGameWidget->received);
            return;
        }
        else
        {
            this->mGameWidget->count_giveup=0;
            this->mGameWidget->received.clear();
            // 2. 遍历每张牌的字符串（过滤空字符串）
            QStringList cardList=handOutInfo.split(";", Qt::KeepEmptyParts);
            foreach (QString cardStr, cardList) {
                cardStr = cardStr.trimmed(); // 去除空格
                if (cardStr.isEmpty()) continue; // 跳过空值
                QStringList parts = cardStr.split(",");
                if (parts.size() != 2) continue; // 格式错误则跳过

                // 4. 转换为整数,两bool保证数字正确
                bool suitOk, rankOk;
                int suit = parts[0].toInt(&suitOk);
                int rank = parts[1].toInt(&rankOk);

                // 5. 验证转换结果，创建Card对象
                if (suitOk && rankOk) {
                    Card* card = new Card(suit,rank); // 假设Card有默认构造函数
                    this->mGameWidget->received.append(card);
                }
            }
            this->mGameWidget->Changelast();
            this->mGameWidget->showHandOutCard(who.toInt(),this->mGameWidget->received);

        }

    }
    else if(type==protocolString::S_FINISH_HANDOUT)
    {
        //contentNext=
        this->mGameWidget->mLandlordNum=contentNext.toInt();
        QString info(protocolString::C_KONW_WHO_IS_WINNER);
        info.append(";"+QString::number(this->mGameWidget->mMyNum));
        this->sendMessage(info);
        //加一个胜者动画以及豆子结算
        QMessageBox::information(this,"结算",contentNext+"胜利");
    }
    else if(type==protocolString::S_PlAY_AGAIN_C)
    {
        //QMessageBox::information(this,"开始游戏","全员确认完毕，让我们开始吧");
        this->mStackedWidget->setCurrentIndex(0);
        this->mGameWidget->clearTable();

    }
    else if(type==protocolString::S_TIMECOUNT_C)
    {
        this->mGameWidget->TimeCount(contentNext);
    }




}


//发送给服务器
void Widget::sendMessage(QString& content)
{
    if(!this->mSocket||!this->mSocket->isOpen())
    {
        QMessageBox::information(this,"提示","无法完成信息发送，请检查链接");
        return;
    }
    int length=content.size();
    mSocket->write((char*)&length,sizeof(length));
    mSocket->write(content.toUtf8());
    mSocket->flush();
    return;
}

