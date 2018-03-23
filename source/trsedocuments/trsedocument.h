#ifndef TRSEDOCUMENT_H
#define TRSEDOCUMENT_H

#include "source/workerthread.h"
#include "source/util/cinifile.h"
#include <QWidget>

class TRSEDocument : public QWidget
{
public:
    TRSEDocument(QWidget* parent);

    QString m_currentSourceFile;
    QString m_currentFileShort;


    WorkerThread* m_updateThread;
    CIniFile* m_iniFile;

    virtual void Save(QString filename) {}
    virtual void Load(QString filename) {}
    virtual void Save() {
        Save(m_currentSourceFile);
    }
    virtual void Init() {}

    virtual void InitDocument(WorkerThread* t, CIniFile* ini) {
        m_updateThread = t;
        m_iniFile = ini;
    }

    virtual void Destroy() = 0;

};

#endif // TRSEDOCUMENT_H
