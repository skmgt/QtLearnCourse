#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit * textedit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    pText=textedit;

    ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_btnFindNext_clicked()
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


void ReplaceDialog::on_btnReplace_clicked()
{
    QString target = ui->searchEdit->text();
    QString to = ui->targetText->text();

    if(pText !=nullptr&&target!=""&&to!=""){
        QString selText = pText->textCursor().selectedText();
        if(selText == target){
            pText->insertPlainText(to);
        }
        on_btnFindNext_clicked();
    }
}


void ReplaceDialog::on_btnReplaceAll_clicked()
{
    QString target = ui->searchEdit->text();
    QString to = ui->targetText->text();
    if(pText !=nullptr&&target!=""&&to!=""){
        QString text = pText->toPlainText();
        text.replace(target,to,ui->cbDimension->isChecked()?Qt::CaseSensitive:
                                     Qt::CaseInsensitive);
        pText->clear();
        pText->insertPlainText(text);
    }
}


void ReplaceDialog::on_btnCancel_clicked()
{
    accept();
}

