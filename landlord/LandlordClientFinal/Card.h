#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include<QMouseEvent>
#include<QApplication>
#include<QToolButton>
class Card : public QToolButton
{
    Q_OBJECT
public:
    explicit Card(int suit,int rank,QWidget *parent = nullptr);
    enum Suit {DIAMOND=0,CLUB,HEART,SPADE, JOKER };
    enum Rank {R3=3, R4, R5, R6, R7, R8, R9, R10, J, Q, K, A, R2 ,SJOKER, BJOKER};
    int suit;
    int rank;
    bool isSelected=false;
    static QPixmap& getPixmap() {
        static QPixmap pix(":/Card.png");  // 首次调用时初始化
        return pix;
    }
    int mPixX;
    int mPixY;

    void setSelected(bool selected);

    //卡片大小改变，图样也自适应
    void resizeEvent(QResizeEvent *event);
    // 重写鼠标按下事件，替代clicked信号
    void mousePressEvent(QMouseEvent *event);

    // 鼠标进入事件（处理按住左键划过选中）
    void enterEvent(QEnterEvent *event);

signals:
};

#endif // CARD_H
