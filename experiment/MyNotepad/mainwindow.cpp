#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化 TabWidget
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    tabWidget->setTabsClosable(true);
    // Tab 切换
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::switchTab);
    // 连接关闭请求信号
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    textChanged = false;
    on_actionNew_triggered();

    autoSaveTimer=new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::on_actionSave_triggered);
    statusLabel.setMidLineWidth(150);
    statusLabel.setText("length: " + QString::number(0) + "     lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMidLineWidth(150);
    statusCursorLabel.setText("Row: " + QString::number(0) + "     Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new  QLabel(ui->statusbar);
    author->setText(tr("xxx"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
    if(mode == QTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(false);
    } else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(true);
    }


    ui->actionStatusBar->setChecked(true);
    ui->actionToolBar->setChecked(true);
    // ui->actionLineNumber->setChecked(false);
    on_actionLineNumber_triggered(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg(this,ui->textEdit);
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this,ui->textEdit);
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())
        return;
    filePath = "";
    // ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));
    CodeEditor* textEdit = new CodeEditor(this);
    tabWidget->addTab(textEdit,"新建文本文件.txt");
    tabWidget->setCurrentWidget(textEdit);
    connect(textEdit, &CodeEditor::textChanged, this, &MainWindow::on_textEdit_textChanged);
    textChanged = false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files (*.txt) ;;All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        // QMessageBox::warning(this,"..","打开文件失败");
        return;
    }

    filePath = filename;
    QTextStream in(&file);
    QString text = in.readAll();

    // ui->textEdit->setPlainText(text);
    // ui->textEdit->insertPlainText(text);
    file.close();

    CodeEditor *textEdit = new CodeEditor(this);
    textEdit->setPlainText(text);
    tabWidget->addTab(textEdit,QFileInfo(filename).fileName());
    tabWidget->setCurrentWidget(textEdit);
    connect(textEdit, &CodeEditor::textChanged, this, &MainWindow::on_textEdit_textChanged);
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{

    CodeEditor * textEdit = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    if(filePath ==""){
        QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt) "));
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this,"..","打开保存文件失败");
            return;
        }
        file.close();
        filePath = filename;
    }


    QFile file(filePath);

    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"..","打开保存文件失败");
        return;
    }
    QTextStream out(&file);

    QString text = textEdit->toPlainText();
    out<<text;

    file.flush();
    file.close();


    // 更新标签页的标题为文件名
    int index = tabWidget->currentIndex();  // 获取当前标签页的索引
    if (index != -1) {
        tabWidget->setTabText(index, QFileInfo(filePath).fileName());  // 更新标签标题为文件名
    }
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());

    textChanged = false;
}


void MainWindow::on_actionSaveAs_triggered()
{
    CodeEditor * textEdit = qobject_cast<CodeEditor*>(tabWidget->currentWidget());
    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files (*.txt) "));
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        // QMessageBox::warning(this,"..","打开保存文件失败");
        return;
    }
    filePath = filename;

    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();
    // 更新标签页的标题为文件名
    int index = tabWidget->currentIndex();  // 获取当前标签页的索引
    if (index != -1) {
        tabWidget->setTabText(index, QFileInfo(filePath).fileName());  // 更新标签标题为文件名
    }

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_textEdit_textChanged()
{
    // if(!textChanged){
    //     this->setWindowTitle("*" + this->windowTitle());
    //     textChanged = true;
    // }
    // 获取当前激活的 tab 页索引
    int index = tabWidget->currentIndex();
    if (index == -1) return;  // 如果没有 tab 选中，则返回
    // qDebug()<<index;
    // 获取当前 tab 页对应的 CodeEdit 控件
    CodeEditor *codeEdit = qobject_cast<CodeEditor*>(tabWidget->widget(index));
    if (codeEdit) {
        // 获取当前 tab 页的标题
        QString currentTabTitle = tabWidget->tabText(index);
        // qDebug()<<currentTabTitle;
        // 如果标题不包含 "*"，则加上 "*"
        if (!currentTabTitle.startsWith("*")) {
            tabWidget->setTabText(index, "*" + currentTabTitle);
        }
        statusLabel.setText("length: " + QString::number(codeEdit->toPlainText().length())
                            + "     lines: " +
                            QString::number(codeEdit->document()->lineCount()));
    }




}

bool MainWindow::userEditConfirmed()
{
    if(textChanged){
        QString path = (filePath != "") ? filePath : "无标题.txt";
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt::Drawer);
        msg.setText(QString("是否将更改保存到\n") +"\"" + path + "\" ?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch(r){
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }

    return true;
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);

}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionEditColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit{background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionFontBackgroundColor_triggered()
{

}


void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
    if(mode == QTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(true);
    } else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok=false;
    QFont font = QFontDialog::getFont(&ok,this);

    if(ok){
        ui->textEdit->setFont(font);
    }
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible =ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setChecked(!visible);
}


void MainWindow::on_actionStatusBar_triggered()
{
    bool visible =ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusBar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed())
        exit(0);

}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col =0;
    int row=0;
    int flag=-1;
    int pos = ui->textEdit->textCursor().position();
    QString text = ui->textEdit->toPlainText();

    for(int i=0;i<pos;i++){
        if(text[i]=='\n'){
            row++;
            flag=i;
        }
    }
    flag++;
    col=pos-flag;
    statusCursorLabel.setText("Row: " + QString::number(row+1) + "     Col: " + QString::number(col+1));
}



void MainWindow::on_actionLineNumber_triggered(bool checked)
{
    ui->textEdit->hideLineNumberArea(checked);
}





void MainWindow::on_actionAutoSave_triggered(bool checked)
{
    if (checked) {
        autoSaveTimer->start(5000);  // 5000 毫秒 = 5秒
    } else {
        autoSaveTimer->stop();
    }
}

void MainWindow::switchTab(int index)
{
    if (index >= 0) {
        QWidget *widget = tabWidget->widget(index);
        QTextEdit *editor = qobject_cast<QTextEdit *>(widget);
        if (editor) {
            currentFilePath = editor->property("filePath").toString();
        }
    }
}

void MainWindow::closeTab(int index)
{
    // 关闭标签页
    tabWidget->removeTab(index);
}
