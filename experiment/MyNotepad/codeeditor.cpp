#include "codeeditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QRegularExpression>
#include <QDesktopServices>
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{

    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);


    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::addBookmark(int lineNumber, const QString &description)
{
    bookmarks[lineNumber] = description.isEmpty() ? QString("Line %1").arg(lineNumber) : description;
    emit bookmarksUpdated(); // 通知书签更新
    viewport()->update();    // 刷新视图
}

void CodeEditor::removeBookmark(int lineNumber)
{
    if (bookmarks.contains(lineNumber)) {
        bookmarks.remove(lineNumber);
        emit bookmarksUpdated(); // 通知书签更新
        viewport()->update();    // 刷新视图
    }
}

void CodeEditor::toggleBookmark(int lineNumber)
{
    if (bookmarks.contains(lineNumber)) {
        removeBookmark(lineNumber);
    } else {
        addBookmark(lineNumber);
    }
}

QMap<int, QString> CodeEditor::getBookmarks() const
{
    return bookmarks;
}

void CodeEditor::clearBookmarks()
{
    bookmarks.clear();
    emit bookmarksUpdated();
    viewport()->update();
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::hideLineNumberArea(bool flag)
{
    if(!flag){
        setViewportMargins(0, 0, 0, 0);
        lineNumberArea->hide();
    }
    else{
        lineNumberArea->show();
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
}

bool CodeEditor::isValidLink(const QString &text)
{
    QRegularExpression urlRegex(R"((http|https|ftp|mailto)://[^\s]+)");
    return urlRegex.match(text).hasMatch();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos());
        QString clickedText = cursor.selectedText();
        // qDebug()<<clickedText;
        // 如果没有选中文本，就获取光标下的文本
        if (clickedText.isEmpty()) {
            clickedText = cursor.block().text();
        }
        // qDebug()<<clickedText;
        // 如果文本包含链接，就打开它
        if (isValidLink(clickedText)) {
            QUrl url(clickedText);
            if (url.isValid()) {
                // 打开链接
                QDesktopServices::openUrl(url);
            }
        }
    }

    // 继续处理其他鼠标事件
    QPlainTextEdit::mouseReleaseEvent(event);
}

void CodeEditor::paintEvent(QPaintEvent *event)
{
    QPlainTextEdit::paintEvent(event);

    // 绘制书签符号
    QPainter painter(viewport());
    QFontMetrics metrics(font());
    int lineHeight = metrics.height();

    for (auto it = bookmarks.begin(); it != bookmarks.end(); ++it) {
        int lineNumber = it.key();
        QTextBlock block = document()->findBlockByLineNumber(lineNumber);

        if (!block.isValid() || !block.isVisible())
            continue;

        // 计算书签图标的绘制位置
        QRect rect = blockBoundingGeometry(block).translated(contentOffset()).toRect();
        int y = rect.top();
        int x = -10; // 绘制在行号区域

        // 绘制圆形书签图标
        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(x + 10, y + lineHeight / 2), 5, 5);
    }
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}


