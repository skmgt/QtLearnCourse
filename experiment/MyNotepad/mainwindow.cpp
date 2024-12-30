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
#include <QClipboard>
#include <QGuiApplication>
#include "BookmarkDialog.h"
#include <QMimeData>
#include "favoritesdialog.h"
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
    on_actionNew_triggered();
    textChanged = false;
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


    // 捕获当前选项卡切换的信号
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateActions);
    // 初始化 QAction 状态
    updateActions();

    CodeEditor * textEdit = currentCodeEdit();
    QPlainTextEdit::LineWrapMode mode = textEdit->lineWrapMode();
    if(mode == QTextEdit::NoWrap){
        textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(false);
    } else{
        textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(true);
    }


    ui->actionStatusBar->setChecked(true);
    ui->actionToolBar->setChecked(true);
    // ui->actionLineNumber->setChecked(false);
    on_actionLineNumber_triggered(false);


    QAction* recentFileAction = nullptr;
    for(auto i = 0; i <= maxFileNr; ++i){
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        if(i==maxFileNr){
            connect(recentFileAction,&QAction::triggered,this,&MainWindow::clearHistory);
        }else{
            connect(recentFileAction, &QAction::triggered, this, [=]()
                    {
                        loadFile(recentFileAction->data().toString());
                    });
        }

        recentFileActionList.append(recentFileAction);
    }



    // 获取 fileMenu 中的所有动作
    recentFilesMenu = new QMenu(tr("历史记录"), ui->fileMenu);
    QList<QAction *> actions = ui->fileMenu->actions();
    if (actions.size() > 1) {
        // 获取倒数第二个动作
        QAction *secondLastAction = actions[actions.size() - 2];

        // 插入 recentFilesMenu 在 secondLastAction 之前
        ui->fileMenu->insertMenu(secondLastAction, recentFilesMenu);
    } else {
        ui->fileMenu->addMenu(recentFilesMenu);
    }

    for(auto i = 0; i <= maxFileNr; ++i)
        recentFilesMenu->addAction(recentFileActionList.at(i));

    updateRecentActionList();



    //书签功能初始化
    bookMarkMenu = new QMenu(tr("书签"),ui->showMenu);
    ui->showMenu->addMenu(bookMarkMenu);
    QAction* bookMarkAction = new QAction(this);
    bookMarkAction->setText("添加书签");
    bookMarkAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F2));
    bookMarkMenu->addAction(bookMarkAction);
    connect(bookMarkAction,&QAction::triggered, this,&MainWindow::addBookmark);

    bookMarkAction = new QAction(this);
    bookMarkAction->setText("查看所有书签");
    bookMarkMenu->addAction(bookMarkAction);
    connect(bookMarkAction, &QAction::triggered, this, &MainWindow::showBookmarkDialog);


    // 收藏夹


    favoritesManager = new FavoritesManager(this);
    QAction *favoritesAction = new QAction("收藏夹管理", this);
    connect(favoritesAction, &QAction::triggered, this, &MainWindow::showFavoritesDialog);
    ui->showMenu->addAction(favoritesAction);

    //高亮与主题
    setupEditor();
    connect(ui->actionTheme,&QAction::triggered,this,&MainWindow::setTheme);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);

}

void MainWindow::redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

void MainWindow::copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}





