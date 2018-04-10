#ifndef TRSEDOCUMENT_H
#define TRSEDOCUMENT_H

#include "source/workerthread.h"
#include "source/util/cinifile.h"
#include <QWidget>
#include "source/data.h"

class TRSEDocument : public QWidget
{
    Q_OBJECT
public:
    TRSEDocument(QWidget* parent);

    QString m_currentSourceFile;
    QString m_currentFileShort;

    QString m_fileExtension;

    WorkerThread* m_updateThread;
    CIniFile* m_iniFile;
    CIniFile* m_projectIniFile;

    virtual void Save(QString filename) {}
    virtual void Load(QString filename) {}
    virtual void AutoFormat() {}
    void SaveCurrent() {
        if (m_currentSourceFile=="") {
            Data::data.requestSaveAs = true;
        }
        Save(m_currentSourceFile);
        Data::data.blink = true;
    }
    virtual void Build() {}
    virtual void Run() {}

    virtual void UpdateFromIni() {}
    virtual void Init() {}
    virtual void UpdateColors() { }
    virtual void InitDocument(WorkerThread* t, CIniFile* ini, CIniFile* iniProject) {
        m_updateThread = t;
        m_iniFile = ini;
        m_projectIniFile = iniProject;

    }

    virtual void Reload() {}


    virtual void Destroy() = 0;

};

#endif // TRSEDOCUMENT_H
