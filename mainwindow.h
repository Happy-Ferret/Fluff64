#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

namespace Ui {
    class MainWindow;
}



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

    QPoint m_currentPos;
    int m_currentButton = 0;
    float m_zoom = 1;
    QPoint m_zoomCenter = QPoint(00,00);

    void run() override;
    QImage* m_tmpImage = nullptr;
    QPixmap m_pixMapImage;


    void UpdateInput();
    void UpdateOutput();
    void UpdateDrawing();
    void UpdateMousePosition();
    void UpdateImage(MultiColorImage& mc);

signals:
    void updateImageSignal();

public slots:
    void OnQuit();
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ImageWorker m_work;
    Toolbox m_toolBox;

    LImage m_grid;
    QColor m_gridColor = QColor(64,128,128,128);

    void Convert();

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* e);

    WorkerThread* m_updateThread;

   bool m_quit = false;

public slots:
   void Update();
//   void OnQuit();

    void updateImage() {
        ui->lblImage->setPixmap(m_updateThread->m_pixMapImage);
        m_grid.ApplyToLabel(ui->lblGrid);

    }


signals:
   void ValueChanged();


private slots:
    void on_actionImport_triggered();

    void on_btnConvert_clicked();

    void on_hsContrast_sliderMoved(int position);

    void on_hsShift_sliderMoved(int position);

    void on_hsHsv_sliderMoved(int position);

    void on_hsSat_sliderMoved(int position);

    void on_hsBlur_sliderMoved(int position);


    void on_btnExportAsm_clicked();

    void on_cmbForeground_activated(int index);

    void on_cmbBackground_activated(int index);

    void on_pushButton_clicked();



    void on_btnLoad_clicked();

    void on_btnSave_clicked();

    void on_chkGrid_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
