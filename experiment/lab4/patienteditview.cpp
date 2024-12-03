#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h"
PatientEditView::PatientEditView(QWidget *parent,int row)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDatabase::getInstance().patientTabModel;
    dataMapper->setModel(IDatabase::getInstance().patientTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID,tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEDitName,tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard,tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbHigh,tabModel->fieldIndex("HEIGET"));
    dataMapper->addMapping(ui->dbWeight,tabModel->fieldIndex("WEIGET"));
    dataMapper->addMapping(ui->dbPhone,tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbCreateTime,tabModel->fieldIndex("CREATEDTIMESTAMP"));
    dataMapper->addMapping(ui->dbSex,tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbDate,tabModel->fieldIndex("DOB"));
    qDebug()<<row;
    dataMapper->setCurrentIndex(row);

}

PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{
    dataMapper->submit();
    IDatabase::getInstance().submitPatientEdit();
    emit goPreviousView();
}


void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().revertPatientEdit();
    emit goPreviousView();
}

