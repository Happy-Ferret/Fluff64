

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QTextEdit>
#include <QObject>
#include <QMap>

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




    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void cycleNumberAreaPaintEvent(QPaintEvent *event);
    int cycleNumberAreaWidth();


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
