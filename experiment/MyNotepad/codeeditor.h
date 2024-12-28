#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QPlainTextEdit>
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();


    void addBookmark(int lineNumber, const QString &description = QString()); // 添加书签
    void removeBookmark(int lineNumber);                                      // 删除书签
    void toggleBookmark(int lineNumber);                                      // 切换书签状态
    QMap<int, QString> getBookmarks() const;                                  // 获取所有书签
    void clearBookmarks();                                                    // 清空所有书签
signals:
    void bookmarksUpdated(); // 通知书签列表发生变化
protected:
    void resizeEvent(QResizeEvent *event) override;
    // 捕捉鼠标点击事件
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // 绘制书签符号到行号区域

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
public slots:
    void hideLineNumberArea(bool flag);


private:
    QWidget *lineNumberArea;
    QMap<int, QString> bookmarks; // 存储行号和书签描述
    bool isValidLink(const QString &text);
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};



#endif // CODEEDITOR_H
