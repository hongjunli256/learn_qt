#include "GameMap.h"
#include"publicEnum.h"
GameMap::GameMap(QWidget *parent)
    : QWidget{parent}
{
    this->initMap();
}
void GameMap:: drawBackground(QPainter*painter)
{
    painter->drawImage(QRect(0,0,1200,750),QImage("://src/background.jpg"));
}
bool GameMap:: initMap(int level)
{
    QString mapFileName="map";
	QFile file(":/map/maplevel/" + mapFileName + QString::number(level) + ".txt");
	if(!file.open(QFile::ReadOnly))
    {
		QMessageBox::information(this, "提示", "地图文件不存在");
		this->initFail=true;
        return false;
    }
    QByteArray arr_all="";
    QList<QByteArray>arr_y;
    QList<QByteArray>arr_x;
    arr_all=file.readAll();
    arr_y=arr_all.split('\n');

    this->gridY=arr_y.size();
    this->mapArr=new int*[this->gridY];
    for(int i=0;i<this->gridY;i++)
    {
        arr_x=arr_y[i].split(',');
        this->mapArr[i]=new int [arr_x.size()];
        for(int j=0;j<arr_x.size();j++)
        {
            this->mapArr[i][j]=arr_x[j].toInt();
        }
    }
    this->gridX=arr_x.size();
    file.close();
    this->initFail=false;
    return true;
}
void GameMap::drawMap(QPainter*painter)
{
    if(this->mapArr!=nullptr)
    {
        for(int i=0;i<this->gridY;i++)
        {
            for(int j=0;j<this->gridX;j++)
            {
                painter->drawImage(QRect(j*grid_width,i*grid_height+50,grid_width,grid_height),QImage("://src/gameMap.png"),QRect(this->groundStyle.x()*64,this->groundStyle.y()*64,grid_width,grid_height));
                switch(this->mapArr[i][j])
                {
                case gridStyle::Ground:
                    break;
                case gridStyle::Box:
                    painter->drawImage(QRect(j*grid_width,i*grid_height+50,grid_width,grid_height),QImage("://src/gameMap.png"),QRect(this->boxStyle.x()*64,this->boxStyle.y()*64,grid_width,grid_height));
                    break;
                case gridStyle::Dot:
                    painter->drawImage(QRect(j*grid_width+grid_width/4,i*grid_height+grid_height/4+50,grid_width/2,grid_height/2),QImage("://src/gameMap.png"),QRect(this->dotStyle*grid_width/2,384,grid_width/2,grid_height/2));
                    break;
                case gridStyle::BoxInDot:
                    painter->drawImage(QRect(j*grid_width,i*grid_height+50,grid_width,grid_height),QImage("://src/gameMap.png"),QRect(this->boxStyle.x()*64,this->boxStyle.y()*64,grid_width,grid_height));
                    painter->drawImage(QRect(j*grid_width+grid_width/4,i*grid_height+grid_height/4+50,grid_width/2,grid_height/2),QImage("://src/gameMap.png"),QRect(this->dotStyle*grid_width/2,384,grid_width/2,grid_height/2));
                    break;
                case gridStyle::Wall:
                    painter->drawImage(QRect(j*grid_width,i*grid_height+50,grid_width,grid_height),QImage("://src/gameMap.png"),QRect(this->wallStyle.x()*64,this->wallStyle.y()*64,grid_width,grid_height));
                    break;
                }

            }
        }
    }

}
void GameMap::releaseMap()
{
    if(this->mapArr!=nullptr)
    {
        for(int i=0;i<this->gridY;i++)
        {
            if(this->mapArr[i]!=nullptr)
            {
                delete[]this->mapArr[i];
                this->mapArr[i]=nullptr;
            }
        }
        delete[]this->mapArr;
        this->mapArr=nullptr;
    }
}
GameMap::~GameMap()
{
    this->releaseMap();

}
