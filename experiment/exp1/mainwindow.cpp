#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

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

QString MainWindow::calculation(bool *ok)
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
    ui->display->setText(operand);
}

void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode=qobject_cast<QPushButton*>(sender())->text();

    if(operand!=""){
        operands.push_back(operand);
        operand.clear();
        opcodes.push_back(opcode);
        QString result=calculation();

        ui->display->setText(result);
    }


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

        ui->display->setText(QString::number(ans));
    }
}


void MainWindow::on_btnEqual_clicked()
{
    if(operand!=""){
        operands.push_back(operand);
        operand.clear();
    }

    QString result=calculation();

    ui->display->setText(result);
}

