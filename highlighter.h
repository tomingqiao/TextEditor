#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include <QRegularExpression>


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0, QString fe = nullptr);
    void setFileExtension(QString fe);//得到文件后缀名
    void addHighlightingRule(int a, int b, int c, QString regularExpression);//添加高亮规则
    void addHighlightingBoldRule(int a, int b, int c,
                                 QString regularExpression);//添加bold高亮规则

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QString fileExtension;
};

#endif // HIGHLIGHTER_H
