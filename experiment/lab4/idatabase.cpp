#include "idatabase.h"
#include <QUuid>
void IDatabase::initDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE"); //数据库驱动
    QString aFile ="../../lab4.db" ;
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug()<<"failed to open database";
    }else
        qDebug()<<"open database is ok"<<database.connectionName();;
}

bool IDatabase::initPatientModel()
{
    patientTabModel = new QSqlTableModel(this,database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("name"),Qt::AscendingOrder);

    if(!(patientTabModel->select()))
        return false;
    thePatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());  //在末尾添加一个记录
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount() - 1, 1); //创建最后一行的ModeIndex

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);  //获取当前记录

    curRec.setValue("CREATEDTIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID",QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo,curRec);

    return curIndex.row();
}



bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
}

bool IDatabase::submitPatientEdit()
{
    bool flag = patientTabModel->submitAll();
    for (int row = 0; row < patientTabModel->rowCount(); ++row) {
        // 获取某一行的记录
        QSqlRecord record = patientTabModel->record(row);

        // 遍历记录中的所有列
        for (int i = 0; i < record.count(); ++i) {
            QVariant value = record.value(i);
            QString fieldName = patientTabModel->headerData(i, Qt::Horizontal).toString();
            qDebug() << fieldName << ": " << value.toString();
        }
        //QVariant id = patientTabModel->data(patientTabModel->index(row, patientTabModel->fieldIndex("ID")));
        // qDebug() << "Row" << row << "ID:" << id.toString();
    }
    if(flag)qDebug()<<"成功提交";
    else {
        qDebug() << "Failed to submit data:" << patientTabModel->lastError().text();

    }
    return flag;
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

QString IDatabase::userLogin(QString username, QString password)
{
    QSqlQuery query;
    query.prepare("select username , password from user where username = :USER");
    query.bindValue(":USER",username);
    query.exec();
    if(query.first() && query.value("username").isValid()){
        QString passwd = query.value("password").toString();
        if(passwd==password){
            return "ok";
        }else{
            return "password fail";
        }
    }else{
        qDebug()<<"no such user";
        return "fail";
    }
}

IDatabase::IDatabase(QObject *parent) : QObject{parent}
{
    initDatabase();
}
