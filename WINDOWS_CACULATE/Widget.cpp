#include "Widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->NUM_00,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"00");});
    connect(ui->NUM_0,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"0");});
    connect(ui->NUM_1,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"1");});
    connect(ui->NUM_2,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"2");});
    connect(ui->NUM_3,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"3");});
    connect(ui->NUM_4,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"4");});
    connect(ui->NUM_5,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"5");});
    connect(ui->NUM_6,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"6");});
    connect(ui->NUM_7,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"7");});
    connect(ui->NUM_8,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"8");});
    connect(ui->NUM_9,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::NUM,"9");});


    connect(ui->NUM_DOT,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::DOT,".");});
    connect(ui->NUM_percent,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::CHANGE,"%");});
    connect(ui->back_button,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::BACK,"BACK");});
    connect(ui->allclear_button,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::CLEAR,"ALL_CLEAR");});
    connect(ui->cala_equal,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::EQUAL,"=");});

    connect(ui->cala_divide,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::OP,"÷");});
    connect(ui->cala_multiply,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::OP,"×");});
    connect(ui->cala_plus,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::OP,"+");});
    connect(ui->cala_sub,&QPushButton::clicked,this,[this]()->void{this->clickbutton(buttonType::OP,"-");});
    ui->cala_equal->setIcon(QIcon(":/beauty/resource/favicon.ico"));
    ui->cala_equal->setText("");
    ui->cala_equal->setIconSize(QSize(40,40));

}

Widget::~Widget()
{
    delete ui;
}
void Widget:: clickbutton(buttonType bT,QString str)
{
    // this->mStr+=str;
    // ui->show_screen->setText(this->mStr);
    double result=0;
    double num1=0;
    double num2=0;
    switch (bT) {
    case buttonType::BACK:
        if(!this->mNum1.isEmpty())
        {
            this->mNum1.chop(1);
            this->mStr.chop(1);
        }
        else if(!this->mOp.isEmpty())
        {
            this->mOp.chop(1);
            this->mStr.chop(1);
        }
        else
        {
            if(this->mStr.isEmpty())
            {
                return;
            }
            else
            {
                if(this->mNumFont.size()>=this->mOperator.size())
                {
                    this->mNum1=mNumFont.pop();
                    this->mNum1.chop(1);
                    this->mStr.chop(1);
                }
                else
                {
                    this->mOp=this->mOperator.pop();
                    this->mOp.chop(1);
                    this->mStr.chop(1);
                }
            }

        }
        break;
    case buttonType::CLEAR:

        this->mNum1="";
        this->mNum2="";
        this->mOp="";
        this->mStr="";
        this->mNumFont.clear();
        this->mOperator.clear();
        break;
    case buttonType::DOT:
        if(!this->mNum1.isEmpty()&&!this->mNum1.contains("."))
        {
            this->mNum1+=str;
            this->mStr+=str;
        }

        break;
    case buttonType::EQUAL://1÷2÷2+1出错！！

        if(this->mOperator.isEmpty())
        {
            ui->show_screen->setText("这是干什么？");
            return;
        }
        if(!this->mNum1.isEmpty())
        {
            this->mNumFont.push(this->mNum1);
            this->mNum1="";
        }
        if(this->mNumFont.size()==this->mOperator.size()+1)
        {
            this->QStack_reverse(this->mNumFont);
            this->QStack_reverse(this->mOperator);
            while(!this->mOperator.isEmpty())
            {
                if(this->mOperator.top()=="×")
                {
                    num1=this->mNumFont.top().toDouble();
                    this->mNumFont.pop();
                    num2=this->mNumFont.top().toDouble();
                    this->mOperator.pop();
                    result=num1*num2;
                    this->mNumFont.top()=QString::number(result);
                }
                else if(this->mOperator.top()=="÷")
                {
                    num1=this->mNumFont.top().toDouble();
                    this->mNumFont.pop();
                    num2=this->mNumFont.top().toDouble();
                    if(num2==0)
                    {
                        QMessageBox::warning(this,"错误","除数不能为0");
                        this->mNum1="";
                        this->mOp="";
                        this->mStr="";
                        this->mNumFont.clear();
                        this->mOperator.clear();
                        return;
                    }
                    else
                    {
                        this->mOperator.pop();
                        result=num1/num2;
                        this->mNumFont.top()=QString::number(result);
                    }
                }
                else if(this->mOperator.top()=="+"||this->mOperator.top()=="-")
                {
                    QString temp_num=this->mNumFont.top();
                    QString temp_op=this->mOperator.top();
                    this->mOperator.pop();
                    this->mNumFont.pop();
                    if(!this->mOperator.isEmpty()&&(this->mOperator.top()=="×"||this->mOperator.top()=="÷"))
                    {
                        if(this->mOperator.top()=="×")
                        {
                            num1=this->mNumFont.top().toDouble();

                            this->mNumFont.pop();
                            num2=this->mNumFont.top().toDouble();

                            this->mOperator.pop();
                            result=num1*num2;
                            this->mNumFont.top()=QString::number(result);
                        }
                        else if(this->mOperator.top()=="÷")
                        {
                            num1=this->mNumFont.top().toDouble();
                            this->mNumFont.pop();
                            num2=this->mNumFont.top().toDouble();
                            if(num1==0)
                            {
                                QMessageBox::warning(this,"错误","除数不能为0");
                                this->mNum1="";
                                this->mOp="";
                                this->mStr="";
                                this->mNumFont.clear();
                                this->mOperator.clear();
                                return;
                            }
                            else
                            {
                                this->mOperator.pop();
                                result=num1/num2;
                                this->mNumFont.top()=QString::number(result);
                            }
                        }
                        this->mOperator.push(temp_op);
                        this->mNumFont.push(temp_num);
                    }
                    else if(temp_op=="+")
                    {
                        num1=temp_num.toDouble();
                        num2=this->mNumFont.top().toDouble();
                        result=num1+num2;
                        this->mNumFont.top()=QString::number(result);
                    }
                    else if(temp_op=="-")
                    {
                        num1=temp_num.toDouble();

                        num2=this->mNumFont.top().toDouble();
                        result=num1-num2;
                        this->mNumFont.top()=QString::number(result);
                    }
                }
            }
            ui->show_screen->setText(QString::number(result));
            this->mNum1=QString::number(result);
            this->mOp="";
            this->mStr=QString::number(result);
            this->mNumFont.clear();
            this->mOperator.clear();
        }


        return;

        break;

    case buttonType::NUM:
            this->mNum1+=str;
            this->mStr+=str;
            if(!this->mOp.isEmpty())
            {
                this->mOperator.push(this->mOp);
                this->mOp="";
            }

        break;
    case buttonType::OP:
        if(this->mOp.isEmpty()&&!this->mNum1.isEmpty())
        {
            this->mOp+=str;
            this->mStr+=str;
            this->mNumFont.push(this->mNum1);
            this->mNum1="";
        }
        break;
    case buttonType::CHANGE:
        if(!this->mNum1.isEmpty())
        {
            this->mStr.chop(this->mNum1.size());
            this->mNum1=QString::number(-1*this->mNum1.toInt());
            this->mStr+=this->mNum1;
        }
        break;
    default:
        break;
    }
        ui->show_screen->setText(this->mStr);
}
void Widget::QStack_reverse(QStack<QString> &sta)
{
    QQueue<QString>ql;
    while(!sta.isEmpty())
    {
        ql.push_back(sta.pop());
    }
    while(!ql.isEmpty())
    {
        sta.push(ql.front());
        ql.pop_front();
    }
}

