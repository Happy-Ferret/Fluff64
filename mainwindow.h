#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageworker.h"
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include "source/data.h"
#include <QThread>
#include <thread>
#include <QFileSystemModel>
#include <QLayout>
#include <QPointF>
#include <source/toolbox.h>
#include "source/workerthread.h"
#include "imageeditor.h"
#include "dialogimport.h"
#include "source/PmmEdit/highlighter.h"
#include "source/PmmEdit/codeeditor.h"
#include "source/util/cinifile.h"

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
    CIniFile m_iniFile;
    LImageQImage m_grid;
    QColor m_gridColor = QColor(64,128,128,128);
    QString m_currentSourceFile;
    CodeEditor m_codeEditor;
    QFileSystemModel *fileSystemModel;
    int m_searchFromPos = 0;
    int m_currentFromPos = 0;
    bool m_buildSuccess = false;
    void SearchInSource();

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent *e);
    void UpdatePalette();
    void LoadRasFile(QString fileName);
    void ExecutePrg(QString fileName);
    void SetLights();

    WorkerThread* m_updateThread;

    bool m_quit = false;

    void setupEditor();
    Highlighter* highlighter;
    void Build();
    void Run();

    void RefreshFileList();
    void updateCharSet();

    void SetMCColors();
public slots:
    void Update();
    //   void OnQuit();

    void updateImage() {
        ui->lblImage->setPixmap(m_updateThread->m_pixMapImage);
        m_grid.ApplyToLabel(ui->lblGrid);
        if (Data::data.redrawFileList) {
            m_work.UpdateListView(ui->lstImages);
            Data::data.redrawFileList = false;
        }

    }



signals:
   void ValueChanged();


private slots:
    void on_actionImport_triggered();

    void on_btnConvert_clicked();



    void on_btnExportAsm_clicked();


    void on_pushButton_clicked();



    void on_btnLoad_clicked();

    void on_btnSave_clicked();

    void on_chkGrid_clicked(bool checked);

    void on_cmbBackgroundMain_currentIndexChanged(int index);

    void on_btnNew_clicked();

    void on_btnExportImage_clicked();

    void on_b_clicked();

    void on_lstImages_clicked(const QModelIndex &index);

    void on_btnImport_clicked();

    void on_btnTiff_clicked();

    void on_btnSaveAs_clicked();

    void on_btnBuild_clicked();

    void on_btnBuild_2_clicked();

    void on_btnSave_2_clicked();

    void on_treeFiles_doubleClicked(const QModelIndex &index);

    void on_leSearch_textChanged();

    void on_leSearch_returnPressed();

    void on_actionNew_triggered();



    void on_btnExpChar_clicked();

    void on_leSearch_textChanged(const QString &arg1);

    void on_btnImportBin_clicked();

    void on_btnExportBin_clicked();

    void on_tabWidget_2_currentChanged(int index);

    void on_btnLoadCharmap_clicked();

    void on_lstCharMap_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_cmbMC1_currentIndexChanged(int index);

    void on_cmbMC2_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
