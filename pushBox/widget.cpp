#include "widget.h"
#include"publicEnum.h"
//不用ui，而是使用代码进行绘制，然后写好paintevent,在监听用户操作后进行逻辑处理后调用update()
//不过倒是很奇怪当时为什么会被教着用一个成员变量来维持这个painter
//这个项目倒是很不错，用txt存储关卡信息进行映射,现在我重新挪到qrc里面吧
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(1200,750);
	//this->mPainter=new QPainter(this);

	QPushButton *button1 = new QPushButton;
	//设置按钮归属于这个窗口
    button1->setParent(this);
    //设置大小
    button1->resize(200,100);
    //使按钮显示
    button1->show();
    //给按钮加上名称
    button1->setText("下一关");
    //设置文字字体
    QFont font1("华文行楷",20,10,0);//字体，大小，加粗，倾斜
    button1->setFont(font1);
    //移动到坐标
    button1->move(800,100);

    QPushButton *button2=new QPushButton;
    button2->setParent(this);
    button2->resize(200,100);
    button2->show();
    button2->setText("退出游戏");
    button2->setFont(font1);
    button2->move(800,300);


    QPushButton *button3=new QPushButton;
    button3->setParent(this);
    button3->resize(200,100);
    button3->show();
    button3->setText("重新开始");
    button3->setFont(font1);
    button3->move(800,500);

    this->label=new QLabel(this);
    this->label->setBackgroundRole(QPalette::Highlight);
    this->label->resize(200,100);
    this->label->setText("游戏进行中");
    this->label->show();
    this->label->setFont(font1);
    this->label->move(100,600);

    this->levelLabel=new QLabel(this);
    this->levelLabel->setBackgroundRole(QPalette::Highlight);
    this->levelLabel->resize(200,50);
    this->levelLabel->setText("第 "+QString::number(this->level)+" 关");
    this->levelLabel->show();
    this->levelLabel->setFont(font1);
    this->levelLabel->move(150,0);

    connect(button1,&QPushButton::clicked,this,[this](){
        if(this->level<this->mapNum)
        {

            bool init=this->p->initPlayer(this->level+1);
            if(init==false)
            {
                QMessageBox msgBox(QMessageBox::Information,
                                   "提示",
                                   "第" + QString::number(this->level + 1) + "关角色文件不存在\n请检查文件夹maplevel配置",
                                   QMessageBox::Ok,
                                   nullptr);
                connect(&msgBox, &QMessageBox::finished, [](int) {
                    QCoreApplication::exit(1);
                });
                msgBox.exec();

            }

            this->gm->releaseMap();
            init=this->gm->initMap(this->level+1);
            if(init==false)
            {

                QMessageBox msgBox(QMessageBox::Information,
                                   "提示",
                                   "第" + QString::number(this->level + 1) + "关地图文件不存在\n请检查文件夹maplevel配置",
                                   QMessageBox::Ok,
                                   nullptr);
                connect(&msgBox, &QMessageBox::finished, [](int) {
                    QCoreApplication::exit(1);
                });
                msgBox.exec();

            }
            this->level++;
            this->label->setText("游戏进行中");
            this->levelLabel->setText("第 "+QString::number(this->level)+" 关");
            this->update();
        }

    });

    connect(button3,&QPushButton::clicked,this,[this](){
        if(this->level<=this->mapNum)
        {

            bool init=this->p->initPlayer(this->level);

            if(init==false)
            {
                QMessageBox msgBox(QMessageBox::Information,
                                   "提示",
                                   "第" + QString::number(this->level) + "关角色文件不存在\n请检查文件夹maplevel配置",
                                   QMessageBox::Ok,
                                   nullptr);
                connect(&msgBox, &QMessageBox::finished, [](int) {
                    QCoreApplication::exit(1);
                });
                msgBox.exec();

            }
            this->gm->releaseMap();
            init=this->gm->initMap(this->level);
            if(init==false)
            {
                QMessageBox msgBox(QMessageBox::Information,
                                   "提示",
                                   "第" + QString::number(this->level) + "关地图文件不存在\n请检查文件夹maplevel配置",
                                   QMessageBox::Ok,
                                   nullptr);
                connect(&msgBox, &QMessageBox::finished, [](int) {
                    QCoreApplication::exit(1);
                });
                msgBox.exec();

            }
            this->label->setText("游戏进行中");
            this->levelLabel->setText("第 "+QString::number(this->level)+" 关");
            this->update();
        }

    });

    connect(button2,&QPushButton::clicked,this,[this](){
        auto reply=QMessageBox::question(this, "退出游戏", "确定要退出游戏吗？",
                              QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QApplication::quit();
        }

    });

    //检查文件路径正常
    this->mapNum=this->countFiles("./maplevel")/2;
    qDebug()<<this->mapNum;
    if(this->mapNum==0)
    {
        QMessageBox msgBox(QMessageBox::Information,
                           "提示",
                           "文件不存在\n请检查文件夹maplevel配置",
                           QMessageBox::Ok,
                           nullptr);
        connect(&msgBox, &QMessageBox::finished, [](int) {
            QCoreApplication::exit(1);
        });
        msgBox.exec();

    }
    //建立地图
    this->gm=new GameMap(this);

    //
    this->p=new Player(this);
    if(this->gm->initFail||this->p->initFail)
    {
        QMessageBox msgBox(QMessageBox::Information,
                           "提示",
                           "第" + QString::number(this->level) + "关文件初始化失败\n请检查文件夹maplevel配置",
                           QMessageBox::Ok,
                           nullptr);
        connect(&msgBox, &QMessageBox::finished, [](int) {
            QCoreApplication::exit(1);
        });
        msgBox.exec();
    }

    return;
}

