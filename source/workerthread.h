#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QMainWindow>
#include "imageworker.h"
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include "source/data.h"
#include <QThread>
#include <thread>
#include <QLayout>
#include <QPointF>
#include <QLabel>
#include <source/toolbox.h>
#include "ui_formimageeditor.h"



class WorkerThread : public QThread {
    Q_OBJECT

public:

    ImageWorker* m_work = nullptr;
    Toolbox* m_toolBox = nullptr;
    bool m_quit = false;
    QLabel* m_imgLabel;
    WorkerThread() {

    }


    void SetCurrentImage(ImageWorker* work, Toolbox* tb, QLabel* lbl) {
        m_work = work;
        m_toolBox = tb;
        m_imgLabel = lbl;
    }

    QPointF pos;

    QPoint m_currentPos, m_prevPos;
    int m_currentButton = 0;
    float m_zoom = 1;
    bool m_isPanning = false;
    QPoint m_zoomCenter = QPoint(00,00);
    QPoint m_currentPosInImage;

    void run() override;
    QImage* m_tmpImage = nullptr;
    QPixmap m_pixMapImage;
    bool isPreview;

    void UpdateDrawing();
    void UpdateMousePosition();
    void UpdatePanning();
    void UpdateImage(LImage* mc);

signals:
    void updateImageSignal();

public slots:
    void OnQuit();
};


#endif // WORKERTHREAD_H
