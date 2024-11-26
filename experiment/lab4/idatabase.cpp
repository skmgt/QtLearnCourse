#include "idatabase.h"

void IDatabase::initDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE"); //数据库驱动
    QString aFile ="../../lab4.db" ;
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug()<<"failed to open database";
    }else
        qDebug()<<"open database is ok";
}

IDatabase::IDatabase(QObject *parent) : QObject{parent}
{
    initDatabase();
}
