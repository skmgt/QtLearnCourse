#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
#include "idatabase.h"
MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    goLoginView();

    IDatabase::getInstance();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    qDebug()<<"gologinView";
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    connect(loginView,SIGNAL(loginSuccess()),this,SLOT(goWelcomeView()));
}

void MasterView::goWelcomeView()
{
    qDebug()<<"goWelcomeView";
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    connect(welcomeView,SIGNAL(goDoctorView()),this,SLOT(goDoctorView()));
    connect(welcomeView,SIGNAL(goPatientView()),this,SLOT(goPatientView()));
    connect(welcomeView,SIGNAL(goDepartmentView()),this,SLOT(goDepartmentView()));
}

void MasterView::goDoctorView()
{
    qDebug()<<"goDoctorView";
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);
}

void MasterView::goDepartmentView()
{
    qDebug()<<"goDepartmentView";
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void MasterView::goPatientEditView(int row)
{
    qDebug()<<"goPatientEditView";
    patientEditView = new PatientEditView(this,row);
    pushWidgetToStackView(patientEditView);

    connect(patientEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goPatientView()
{
    qDebug()<<"goPatientView";
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);
    connect(patientView,SIGNAL(goPatientEditView(int)),this,SLOT(goPatientEditView(int)));
}

void MasterView::goPreviousView()
{
    int count = ui->stackedWidget->count();
    if(count >1){
        ui->stackedWidget->setCurrentIndex(count-2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget = ui->stackedWidget->widget(count-1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }

}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);
    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count-1);  //总是显示最新加入的页面
    ui->labelTitle->setText(widget->windowTitle());
}

void MasterView::on_btnBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count = ui->stackedWidget->count();
    if(count>1)
        ui->btnBack->setEnabled(true);
    else
        ui->btnBack->setEnabled(false);

    QString title = ui->stackedWidget->currentWidget()->windowTitle();
    if(title == "登录"){
        ui->btnBack->setEnabled(false);
        ui->btnLogout->setEnabled(false);
    }
    else if(title == "欢迎"){
        ui->btnBack->setEnabled(false);
        ui->btnLogout->setEnabled(true);

    }else{
        ui->btnLogout->setEnabled(false);
    }
}


void MasterView::on_btnLogout_clicked()
{
    goPreviousView();
}

