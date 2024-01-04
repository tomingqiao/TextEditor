#include "highlighter.h"
#include <QColor>

Highlighter::Highlighter(QTextDocument *parent, QString fe)
    : QSyntaxHighlighter(parent)
{
    setFileExtension(fe);
    if (fileExtension == ".java") {
        QTextCharFormat tcf1;
        QTextCharFormat tcf2;
        QTextCharFormat tcf3;
        QTextCharFormat tcf4;
        QTextCharFormat tcf5;
        QTextCharFormat tcf6;
        QTextCharFormat tcf7;
        QTextCharFormat tcf8;
        QTextCharFormat tcf9;
        QTextCharFormat tcf10;
        QTextCharFormat tcf11;
        QTextCharFormat tcf12;
        HighlightingRule rule;

        tcf1.setForeground(QColor(51, 85, 204));
        tcf1.setFontWeight(QFont::Bold);
        const QString Patterns1[] = {
            QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bbreak\\b"), QStringLiteral("\\belse\\b"),
            QStringLiteral("\\breturn\\b"), QStringLiteral("\\bint\\b"), QStringLiteral("\\bfor\\b"),
            QStringLiteral("\\bif\\b"), QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bdo\\b"),
            QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
            QStringLiteral("\\bchar\\b"), QStringLiteral("\\bfinal\\b"), QStringLiteral("\\bstatic\\b"),
            QStringLiteral("\\bswitch\\b"), QStringLiteral("\\bcase\\b")
        };
        for (const QString &pattern : Patterns1) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = tcf1;
            highlightingRules.append(rule);
        }
        addHighlightingRule(220, 20, 60, "[0-9]+\\.?[0-9]*f*");
        addHighlightingRule(108, 51, 101,
                            "[\\!\\#\\$\\%\\^&\\*\\(\\)\\+\\-\\=\\{\\}\\[\\]\\:\\;\\,\\.\\<\\>\\?\\~\\|]");
        addHighlightingRule(1, 152, 88, "//.*");


        tcf3.setFontWeight(QFont::Bold);
        tcf3.setForeground(QColor(126, 61, 118));
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Z]+[A-Z0-9_]*\\b"));
        rule.format = tcf3;
        highlightingRules.append(rule);

        addHighlightingRule(115, 115, 185, "/\\*.*\\*/");
        addHighlightingRule(210, 105, 0, "\".*\"");
        addHighlightingRule(255, 128, 0, "\'.*\'");
        addHighlightingRule(220, 20, 60, "[\\?\\:]");


        multiLineCommentFormat.setForeground(QColor(115, 115, 185));

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

    } else {
        HighlightingRule rule;

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);
        const QString keywordPatterns[] = {
            QStringLiteral("\\bchar\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bconst\\b"),
            QStringLiteral("\\bdouble\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\bfriend\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bint\\b"),
            QStringLiteral("\\blong\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
            QStringLiteral("\\bprivate\\b"), QStringLiteral("\\bprotected\\b"), QStringLiteral("\\bpublic\\b"),
            QStringLiteral("\\bshort\\b"), QStringLiteral("\\bsignals\\b"), QStringLiteral("\\bsigned\\b"),
            QStringLiteral("\\bslots\\b"), QStringLiteral("\\bstatic\\b"), QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\btemplate\\b"), QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"),
            QStringLiteral("\\bunion\\b"), QStringLiteral("\\bunsigned\\b"), QStringLiteral("\\bvirtual\\b"),
            QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\bbool\\b")
        };
        for (const QString &pattern : keywordPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
        classFormat.setFontWeight(QFont::Bold);
        classFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
        rule.format = classFormat;
        highlightingRules.append(rule);

        quotationFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        rule.format = functionFormat;
        highlightingRules.append(rule);
        singleLineCommentFormat.setForeground(Qt::red);
        rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::red);

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
    }
}

void Highlighter::setFileExtension(QString fe)
{
    fileExtension = fe;
}

void Highlighter::addHighlightingRule(int a, int b, int c, QString regularExpression)
{
    HighlightingRule rule;
    QTextCharFormat tcf;
    tcf.setForeground(QColor(a, b, c));
    rule.pattern = QRegularExpression(regularExpression);
    rule.format = tcf;
    highlightingRules.append(rule);
}
void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);
    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
