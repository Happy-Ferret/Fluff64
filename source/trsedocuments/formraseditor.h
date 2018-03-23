#ifndef FORMRASEDITOR_H
#define FORMRASEDITOR_H

#include <QWidget>
#include "trsedocument.h"
#include "source/PmmEdit/highlighter.h"
#include "source/PmmEdit/codeeditor.h"

namespace Ui {
    class FormRasEditor;
}

class FormRasEditor : public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormRasEditor(QWidget *parent = 0);
    ~FormRasEditor();
    Highlighter* highlighter;

    int m_searchFromPos = 0;
    int m_currentFromPos = 0;
    bool m_buildSuccess = false;
    void LoadRasFile(QString fileName);
    void ExecutePrg(QString fileName);
    void InitDocument(WorkerThread *t, CIniFile *ini) override;
    void setupEditor();
    void Build();
    void Setup();
    void Run();
    void SetLights();
    void SetText(QString s);
    void SearchInSource();

    void Save(QString filename) override;
    void Load(QString filename) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent* e) override;

    void Destroy() override {}

    Ui::FormRasEditor* UI(){return ui;}

private:
    Ui::FormRasEditor *ui;

private slots:
    void on_leSearch_textChanged();

    void on_leSearch_returnPressed();

    void on_leSearch_textChanged(const QString &arg1);

};

#endif // FORMRASEDITOR_H
