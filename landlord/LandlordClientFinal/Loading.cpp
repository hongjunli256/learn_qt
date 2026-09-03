#include "Loading.h"
#include "ui_Loading.h"

Loading::Loading(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Loading)
{
    ui->setupUi(this);
    //点击链接按钮
    this->imagePath=":/1s.png";
    this->ui->imageButton->setIconSize(this->ui->imageButton->size());
    this->ui->imageButton->setIcon(QIcon(this->imagePath));
    connect(this->ui->buttonConnectServer,&QPushButton::clicked,this,&Loading::ConnectServer);
    //点击准备按钮
    connect(this->ui->buttonReady,&QPushButton::clicked,this,&Loading::buttonReadyClicked);

    connect(this->ui->comboBox,&QComboBox::currentIndexChanged,this,&Loading::changeImage);

    connect(this->ui->lineEditName,&QLineEdit::textChanged,this,[this](){
        this->name=this->ui->lineEditName->text();
    });

}
//点击链接服务器按钮
void Loading::ConnectServer()
{
    // 1. 获取输入内容并去除首尾空格
    QString ipStr = this->ui->lineEditHostAdress->text().trimmed();
    QString portStr = this->ui->lineEditPort->text().trimmed();

    // 2. 验证IP和端口是否为空
    if (ipStr.isEmpty() || portStr.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "IP地址和端口号不能为空！");
        return;
    }

    // 3. 验证IP地址格式是否合法
    QHostAddress ipAddr;
    if (!ipAddr.setAddress(ipStr)) {
        if (ipStr.compare("localhost", Qt::CaseInsensitive) != 0) {
            QMessageBox::warning(this, "输入错误", "请输入有效的IP地址（如127.0.0.1或localhost）！");
            return;
        }
        ipAddr = QHostAddress(QHostAddress::LocalHost);
    }
    // 4. 验证端口号是否为有效数字
    bool isPortValid;
    int portNum = portStr.toInt(&isPortValid);
    if (!isPortValid) {
        QMessageBox::warning(this, "输入错误", "端口号必须是数字！");
        return;
    }
    // 5. 验证端口号范围（1025-65535）
    if (portNum <1025 || portNum > 65535) {
        QMessageBox::warning(this, "输入错误", "端口号必须在1025-65535之间！");
        return;
    }
    emit this->buttonConnectServerClicked(QHostAddress(ipStr),portNum);
}
void Loading::changeImage(int index)
{
    this->imagePath=":/"+QString::number(index+1)+"s.png";
    this->ui->imageButton->setIconSize(this->ui->imageButton->size());
    this->ui->imageButton->setIcon(QIcon(this->imagePath));
}
//使能链接服务器的按钮
void Loading:: setEnableConnectButton(bool isable)
{
    this->ui->buttonConnectServer->setEnabled(isable);
}

Loading::~Loading()
{
    delete ui;
}
