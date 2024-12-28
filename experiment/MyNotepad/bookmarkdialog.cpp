#include "bookmarkdialog.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTextBlock>
BookmarkDialog::BookmarkDialog(QWidget *parent)
    : QDialog(parent), editor(nullptr)
{
    setWindowTitle("书签管理");
    resize(400, 300);

    // 表格初始化
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);  // 两列：行号和描述
    tableWidget->setHorizontalHeaderLabels(QStringList() << "行号" << "描述");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 按钮
    jumpButton = new QPushButton("跳转", this);
    deleteButton = new QPushButton("删除", this);

    connect(jumpButton, &QPushButton::clicked, this, &BookmarkDialog::handleJump);
    connect(deleteButton, &QPushButton::clicked, this, &BookmarkDialog::handleDelete);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(jumpButton);
    buttonLayout->addWidget(deleteButton);

    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(buttonLayout);

}

void BookmarkDialog::setEditor(CodeEditor *editor)
{
    this->editor = editor;

    if (editor) {
        loadBookmarks(); // 加载当前文件的书签
    }
}

void BookmarkDialog::handleJump()
{
    if (!editor) return;

    // 获取选中的行
    int currentRow = tableWidget->currentRow();
    if (currentRow < 0) return;

    int lineNumber = tableWidget->item(currentRow, 0)->text().toInt() - 1;

    QTextBlock block = editor->document()->findBlockByLineNumber(lineNumber);
    if (block.isValid()) {
        QTextCursor cursor(block);
        editor->setTextCursor(cursor);
        editor->centerCursor(); // 将光标居中
        this->accept();         // 关闭窗口
    }
}

void BookmarkDialog::handleDelete()
{
    if (!editor) return;

    // 获取选中的行
    int currentRow = tableWidget->currentRow();
    if (currentRow < 0) return;

    int lineNumber = tableWidget->item(currentRow, 0)->text().toInt() - 1;

    editor->removeBookmark(lineNumber); // 删除书签
    loadBookmarks();                    // 刷新表格
}

void BookmarkDialog::loadBookmarks()
{
    tableWidget->setRowCount(0); // 清空表格

    if (!editor) return;

    QMap<int, QString> bookmarks = editor->getBookmarks();
    int row = 0;

    for (auto it = bookmarks.begin(); it != bookmarks.end(); ++it) {
        tableWidget->insertRow(row);

        // 行号
        QTableWidgetItem *lineItem = new QTableWidgetItem(QString::number(it.key() + 1));
        lineItem->setFlags(lineItem->flags() & ~Qt::ItemIsEditable); // 禁止编辑
        tableWidget->setItem(row, 0, lineItem);

        // 描述
        QTableWidgetItem *descItem = new QTableWidgetItem(it.value());
        descItem->setFlags(descItem->flags() & ~Qt::ItemIsEditable); // 禁止编辑
        tableWidget->setItem(row, 1, descItem);

        ++row;
    }
}
