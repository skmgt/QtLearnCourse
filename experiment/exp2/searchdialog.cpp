#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QDebug>
#include <QMessageBox>

SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit * textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    pText = textEdit;
    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btnFindNext_clicked()
{
    QString target = ui->searchEdit->text();
    if(target ==""||pText == nullptr){
        return;
    }
    QString text = pText->toPlainText();
    QTextCursor c= pText->textCursor(); //光标位置
    int index =-1;
    // qDebug()<<target<<'\n';
    if(ui->rbDown->isChecked()){
        index = text.indexOf(target,c.position(),ui->cbDimension->isChecked()? Qt::CaseSensitive:Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index);
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);

            pText->setTextCursor(c);
        }
    }else if(ui->rbUp->isChecked()){
        index = text.lastIndexOf(target,c.position()-text.length()-1,ui->cbDimension->isChecked()? Qt::CaseSensitive:Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);

            pText->setTextCursor(c);
        }
    }

    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("结果");
        msg.setText(QString("找不到 ")+target);
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

}


void SearchDialog::on_btnCancel_clicked()
{
    accept();
}

