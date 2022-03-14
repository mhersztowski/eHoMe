#ifndef QEHLANGTEXTVIEW_H
#define QEHLANGTEXTVIEW_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextEdit>

class Highlighter;

class QEHLangTextView : public QTextEdit {
    Q_OBJECT

public:

    QEHLangTextView(QWidget *parent = nullptr) : QTextEdit(parent) {

        setupEditor();
    }

protected:
    Highlighter* highlighter;

    void setupEditor();
};


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
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
};



#endif // QEHLANGTEXTVIEW_H
