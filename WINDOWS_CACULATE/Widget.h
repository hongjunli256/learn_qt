#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMessageBox>
#include<QStack>
#include<QtDebug>
#include<QQueue>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

enum buttonType
{
    NUM,
    OP,
    DOT,
    CLEAR,
    CHANGE,
    BACK,
    EQUAL
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QString mNum1="";
    QString mNum2="";
    QString mOp="";
    QString mStr="";
    QStack<QString>mNumFont;
    QStack<QString>mOperator;
    void QStack_reverse(QStack<QString> &sta);

public slots:
    void clickbutton(buttonType bT,QString str);
};
#endif // WIDGET_H
