

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QTextEdit>
#include <QObject>
#include <QMap>
#include <QCompleter>
#include <QStringListModel>
#include "source/symboltable.h"
#include "source/syntax.h"
#include "source/parser.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;



class LineNumberArea;



class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    QAbstractItemModel *modelFromFile(const QString& fileName);
    QAbstractItemModel *modelFromTRSE(SymbolTable* symtab, Parser* parser);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void cycleNumberAreaPaintEvent(QPaintEvent *event);
    int cycleNumberAreaWidth();

    void FixBackTab(QKeyEvent* e);

    void InitCompleter(SymbolTable* m_symTab, Parser* parser);


    QMap<int,int> m_cycles;

    void RepaintCycles() {
        cycleNumberArea->repaint();
    }

protected:
    void resizeEvent(QResizeEvent *event) override;
//    void cycleresizeEvent(QResizeEvent *event) override;

private slots:
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void updateLineNumberAreaWidth(int newBlockCount);

    void updateCycleNumberArea(const QRect &, int);
    void updateCycleNumberAreaWidth(int newBlockCount);

private:
    QWidget *lineNumberArea;
    QWidget *cycleNumberArea;


 // Completer stuff

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

private:
    QCompleter *c = nullptr;




};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};


class CycleNumberArea : public QWidget
{
public:
    CycleNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->cycleNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->cycleNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};


#endif
