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
#include "source/workerthread.h"

namespace Ui {
    class MainWindow;
}





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ImageWorker m_work;
    Toolbox m_toolBox;

    LImageQImage m_grid;
    QColor m_gridColor = QColor(64,128,128,128);


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

    void on_cmbBackgroundMain_currentIndexChanged(int index);

    void on_btnNew_clicked();

    void on_btnExportImage_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
