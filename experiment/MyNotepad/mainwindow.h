#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QTabWidget>
#include "codeeditor.h"
#include "highlighter.h"
#include <QSettings>
#include <QListWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void undoAvailable(bool b);

    void redoAvailable(bool b);

    void copyAvailable(bool b);

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_textEdit_textChanged();

    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionFontColor_triggered();

    void on_actionEditColor_triggered();

    void on_actionFontBackgroundColor_triggered();

    void on_actionLineWrap_triggered();

    void on_actionFont_triggered();

    void on_actionToolBar_triggered();

    void on_actionStatusBar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_textEdit_cursorPositionChanged();

    void on_actionLineNumber_triggered(bool checked);

    void on_actionAutoSave_triggered(bool checked);
    void switchTab(int index);
    void closeTab(int index);
    void clearHistory();
    void updateActions();  // 更新 QAction 的状态

    void addBookmark();        // 添加书签
    void showBookmarkDialog(); // 显示书签管理窗口

private:
    Ui::MainWindow *ui;
    QString currentFilePath;
    QTabWidget *tabWidget;
    QLabel statusCursorLabel;
    QLabel statusLabel;
    QTimer* autoSaveTimer;
    QString filePath;
    highlighter* highl; // 语法高亮对象
    bool textChanged;


    QList<QAction*> recentFileActionList;
    const int maxFileNr=10;
    QMenu* recentFilesMenu;
    QMenu* bookMarkMenu;
    QListWidget *bookmarkList;
    void adjustForCurrentFile(const QString& filePath);
    void updateRecentActionList();
    void loadFile(const QString& fileName);

    bool userEditConfirmed();
    void setupEditor();

    CodeEditor *currentCodeEdit();  // 返回当前选项卡中的 CodeEdit


};
#endif // MAINWINDOW_H
