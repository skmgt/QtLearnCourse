#include "idatabase.h"

void IDatabase::initDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE"); //数据库驱动
    QString aFile ="E:/Program/Qt/codeRepository/QtLearnCourse/experiment/lab4/build/lab4.db" ;
    qDebug()<<aFile;
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug()<<"failed to open database";
    }else
        qDebug()<<"open database is ok";
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
