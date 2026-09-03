#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QStackedWidget>
#include<QTcpSocket>
#include<QLayout>
#include"Loading.h"
#include"Game.h"
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
    QStackedWidget*mStackedWidget;
    Loading*mloadingWidget;
    Game*mGameWidget;
    QTcpSocket*mSocket;
    //客户端软件启动
    void launchClient();

    //收到信息并处理
    void GetMessage(QTcpSocket *sender);
    void handleInfomation(QString &content);
    //发送信息给服务器
    void sendMessage(QString& content);

public slots:
    //客户端链接服务器
    void LinktoServer(QHostAddress addr,int portNum);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
