#include "highlighter.h"
#include <QColor>

Highlighter::Highlighter(QTextDocument *parent, QString fe)
    : QSyntaxHighlighter(parent)
{
    setFileExtension(fe);


    if (fileExtension == ".java" || fileExtension == ".c" || fileExtension == ".h"
            || fileExtension == ".js" ||
            fileExtension == ".s" || fileExtension == ".cpp" || fileExtension == ".scala") {
        QTextCharFormat tcf1;
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

        addHighlightingRule(115, 115, 185, "/\\*.*\\*/");
        addHighlightingRule(210, 105, 0, "\".*\"");
        addHighlightingRule(255, 128, 0, "\'.*\'");

    }
    if (fileExtension == ".java" || fileExtension == ".c" || fileExtension == ".cpp") {
        addHighlightingRule(220, 20, 60, "[\\?\\:]");
    }
    if (fileExtension == ".java" || fileExtension == ".scala") {
        QTextCharFormat tcf1;
        HighlightingRule rule;

        tcf1.setForeground(QColor(51, 85, 204));
        tcf1.setFontWeight(QFont::Bold);
        const QString Patterns1[] = {
            QStringLiteral("\\bsynchronized\\b"), QStringLiteral("\\binstanceof\\b"), QStringLiteral("\\bfinally\\b"),
            QStringLiteral("\\bsuper\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\bprivate\\b"),
            QStringLiteral("\\bprotected\\b"), QStringLiteral("\\babstract\\b"), QStringLiteral("\\bimplements\\b"),
            QStringLiteral("\\bextends\\b"), QStringLiteral("\\bpublic\\b"), QStringLiteral("\\bthis\\b"),
            QStringLiteral("\\bnew\\b"), QStringLiteral("\\bboolean\\b"), QStringLiteral("\\bvoid\\b"),
            QStringLiteral("\\btry\\b"), QStringLiteral("\\bcatch\\b"), QStringLiteral("\\btransient\\b"),
            QStringLiteral("\\bimport\\b")
        };
        for (const QString &pattern : Patterns1) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = tcf1;
            highlightingRules.append(rule);
        }

        addHighlightingBoldRule(108, 51, 101, "\\bpackage\\b");
        addHighlightingBoldRule(159, 77, 149, "\\bfalse\\b");
        addHighlightingBoldRule(159, 77, 149, "\\bnull\\b");
        addHighlightingBoldRule(159, 77, 149, "\\btrue\\b");
        addHighlightingBoldRule(198, 163, 0, "@\\w+");
        addHighlightingRule(1, 152, 88, "//.*");
        addHighlightingBoldRule(126, 61, 118, "\\bTODO\\b");

    }
    if (fileExtension == ".cpp" || fileExtension == ".c" || fileExtension == ".h") {
        QTextCharFormat tcf1;
        HighlightingRule rule;

        tcf1.setForeground(QColor(51, 85, 204));
        tcf1.setFontWeight(QFont::Bold);
        const QString Patterns1[] = {
            QStringLiteral("(\\bunsigned\\b)|(\\bsigned\\b)"), QStringLiteral("\\bconst\\b"), QStringLiteral("\\btypedef\\b"),
            QStringLiteral("(\\benum\\b)|(\\bunion\\b)"), QStringLiteral("\\bstruct\\b"), QStringLiteral("(\\blong\\b)|(\\bshort\\b)"),
            QStringLiteral("\\bregister\\b"), QStringLiteral("(\\bextern\\b)|(\\bauto\\b)"), QStringLiteral("\\bdefault\\b"),
            QStringLiteral("\\bgoto\\b")
        };
        for (const QString &pattern : Patterns1) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = tcf1;
            highlightingRules.append(rule);
        }
        addHighlightingRule(132, 43, 0, "\\b\\-\\>\\b");
        addHighlightingBoldRule(72, 72, 145, "#\\w+");
        addHighlightingRule(255, 0, 0, "0[xX][0-9a-fA-F]+");
        addHighlightingBoldRule(108, 51, 101, "\\bNULL\\b");
        addHighlightingBoldRule(162, 52, 0, "\\bsizeof\\b");
        addHighlightingRule(70, 117, 0, "#include");
    }
    if (fileExtension == ".cpp") {
        QTextCharFormat tcf1;
        HighlightingRule rule;

        tcf1.setForeground(QColor(51, 85, 204));
        tcf1.setFontWeight(QFont::Bold);
        const QString Patterns1[] = {
            QStringLiteral("\\busing\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\bbool\\b"),
            QStringLiteral("\\bcatch\\b"), QStringLiteral("\\bclass\\b"), QStringLiteral("\\benum\\b"),
            QStringLiteral("\\bexplicit\\b"), QStringLiteral("\\bexport\\b"), QStringLiteral("\\bfriend\\b"),
            QStringLiteral("\\bnew\\b"), QStringLiteral("\\boperator\\b"), QStringLiteral("\\bpublic\\b"),
            QStringLiteral("\\bprivate\\b"), QStringLiteral("\\btemplate\\b"), QStringLiteral("\\bthis\\b"),
            QStringLiteral("\\bthrow\\b"), QStringLiteral("\\bdelete\\b"), QStringLiteral("\\btemplate\\b")
        };
        for (const QString &pattern : Patterns1) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = tcf1;
            highlightingRules.append(rule);
        }
        addHighlightingBoldRule(159, 77, 149, "\\bfalse\\b");
        addHighlightingBoldRule(159, 77, 149, "\\bnullptr\\b");
        addHighlightingBoldRule(159, 77, 149, "\\btrue\\b");
        addHighlightingBoldRule(162, 52, 0, "\\bvector\b");
        addHighlightingBoldRule(162, 52, 0, "\\bstring\b");
        addHighlightingBoldRule(162, 52, 0, "\\bmap\b");
        addHighlightingBoldRule(162, 52, 0, "\\bset\b");
    }
    if (fileExtension == ".js" || fileExtension == ".jsp" ) {
        QTextCharFormat tcf1;
        HighlightingRule rule;

        tcf1.setForeground(QColor(51, 85, 204));
        tcf1.setFontWeight(QFont::Bold);
        const QString Patterns1[] = {
            QStringLiteral("\\bvar\\b"), QStringLiteral("\\bfunction\\b"), QStringLiteral("\\bin\\b"),
            QStringLiteral("\\bnew\\b")
        };
        for (const QString &pattern : Patterns1) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = tcf1;
            highlightingRules.append(rule);
        }
        addHighlightingBoldRule(108, 51, 101, "\\bdocument\\b");
        addHighlightingRule(159, 77, 149, "\\bthis\\b");
        addHighlightingRule(1, 152, 88, "//.*");
    }
    if (fileExtension == ".css") {
        QTextCharFormat tcf1;
        HighlightingRule rule;
        addHighlightingRule(0, 121, 121, "#[a-zA-z0-9-_]+");
        addHighlightingRule(78, 78, 78, "\\{|\\}");
        addHighlightingRule(255, 68, 0, "[a-zA-Z0-9-]+\\s*:");
        addHighlightingRule(177, 91, 255, "[a-zA-Z0-9#-,\\s\\(\\)\\.]+[;\\n]");
        addHighlightingRule(174, 0, 174, "\\.[a-zA-z0-9-_]+");
        addHighlightingRule(0, 0, 0, ":");
        addHighlightingRule(0, 0, 0, ";");
        addHighlightingRule(78, 78, 78, "\\.");

        tcf1.setFontItalic(true);
        tcf1.setForeground(QColor(157, 157, 157));
        rule.pattern = QRegularExpression(QStringLiteral("/\\*.*\\*/"));
        rule.format = tcf1;
        highlightingRules.append(rule);

        addHighlightingRule(152, 75, 75, "\".*\"");
        addHighlightingRule(152, 75, 75, "\'.*\'");
    }

    if (fileExtension == ".html" || fileExtension == ".htm" || fileExtension == ".xml"
            || fileExtension == ".jsp" || fileExtension == ".iml") {
        QTextCharFormat tcf1;
        HighlightingRule rule;
        addHighlightingRule(60, 60, 60, "\\.");
        addHighlightingRule(60, 60, 60, "(<script.*>)|(</script>)");
        addHighlightingRule(151, 124, 0, "(==|>=|<=|!=)");
        addHighlightingRule(60, 60, 60, "=");
        addHighlightingRule(123, 123, 123, "(</)|(/>)|(<\\?)|(\\?>)|[!<>]");
        addHighlightingRule(0, 85, 202, "<[a-zA-Z0-9/_\\?\\.-]+>");
        addHighlightingRule(151, 124, 0, "=\\s*\"(.|\\n)*?\"");
        addHighlightingRule(159, 53, 255, "&[a-z]+;");

        tcf1.setFontItalic(true);
        tcf1.setForeground(QColor(157, 157, 157));
        rule.pattern = QRegularExpression(QStringLiteral("<!--.*-->"));
        rule.format = tcf1;
        highlightingRules.append(rule);

        addHighlightingRule(79, 148, 205, "(<)|(>)");
    }
    if (fileExtension == ".jsp") {
        QTextCharFormat tcf1;
        HighlightingRule rule;
        addHighlightingRule(123, 123, 123, "(<%)|(%>)|[@]");
        addHighlightingRule(0, 128, 255, "\\bpage\\b");
        addHighlightingRule(210, 105, 0, "\"(.*?)\"");
        addHighlightingRule(187, 0, 187, "(<%@)|(%>)");
        addHighlightingRule(60, 60, 60, "(<%)|(%>)");


        tcf1.setFontItalic(true);
        tcf1.setForeground(QColor(157, 157, 157));
        rule.pattern = QRegularExpression(QStringLiteral("<%--.*--%>"));
        rule.format = tcf1;
        highlightingRules.append(rule);
    }
    /*else {
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

    }*/
    multiLineCommentFormat.setForeground(QColor(115, 115, 185));

    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
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

void Highlighter::addHighlightingBoldRule(int a, int b, int c, QString regularExpression)
{
    HighlightingRule rule;
    QTextCharFormat tcf;
    tcf.setFontWeight(QFont::Bold);
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
