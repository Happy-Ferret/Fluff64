#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QMainWindow>
#include "imageworker.h"
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include "data.h"
#include <QThread>
#include <thread>
#include <QLayout>
#include <QPointF>
#include <source/toolbox.h>



class WorkerThread : public QThread {
    Q_OBJECT

public:

    ImageWorker* m_work = nullptr;
    bool m_quit = false;
    Ui::MainWindow* ui;
    Toolbox* m_toolBox = nullptr;
    WorkerThread(ImageWorker* iw, Ui::MainWindow* _ui, Toolbox* toolbox) {
            m_work = iw;
            m_toolBox = toolbox;
            ui = _ui;
    }

    QPoint m_currentPos, m_prevPos;
    int m_currentButton = 0;
    float m_zoom = 1;
    bool m_isPanning = false;
    QPoint m_zoomCenter = QPoint(00,00);

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