void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionFind_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    SearchDialog dlg(this,textEdit);
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    ReplaceDialog dlg(this,textEdit);
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())
        return;
    filePath = "";

    this->setWindowTitle(tr("新建文本文件 - 编辑器"));
    CodeEditor* textEdit = new CodeEditor(this);
    tabWidget->addTab(textEdit,"新建文本文件.txt");
    tabWidget->setCurrentWidget(textEdit);
    connect(textEdit, &CodeEditor::textChanged, this, &MainWindow::on_textEdit_textChanged);
    textChanged = false;
    setupEditor();
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        // QMessageBox::warning(this,"..","打开文件失败");
        return;
    }

    filePath = filename;
    QTextStream in(&file);
    QString text = in.readAll();


    file.close();

    CodeEditor *textEdit = new CodeEditor(this);
    textEdit->setPlainText(text);
    tabWidget->addTab(textEdit,QFileInfo(filename).fileName());
    tabWidget->setCurrentWidget(textEdit);
    connect(textEdit, &CodeEditor::textChanged, this, &MainWindow::on_textEdit_textChanged);
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    adjustForCurrentFile(filePath);

    setupEditor();
    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{

    CodeEditor * textEdit = currentCodeEdit();
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
    CodeEditor * textEdit = currentCodeEdit();
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
    //获取当前 tab 页对应的 CodeEdit 控件
    CodeEditor *codeEdit = currentCodeEdit();
    if (codeEdit) {
        // 获取当前 tab 页的标题
        QString currentTabTitle = tabWidget->tabText(index);
        // // qDebug()<<currentTabTitle;
        // // 如果标题不包含 "*"，则加上 "*"
        // if (!currentTabTitle.startsWith("*")) {
        //     tabWidget->setTabText(index, "*" + currentTabTitle);
        // }
        statusLabel.setText("length: " + QString::number(codeEdit->toPlainText().length())
                            + "     lines: " +
                            QString::number(codeEdit->document()->lineCount()));
    }
}

bool MainWindow::userEditConfirmed()
{
    // if(textChanged){
    //     QString path = (filePath != "") ? filePath : "无标题.txt";
    //     QMessageBox msg(this);
    //     msg.setIcon(QMessageBox::Question);
    //     msg.setWindowTitle("...");
    //     msg.setWindowFlag(Qt::Drawer);
    //     msg.setText(QString("是否将更改保存到\n") +"\"" + path + "\" ?");
    //     msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    //     int r = msg.exec();
    //     switch(r){
    //     case QMessageBox::Yes:
    //         on_actionSave_triggered();
    //         break;
    //     case QMessageBox::No:
    //         textChanged = false;
    //         break;
    //     case QMessageBox::Cancel:
    //         return false;
    //     }
    // }

    return true;
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(12);

    CodeEditor* editor = currentCodeEdit();
    editor->setFont(font);

    highl = new Highlighter(editor->document());

    QFile file("mainwindow.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}

void MainWindow::setTheme()
{
    QFile file;
    if(!theme){
        file.setFileName(":/light.qss");
        theme=true;
    }else{
        file.setFileName(":/dark.qss");
        theme=false;
    }


    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        QString style = stream.readAll();
        qDebug()<<style;
        qApp->setStyleSheet(style);
    }
}

CodeEditor *MainWindow::currentCodeEdit()
{
    // 获取当前选项卡中的 CodeEdit
    QWidget *currentWidget = tabWidget->currentWidget();
    if (currentWidget) {
        return qobject_cast<CodeEditor *>(currentWidget);
    }
    return nullptr;
}


void MainWindow::on_actionUndo_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->redo();
}

