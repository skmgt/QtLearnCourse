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

    foreach(auto btn,btnNums)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));


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

    connect(ui->btnMul,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnSub,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDiv,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnMod,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnFraction,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
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
        qDebug()<<operand1<<op<<operand2<<'\n';
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
    }
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

    operand+=digit;

    ui->display->setText(operand);


}

void MainWindow::on_btnPoint_clicked()
{

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

    if(operand!=""){
        QString temp=operand;
        operands.push_back(operand);
        ui->display->setText(temp);
        operand="";
        // operand.clear();
        // opcodes.push_back(opcode);

        if(operands.size()==2&&opcodes.size()>0){
            QString result=calculation();
            ui->display->setText(result);
        }
    }

    //在 不符合计算情况前 按其他运算符会更改
    if(opcodes.size()!=0)
        opcodes.pop_front();
    opcodes.push_back(opcode);


}

void MainWindow::btnUnaryOperatorClicked()
{
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
    if(operands.size()==2&&opcodes.size()>0){
        QString result=calculation();
        ui->display->setText(result);
    }
}


void MainWindow::on_btnOpp_clicked()
{
    if(!operand.contains("-"))
        operand="-"+operand;
    else
        operand=operand.mid(1);
    ui->display->setText(operand);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (auto btnKey, btnNums.keys()) {
        if(event->key()==btnKey){
            btnNums[btnKey]->animateClick();
        }
    }
    // if(event->key()==Qt::Key_0)
    //     ui->btnNum0->animateClick();

}


void MainWindow::on_btnClear_clicked()
{
    operand="";
    ui->display->setText(operand);
}

