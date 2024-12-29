#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    HighlightRule rule;


    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        // C++ 关键字
        QStringLiteral("\\bchar\\b"),       QStringLiteral("\\bclass\\b"),      QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bdouble\\b"),     QStringLiteral("\\benum\\b"),       QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bfriend\\b"),     QStringLiteral("\\binline\\b"),     QStringLiteral("\\bint\\b"),
        QStringLiteral("\\blong\\b"),       QStringLiteral("\\bnamespace\\b"),  QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"),    QStringLiteral("\\bprotected\\b"),  QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bshort\\b"),      QStringLiteral("\\bsignals\\b"),    QStringLiteral("\\bsigned\\b"),
        QStringLiteral("\\bslots\\b"),      QStringLiteral("\\bstatic\\b"),     QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\btemplate\\b"),   QStringLiteral("\\btypedef\\b"),    QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\bunion\\b"),      QStringLiteral("\\bunsigned\\b"),   QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\bvoid\\b"),       QStringLiteral("\\bvolatile\\b"),   QStringLiteral("\\bbool\\b"),

        // C 语言关键字
        QStringLiteral("\\bauto\\b"),       QStringLiteral("\\bbreak\\b"),      QStringLiteral("\\bcase\\b"),
        QStringLiteral("\\bchar\\b"),       QStringLiteral("\\bconst\\b"),      QStringLiteral("\\bcontinue\\b"),
        QStringLiteral("\\bdefault\\b"),    QStringLiteral("\\bdo\\b"),         QStringLiteral("\\bdouble\\b"),
        QStringLiteral("\\belse\\b"),       QStringLiteral("\\benum\\b"),       QStringLiteral("\\bextern\\b"),
        QStringLiteral("\\bfloat\\b"),      QStringLiteral("\\bfor\\b"),        QStringLiteral("\\bgoto\\b"),
        QStringLiteral("\\bif\\b"),         QStringLiteral("\\blong\\b"),       QStringLiteral("\\bregister\\b"),
        QStringLiteral("\\bsigned\\b"),     QStringLiteral("\\bsizeof\\b"),     QStringLiteral("\\bstatic\\b"),
        QStringLiteral("\\bstruct\\b"),     QStringLiteral("\\bswitch\\b"),     QStringLiteral("\\btypedef\\b"),
        QStringLiteral("\\bunion\\b"),      QStringLiteral("\\bunsigned\\b"),   QStringLiteral("\\bvoid\\b"),
        QStringLiteral("\\bvolatile\\b"),   QStringLiteral("\\bwhile\\b"),

        // Java 关键字
        QStringLiteral("\\babstract\\b"),   QStringLiteral("\\bassert\\b"),     QStringLiteral("\\bbreak\\b"),
        QStringLiteral("\\bcase\\b"),       QStringLiteral("\\bcatch\\b"),      QStringLiteral("\\bclass\\b"),
        QStringLiteral("\\bconst\\b"),      QStringLiteral("\\bcontinue\\b"),  QStringLiteral("\\bdefault\\b"),
        QStringLiteral("\\bdo\\b"),         QStringLiteral("\\belse\\b"),       QStringLiteral("\\benum\\b"),
        QStringLiteral("\\bextends\\b"),    QStringLiteral("\\bfinal\\b"),      QStringLiteral("\\bfinally\\b"),
        QStringLiteral("\\bfloat\\b"),      QStringLiteral("\\bfor\\b"),        QStringLiteral("\\bgoto\\b"),
        QStringLiteral("\\bif\\b"),         QStringLiteral("\\bimplements\\b"), QStringLiteral("\\bin\\b"),
        QStringLiteral("\\binstanceof\\b"), QStringLiteral("\\bint\\b"),        QStringLiteral("\\blong\\b"),
        QStringLiteral("\\bnative\\b"),     QStringLiteral("\\bnew\\b"),        QStringLiteral("\\bnull\\b"),
        QStringLiteral("\\bpackage\\b"),    QStringLiteral("\\bprivate\\b"),    QStringLiteral("\\bprotected\\b"),
        QStringLiteral("\\bpublic\\b"),     QStringLiteral("\\breturn\\b"),     QStringLiteral("\\bshort\\b"),
        QStringLiteral("\\bstatic\\b"),     QStringLiteral("\\bstrictfp\\b"),   QStringLiteral("\\bsuper\\b"),
        QStringLiteral("\\bswitch\\b"),     QStringLiteral("\\bsynthetic\\b"), QStringLiteral("\\bthis\\b"),
        QStringLiteral("\\bthrow\\b"),      QStringLiteral("\\bthrows\\b"),     QStringLiteral("\\btransient\\b"),
        QStringLiteral("\\btry\\b"),        QStringLiteral("\\bvoid\\b"),       QStringLiteral("\\bvolatile\\b"),
        QStringLiteral("\\bwhile\\b"),

        // Python 关键字
        QStringLiteral("\\bFalse\\b"),      QStringLiteral("\\bclass\\b"),      QStringLiteral("\\bdef\\b"),
        QStringLiteral("\\bdel\\b"),        QStringLiteral("\\belif\\b"),       QStringLiteral("\\belse\\b"),
        QStringLiteral("\\bexcept\\b"),     QStringLiteral("\\bfinally\\b"),    QStringLiteral("\\bfor\\b"),
        QStringLiteral("\\bfrom\\b"),       QStringLiteral("\\bglobal\\b"),     QStringLiteral("\\bif\\b"),
        QStringLiteral("\\bin\\b"),         QStringLiteral("\\bis\\b"),         QStringLiteral("\\blambda\\b"),
        QStringLiteral("\\bNone\\b"),       QStringLiteral("\\bnonlocal\\b"),  QStringLiteral("\\bnot\\b"),
        QStringLiteral("\\bor\\b"),         QStringLiteral("\\bpass\\b"),       QStringLiteral("\\braise\\b"),
        QStringLiteral("\\breturn\\b"),     QStringLiteral("\\btry\\b"),        QStringLiteral("\\bwhile\\b"),
        QStringLiteral("\\bwith\\b"),       QStringLiteral("\\byield\\b"),

        // HTML 标签（仅列出部分常见标签）
        QStringLiteral("\\ba\\b"),           QStringLiteral("\\bdiv\\b"),       QStringLiteral("\\bspan\\b"),
        QStringLiteral("\\bimg\\b"),         QStringLiteral("\\bform\\b"),      QStringLiteral("\\binput\\b"),
        QStringLiteral("\\bbutton\\b"),      QStringLiteral("\\bhead\\b"),      QStringLiteral("\\bbody\\b"),
        QStringLiteral("\\bhtml\\b"),        QStringLiteral("\\bmeta\\b"),      QStringLiteral("\\blink\\b"),
        QStringLiteral("\\btitle\\b"),       QStringLiteral("\\bstyle\\b"),     QStringLiteral("\\blink\\b"),
        QStringLiteral("\\bscript\\b"),      QStringLiteral("\\btable\\b"),     QStringLiteral("\\btr\\b"),
        QStringLiteral("\\btd\\b"),          QStringLiteral("\\bth\\b"),        QStringLiteral("\\biframe\\b"),

        // JavaScript 关键字
        QStringLiteral("\\bawait\\b"),      QStringLiteral("\\bbreak\\b"),      QStringLiteral("\\bcase\\b"),
        QStringLiteral("\\bcatch\\b"),      QStringLiteral("\\bclass\\b"),      QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bcontinue\\b"),   QStringLiteral("\\bdebugger\\b"),  QStringLiteral("\\bdefault\\b"),
        QStringLiteral("\\bdelete\\b"),     QStringLiteral("\\bdo\\b"),         QStringLiteral("\\belse\\b"),
        QStringLiteral("\\bexport\\b"),     QStringLiteral("\\bextends\\b"),    QStringLiteral("\\bfinally\\b"),
        QStringLiteral("\\bfor\\b"),        QStringLiteral("\\bfunction\\b"),  QStringLiteral("\\bif\\b"),
        QStringLiteral("\\bimport\\b"),     QStringLiteral("\\bin\\b"),         QStringLiteral("\\binstanceof\\b"),
        QStringLiteral("\\blet\\b"),        QStringLiteral("\\bnew\\b"),        QStringLiteral("\\bnull\\b"),
        QStringLiteral("\\breturn\\b"),     QStringLiteral("\\bsuper\\b"),      QStringLiteral("\\bthis\\b"),
        QStringLiteral("\\bthrow\\b"),      QStringLiteral("\\btry\\b"),        QStringLiteral("\\btypeof\\b"),
        QStringLiteral("\\bvar\\b"),        QStringLiteral("\\bvoid\\b"),       QStringLiteral("\\bwhile\\b"),
        QStringLiteral("\\bwith\\b")
    };

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        rules.append(rule);
    }
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    rules.append(rule);


    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    rules.append(rule);


    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    rules.append(rule);

    // 单行注释的高亮（//）
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    rules.append(rule);

    // 多行注释的高亮（/*...*/）
    multiLineCommentFormat.setForeground(Qt::red);
    // 多行注释的开始：/*
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    // 多行注释的结束：/*
    commentEndExpression   = QRegularExpression(QStringLiteral("\\*/"));

}





// 高亮文本块
void Highlighter::highlightBlock(const QString &text)
{

    for (const HighlightRule &rule : qAsConst(rules)) {

        //使用QRegularExpression::globalMatch()函数在给定的文本块中搜索模式。
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

        while (matchIterator.hasNext()) {

            // 当发现模式出现时，获取该模式
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    //接着查找结束表达式的位置，因为跨行，所以需要循环查找。
    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        //如果在当前文本块中找不到结束表达式，则将当前块状态设置为1，并计算当前注释文本的长度
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        //如果在当前文本块中找到结束表达式，则计算注释文本的实际长度
        else {

            commentLength = endIndex - startIndex + match.capturedLength();
        }
        //应用多行注释格式。
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        //然后，我们搜索下一个出现的开始表达式并重复该过程。
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
