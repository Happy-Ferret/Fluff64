#include "highlighter.h"
#include "source/token.h"
#include "source/symboltable.h"
#include "source/syntax.h"

#include <QDebug>


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor(128,200,255));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    for (Token t: Syntax::s.reservedWords) {
        //qDebug() << QString::number(i) << TokenType::types[i].toLower();
        QString s = "\\b" + t.m_value.toLower() + "\\b";
        keywordPatterns<<s;
    }

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }


    builtinFunctionFormat.setForeground(QColor(255,200,50));
    builtinFunctionFormat.setFontWeight(QFont::Bold);
    keywordPatterns.clear();

    for (QString k: Syntax::s.builtInFunctions.keys()) {
        //qDebug() << QString::number(i) << TokenType::types[i].toLower();
        QString name = Syntax::s.builtInFunctions[k].m_name;
        QString s = "\\b" + name.toLower() + "\\b";
        keywordPatterns<<s;
    }

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = builtinFunctionFormat;
        highlightingRules.append(rule);
    }

    /* CONSTANTS */

    SymbolTable::Initialize();

    constantsFormat.setForeground(QColor(50,200,100));
    constantsFormat.setFontWeight(QFont::Normal);
    keywordPatterns.clear();

    for (QString k: SymbolTable::m_constants.keys()) {
        //qDebug() << QString::number(i) << TokenType::types[i].toLower();
        QString s = "\\b" + k.toLower() + "\\b";
        keywordPatterns<<s;
    }

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern,QRegularExpression::CaseInsensitiveOption);
        rule.format = constantsFormat;
        highlightingRules.append(rule);
    }





    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b",QRegularExpression::CaseInsensitiveOption);
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(QColor(255,128,40));
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

/*    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(200,255, 100));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
*/
/*    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("{[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);*/

    multiLineCommentFormat.setForeground(QColor(128,128,128));

    commentStartExpression = QRegularExpression("\\{");
    commentEndExpression = QRegularExpression("\\}");


}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
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

