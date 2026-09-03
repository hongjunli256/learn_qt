#include "Card.h"
#include"CardPicConfig.h"
Card::Card(int suit,int rank,QWidget *parent)
    : QToolButton{parent}
{
    // 未选中状态 - 恢复默认样式
    this->setStyleSheet(
        "QToolButton {"
        "border: 1px solid #CCCCCC;"
        "border-radius: 4px;"
        "background-color: white;"
        "}"
        "QToolButton:hover {"
        "border: 1px solid #999999;"
        "}");
    this->isSelected=false;
    this->suit=suit;
    this->rank=rank;

    if(rank<=15)
    {
        this->mPixX=CardPicConfig::initPicWidth+rank%3*207*4+(suit-1)*207;
        this->mPixY=(rank/3-1)*300;
    }
    else if(rank==16)
    {
        this->mPixX=CardPicConfig::initPicWidth+rank%3*207*4;
        this->mPixY=(rank/3-1)*300;
    }
    else if(rank==17)
    {
        this->mPixX=CardPicConfig::initPicWidth+(rank-1)%3*207*4+207;
        this->mPixY=(rank/3-1)*300;
    }

    this->suit = suit;
    this->rank = rank;
    // 设置自适应大小策略（在构造函数中添加）
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // 设置最小尺寸限制，防止过小
    this->setMinimumSize(30,45);
    // 设置最大尺寸限制，防止过大
    this->setMaximumSize(60, 90);

    QPixmap Cardpix = this->getPixmap().copy(mPixX, mPixY,
                                             CardPicConfig::cardPixWidth,
                                             CardPicConfig::cardPixHeight);
    if(!Cardpix.isNull())
    {
        QPixmap scaledPix = Cardpix.scaled(this->size(),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation);
        this->setIconSize(this->size());
        this->setIcon(QIcon(scaledPix));
    }

}
void Card::resizeEvent(QResizeEvent *event)
{

    QToolButton::resizeEvent(event);
    // 获取原始卡牌图片
    QPixmap Cardpix = this->getPixmap().copy(mPixX, mPixY,
                                             CardPicConfig::cardPixWidth,
                                             CardPicConfig::cardPixHeight);
    // 当按钮大小变化时，重新缩放图标
    if(!Cardpix.isNull())
    {
        QPixmap scaledPix = Cardpix.scaled(this->size(),
                                           Qt::IgnoreAspectRatio,
                                           Qt::SmoothTransformation);
        this->setIconSize(this->size());
        this->setIcon(QIcon(scaledPix));
    }
}

// 重写鼠标按下事件，替代clicked信号
void Card::mousePressEvent(QMouseEvent *event)
{
    // 先调用父类的事件处理（确保按钮的其他默认行为正常）
    QToolButton::mousePressEvent(event);

    // 只处理左键点击
    if (event->button() == Qt::LeftButton) {
        // 点击时切换选中状态
        this->setSelected(!this->isSelected);

    }

}

void Card::enterEvent(QEnterEvent *event)
{
    // Qt6 中直接传递 QEnterEvent* 给父类，无需类型转换
    QToolButton::enterEvent(event);
    // 检查是否是鼠标进入事件，且左键处于按下状态
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        this->setSelected(!this->isSelected);
    }
}
void Card::setSelected(bool selected)
{
    this->isSelected=selected;
    if (this->isSelected) {
        // 选中状态 - 显示金色高光边框
        this->setStyleSheet(
            "QToolButton {"
            "border: 3px solid #FFD700;"  // 金色边框
            "border-radius: 4px;"         // 圆角
            "background-color: rgba(255, 215, 0, 0.2);"  // 轻微金色背景
            "}"
            "QToolButton:hover {"
            "border: 3px solid #FFA500;"  // 鼠标悬停时更深的金色
            "}");
    }
    else
    {
        // 未选中状态 - 恢复默认样式
        this->setStyleSheet(
            "QToolButton {"
            "border: 1px solid #CCCCCC;"
            "border-radius: 4px;"
            "background-color: white;"
            "}"
            "QToolButton:hover {"
            "border: 1px solid #999999;"
            "}");
    }

}