Widget::~Widget() {}

void Widget::paintEvent(QPaintEvent *event)

{
	//this->mPainter->begin(this);
	QPainter painter(this);
	this->gm->drawBackground(&painter);
	if (this->mapNum != 0)
	{
		if (!this->gm->initFail)
			this->gm->drawMap(&painter);
		if (!this->p->initFail)
			this->p->drawPlayer(&painter);
	}

	painter.end();
}
void Widget::keyPressEvent(QKeyEvent *event)
{

    //移动向量dv
    int x=0,y=0;
    //根据不同按键，设置不同移动方向
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        this->p->lookForward=Direction::Up;
        x=0;y=-1;
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        this->p->lookForward=Direction::Left;
        x=-1;y=0;
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        this->p->lookForward=Direction::Down;
        x=0;y=1;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        this->p->lookForward=Direction::Right;
        x=1;y=0;
        break;
    default:
        break;
    }
    bool finishMap=true;
    //根据移动方向上不同方块进行不同碰撞操作
    switch (this->gm->mapArr[this->p->y+y][this->p->x+x]) {
    case gridStyle::Wall:
        break;
    case gridStyle::Box:
        switch (this->gm->mapArr[this->p->y+2*y][this->p->x+2*x]) {
        //被箱子或者墙阻挡就不能推动
        case gridStyle::Box:
        case gridStyle::BoxInDot:
        case gridStyle::Wall:
            break;
        //点或者地面就成功推动箱子
        case gridStyle::Dot:
            this->gm->mapArr[this->p->y+y][this->p->x+x]=gridStyle::Ground;
            this->gm->mapArr[this->p->y+2*y][this->p->x+2*x]=gridStyle::BoxInDot;
            this->p->x+=x;
            this->p->y+=y;
            for(int i=0;i<this->gm->gridY;i++)
            {
                for(int j=0;j<this->gm->gridX;j++)
                {
                    if(this->gm->mapArr[i][j]==gridStyle::Dot)
                    {
                        finishMap=false;
                        break;
                    }
                }
                if(finishMap==false)
                {
                    break;
                }
            }
            if(finishMap==true)
            {
                this->label->setText("当前关卡已完成");
            }
            break;
        case gridStyle::Ground:

            this->gm->mapArr[this->p->y+y][this->p->x+x]=gridStyle::Ground;
            this->gm->mapArr[this->p->y+2*y][this->p->x+2*x]=gridStyle::Box;
            this->p->x+=x;
            this->p->y+=y;
            break;
        default:
            break;
        }

        break;
    case gridStyle::BoxInDot:
        switch (this->gm->mapArr[this->p->y+2*y][this->p->x+2*x]) {
        //被箱子或者墙阻挡就不能推动
        case gridStyle::Box:
        case gridStyle::BoxInDot:
        case gridStyle::Wall:
            break;
        //点或者地面就成功推动箱子
        case gridStyle::Dot:
            this->gm->mapArr[this->p->y+y][this->p->x+x]=gridStyle::Dot;
            this->gm->mapArr[this->p->y+2*y][this->p->x+2*x]=gridStyle::BoxInDot;
            this->p->x+=x;
            this->p->y+=y;
            break;
        case gridStyle::Ground:

            this->gm->mapArr[this->p->y+y][this->p->x+x]=gridStyle::Dot;
            this->gm->mapArr[this->p->y+2*y][this->p->x+2*x]=gridStyle::Box;
            this->p->x+=x;
            this->p->y+=y;
            break;
        default:
            break;
        }

        break;
        //点或者地面进行直接移动
    case gridStyle::Dot:
    case gridStyle::Ground:
        this->p->x+=x;
        this->p->y+=y;
        break;
    default:
        break;
    }
    //每次移动进行图像刷新
    this->update();
}

int Widget::countFiles(const QString &path) {
    QDir dir;
    if (!dir.exists(path)) {
        bool success = dir.mkpath(path);  // 递归创建目录
        if (success) {
            qDebug() << "目录创建成功:" << path;
        } else {
            qDebug() << "目录创建失败:" << path;
        }
        return 0;
    }
    dir.setPath(path);

    return dir.entryList(QDir::Files).count();
}
