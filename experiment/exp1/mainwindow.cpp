#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    btnNums ={
        {Qt::Key_0,ui->btnNum0},
        {Qt::Key_1,ui->btnNum1},
        {Qt::Key_2,ui->btnNum2},
        {Qt::Key_3,ui->btnNum3},
        {Qt::Key_4,ui->btnNum4},
        {Qt::Key_5,ui->btnNum5},
        {Qt::Key_6,ui->btnNum6},
        {Qt::Key_7,ui->btnNum7},
        {Qt::Key_8,ui->btnNum8},
        {Qt::Key_9,ui->btnNum9}
    };

    btnCodes ={
        {Qt::Key_Plus,ui->btnPlus},
        {Qt::Key_Minus,ui->btnSub},
        {Qt::Key_Asterisk,ui->btnMul},
        {Qt::Key_Slash,ui->btnDiv}
    };

    btnSingleCodes ={
        {1,ui->btnMod},
        {2,ui->btnSqrt},
        {3,ui->btnSquare},
        {4,ui->btnFraction}
    };


    foreach(auto btn,btnNums)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    foreach(auto btn,btnCodes)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    foreach(auto btn,btnSingleCodes)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));


    // connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    // connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    // connect(ui->btnMul,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnSub,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnDiv,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    // connect(ui->btnMod,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnFraction,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation()
{

    double ans=0;
    if(operands.size()==2&&opcodes.size()>0){
        //去操作数
        double operand1=operands.front().toDouble();
        operands.pop_front();
        double operand2=operands.front().toDouble();
        operands.pop_front();

        //操作符
        QString op=opcodes.front();
        opcodes.pop_front();
        // qDebug()<<operand1<<op<<operand2<<'\n';
        if(op=="+"){
            ans=operand1+operand2;
        }else if(op=="-"){
            ans=operand1-operand2;
        }else if(op=="×"){
            ans=operand1*operand2;
        }else if(op=="÷"){
            ans=operand1/operand2;
        }
        operands.push_back(QString::number(ans));
        // operand=QString::number(ans);
    }else return operands.front();

    return QString::number(ans);

}

void MainWindow::btnNumClicked()
{
    QString digit=qobject_cast<QPushButton*>(sender())->text();


    if(operand=="0"){
        if(digit=="0")
            digit="";
        else
            operand="";
    }

    if(result!=""&&opcodes.size()==0){
        operand.clear();
        result.clear();
        operands.clear();
        opcodes.clear();
    }

    operand+=digit;

    ui->display->setText(operand);


}

void MainWindow::on_btnPoint_clicked()
{

    if(operand=="")operand ="0"+ qobject_cast<QPushButton*>(sender())->text();
    if(!operand.contains(".")){
        operand += qobject_cast<QPushButton*>(sender())->text();
    }
    ui->display->setText(operand);
}


void MainWindow::on_btnBack_clicked()
{

    operand=operand.left(operand.length()-1);
    ui->display->setText(operand);
}


void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();
    ui->display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode=qobject_cast<QPushButton*>(sender())->text();

    // qDebug()<<opcodes<<operands<<"1111\n";
    if(operand!=""){
        QString temp=operand;
        operands.push_back(operand);
        ui->display->setText(temp);
        operand.clear();

        if(operands.size()==2&&opcodes.size()>0){
            result=calculation();
            ui->display->setText(result);
        }
    }


    //在 不符合计算情况前 按其他运算符会更改
    if(opcodes.size()!=0)
        opcodes.pop_front();
    opcodes.push_back(opcode);

    //ui->statusbar->showMessage(QString("operands is %1,opcodes is %2").arg(operands.size()).arg(opcodes.size()));
    // qDebug()<<opcodes<<operands<<"2222\n";
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand==""&&operands.size()>0){
        operand=operands.front();
    }
    if(operand!=""){
        double ans=operand.toDouble();
        operand="";
        QString op =qobject_cast<QPushButton*>(sender())->text();
        if(op=="%")
            ans/=100.0;
        else if(op=="1/x")
            ans=1/ans;
        else if(op=="x²")
            ans*=ans;
        else if(op=="√")
            ans=sqrt(ans);
        operand=QString::number(ans);
        ui->display->setText(QString::number(ans));
    }
}


void MainWindow::on_btnEqual_clicked()
{
    if(operand!=""){
        operands.push_back(operand);
        operand.clear();
    }
    //ui->statusbar->showMessage(QString("operands is %1,opcodes is %2").arg(operands.size()).arg(opcodes.size()));
    if(operands.size()==1&&opcodes.size()>0)
        operands.push_back(operands.front());

    result=calculation();
    ui->display->setText(result);
    qDebug()<<result;
    operands.clear();
    operand=result;


}


void MainWindow::on_btnOpp_clicked()
{
    if(operand!="")
        operand=QString::number(-operand.toDouble());
    else
        operand=QString::number(-operands.front().toDouble());
    ui->display->setText(operand);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (auto btnKey, btnNums.keys()) {
        if(event->key()==btnKey){
            btnNums[btnKey]->animateClick();
        }
    }

    foreach (auto btnKey, btnCodes.keys()) {
        if(event->key()==btnKey){
            btnCodes[btnKey]->animateClick();
        }
    }

    if(event->key()==Qt::Key_Period)
        ui->btnPoint->animateClick();
    else if(event->key()==Qt::Key_Backspace)
        ui->btnBack->animateClick();
    else if(event->key()==Qt::Key_Percent)
        ui->btnMod->animateClick();
    // else if(event->key()==Qt::Key_Asterisk)
    //     ui->btnMul->animateClick();
    // else if(event->key()==Qt::Key_Plus)
    //     ui->btnPlus->animateClick();
    // else if(event->key()==Qt::Key_Minus)
    //     ui->btnSub->animateClick();
    // else if(event->key()==Qt::Key_Slash)
    //     ui->btnDiv->animateClick();
    else if(event->key()==Qt::Key_Equal)
        ui->btnEqual->animateClick();


}


void MainWindow::on_btnClear_clicked()
{
    operand="";
    ui->display->setText(operand);
}

