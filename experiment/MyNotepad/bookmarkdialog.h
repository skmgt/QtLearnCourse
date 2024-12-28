#ifndef BOOKMARKDIALOG_H
#define BOOKMARKDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include "codeeditor.h"
class BookmarkDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BookmarkDialog(QWidget *parent = nullptr);
    void setEditor(CodeEditor *editor); // 设置当前文件的编辑器

private slots:
    void handleJump();   // 跳转到选中的书签
    void handleDelete(); // 删除选中的书签

private:
    CodeEditor *editor;           // 当前文件的 CodeEdit
    QTableWidget *tableWidget;  // 用于显示书签的表格
    QPushButton *jumpButton;    // 跳转按钮
    QPushButton *deleteButton;  // 删除按钮

    void loadBookmarks();       // 加载书签到表格中
};

#endif // BOOKMARKDIALOG_H
