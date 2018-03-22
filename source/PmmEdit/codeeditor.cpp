
#include <QtWidgets>

#include "codeeditor.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);
    cycleNumberArea = new CycleNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateCycleNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateCycleNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
    updateCycleNumberAreaWidth(0);
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

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

int CodeEditor::cycleNumberAreaWidth()
{

    return 80;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateCycleNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(cycleNumberAreaWidth(), 0, 0, 0);
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


void CodeEditor::updateCycleNumberArea(const QRect &rect, int dy)
{

    if (dy)
        cycleNumberArea->scroll(0, dy);
    else
//        cycleNumberArea->update(rect.x()-cycleNumberArea->width()-1, rect.y(), cycleNumberArea->width(), rect.height());
      cycleNumberArea->update(0, rect.y(), cycleNumberArea->width(), rect.height());

//    if (rect.contains(viewport()->rect()))
  //      updateCycleNumberAreaWidth(0);
}




void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    cycleNumberArea->setGeometry(QRect(cr.right()-cycleNumberAreaWidth(), cr.top(), cycleNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(QColor(10,40,50));

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
    painter.fillRect(event->rect(), QColor(50,60,70));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::yellow);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


void CodeEditor::cycleNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(cycleNumberArea);

    painter.fillRect(event->rect(), QColor(50,60,70));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter.setPen(Qt::yellow);
    painter.drawText(0, top, cycleNumberArea->width(), fontMetrics().height(),
                 Qt::AlignLeft, "#Cycles");

    top+=30;
    int bottom = top + (int) blockBoundingRect(block).height();


    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            int ln = blockNumber+1;
           // qDebug() << m_cycles.count();
            if (m_cycles.contains(ln))
            {
                QString number = QString::number(m_cycles[ln]);
                painter.setPen(Qt::yellow);
                painter.drawText(0, top, cycleNumberArea->width(), fontMetrics().height(),
                             Qt::AlignLeft, number);
            }
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
