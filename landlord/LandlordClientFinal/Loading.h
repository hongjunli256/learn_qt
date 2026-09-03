#ifndef LOADING_H
#define LOADING_H

#include <QWidget>
#include<QHostAddress>
#include<QMessageBox>
namespace Ui {
class Loading;
}

class Loading : public QWidget
{
    Q_OBJECT

public:
    explicit Loading(QWidget *parent = nullptr);
    void setEnableConnectButton(bool isable);
    QString name="";//玩家昵称
    QString imagePath=":/1s.png";//玩家头像
    ~Loading();
public slots:
    void ConnectServer();
signals:
    void buttonConnectServerClicked(QHostAddress addr,int portNum);//链接服务器按钮
    void buttonReadyClicked();//准备按钮
public slots:
    void changeImage(int index);


private:
    Ui::Loading *ui;
};

#endif // LOADING_H
