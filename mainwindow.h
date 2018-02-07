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
    void UpdateInput();
    void UpdateOutput();
    void Convert();
    void UpdateImage(MultiColorImage& mc);

   // void Quit() override;

    std::thread* m_updateThread;

   bool m_quit = false;

public slots:
   void Update();
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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