void MainWindow::on_actionFontColor_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        textEdit->setStyleSheet(QString("QPlainTextEdit{color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionEditColor_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        textEdit->setStyleSheet(QString("QPlainTextEdit{background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionFontBackgroundColor_triggered()
{

}


void MainWindow::on_actionLineWrap_triggered()
{
    CodeEditor * textEdit = currentCodeEdit();
    QPlainTextEdit::LineWrapMode mode = textEdit->lineWrapMode();
    if(mode == QTextEdit::NoWrap){
        textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(true);
    } else{
        textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok=false;
    QFont font = QFontDialog::getFont(&ok,this);
    CodeEditor * textEdit = currentCodeEdit();
    if(ok){
        textEdit->setFont(font);
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
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed())
        exit(0);

}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    CodeEditor * textEdit = currentCodeEdit();
    int col =0;
    int row=0;
    int flag=-1;
    int pos = textEdit->textCursor().position();
    QString text = textEdit->toPlainText();

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
    CodeEditor * textEdit = currentCodeEdit();
    textEdit->hideLineNumberArea(checked);
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
        CodeEditor *editor = qobject_cast<CodeEditor *>(widget);
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

void MainWindow::clearHistory()
{
    for (auto action : recentFileActionList) {
        // 清空文本
        action->setText(QString());

        // 清空数据
        action->setData(QVariant());

        // 设置为不可见
        action->setVisible(false);
    }

}

void MainWindow::updateActions()
{
    // 获取当前的 CodeEdit
    CodeEditor *editor = currentCodeEdit();
    if (!editor) {
        // 如果没有有效的 CodeEdit，则禁用所有动作
        ui->actionCopy->setEnabled(false);
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionPaste->setEnabled(false);
        return;
    }

    // 断开之前的连接
    disconnect(editor, &CodeEditor::copyAvailable, ui->actionCopy, &QAction::setEnabled);
    disconnect(editor, &CodeEditor::copyAvailable, ui->actionCut, &QAction::setEnabled);
    disconnect(editor, &CodeEditor::undoAvailable, ui->actionUndo, &QAction::setEnabled);
    disconnect(editor, &CodeEditor::redoAvailable, ui->actionRedo, &QAction::setEnabled);


    // 动态连接当前 CodeEdit 的信号到 QAction
    connect(editor, &CodeEditor::copyAvailable, ui->actionCopy, &QAction::setEnabled);
    connect(editor, &CodeEditor::copyAvailable, ui->actionCut, &QAction::setEnabled);
    connect(editor, &CodeEditor::undoAvailable, ui->actionUndo, &QAction::setEnabled);
    connect(editor, &CodeEditor::redoAvailable, ui->actionRedo, &QAction::setEnabled);

    // 设置 QAction 的初始状态
    ui->actionCopy->setEnabled(editor->textCursor().hasSelection());
    ui->actionCut->setEnabled(editor->textCursor().hasSelection());
    ui->actionUndo->setEnabled(editor->document()->isUndoAvailable());
    ui->actionRedo->setEnabled(editor->document()->isRedoAvailable());
    // 使用 QClipboard 来检查是否有可粘贴的内容
    QClipboard *clipboard = QGuiApplication::clipboard();
    bool canPaste = clipboard->mimeData()->hasText();  // 检查剪贴板是否有文本
    ui->actionPaste->setEnabled(canPaste);  // 启用或禁用粘贴操作

}

void MainWindow::addBookmark()
{
    CodeEditor *editor = currentCodeEdit();
    if (editor) {
        int lineNumber = editor->textCursor().blockNumber();
        editor->toggleBookmark(lineNumber);
    }
}

void MainWindow::showBookmarkDialog()
{
    CodeEditor *editor = currentCodeEdit(); // 获取当前打开的 CodeEdit 对象
    if (!editor) return;
    BookmarkDialog dialog(this);
    dialog.setEditor(editor); // 设置当前文件的编辑器
    dialog.exec();            // 模态弹出窗口
}



void MainWindow::showFavoritesDialog()
{
    FavoritesDialog dialog(favoritesManager, this);
    // 连接收藏夹弹窗的打开文件信号到主窗口的文件打开槽
    connect(&dialog, &FavoritesDialog::openFileRequested, this, &MainWindow::openFile);

    dialog.exec();
}

void MainWindow::openFile(const QString &filePath)
{
    loadFile(filePath);
}



void MainWindow::adjustForCurrentFile(const QString &filePath)
{
    currentFilePath = filePath;
    setWindowFilePath(currentFilePath);

    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > maxFileNr)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);

    // see note
    updateRecentActionList();

}

void MainWindow::updateRecentActionList()
{
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();

    auto itEnd = 0;
    if(recentFilePaths.size() <= maxFileNr)
        itEnd = recentFilePaths.size();
    else
        itEnd = maxFileNr;

    for (auto i = 0; i < itEnd; ++i) {
        QString strippedName = QString::number(i + 1) + ". " + QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }
    for (auto i = itEnd; i < maxFileNr; ++i)
        recentFileActionList.at(i)->setVisible(false);

    if(itEnd>0){
        recentFileActionList.at(maxFileNr)->setText("清除历史记录");
        recentFileActionList.at(maxFileNr)->setVisible(true);
        // qDebug()<<recentFileActionList.at(maxFileNr);
    }

}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Recent Files"),
                             tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    CodeEditor *textEdit = new CodeEditor(this);
    textEdit->setPlainText(in.readAll());
    tabWidget->addTab(textEdit,QFileInfo(fileName).fileName());
    tabWidget->setCurrentWidget(textEdit);
    connect(textEdit, &CodeEditor::textChanged, this, &MainWindow::on_textEdit_textChanged);


    QApplication::restoreOverrideCursor();

}




