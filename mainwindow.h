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
#include "source/trsedocuments/formraseditor.h"
#include "source/trsedocuments/formimageeditor.h"
#include "source/dialogtrsesettings.h"
#include "source/messages.h"

#include "ui_mainwindow.h"
namespace Ui {
class MainWindow;

}


class TRSEProject {
public:
    CIniFile m_ini;
    QString m_filename;
    void Load(QString projectfile) {
        m_ini.Load(projectfile);
        m_filename = projectfile;
    }
    void Save() {
        m_ini.Save(m_filename);
    }
};


class CustomFileSystemModel : public QFileSystemModel {
    Q_OBJECT

  public:
    CustomFileSystemModel(QWidget* parent) : QFileSystemModel(parent) {

    }

    QVariant data ( const QModelIndex & index, int role ) const override
    {
        if( index.column() == 0 && role == Qt::DecorationRole ) {
            //you may want to check "index.data().toString()" for the file-name/-extension
            QImage img;
            QString f = index.data().toString();
            if (f.contains(".ras"))
                img.load(":resources/images/trselogo.png");
            if (f.contains(".asm"))
                img.load(":resources/images/asm_icon.png");
            if (f.contains(".prg"))
                img.load(":resources/images/cmb_icon.png");
            if (f.contains(".flf"))
                img.load(":resources/images/image_icon.png");

            QIcon ic(QPixmap::fromImage(img));

            return QVariant(ic);
        }
    else
        return QFileSystemModel::data(index, role);
    }


};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    CIniFile m_iniFile;
 //   CodeEditor m_codeEditor;
    CustomFileSystemModel *fileSystemModel;
    QString m_iniFileName = "fluff64.ini";

    QVector<TRSEDocument*> m_documents;


    TRSEProject m_currentProject;


    TRSEDocument* m_currentDoc = nullptr;

    void SearchInSource();
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent *e);

    WorkerThread* m_updateThread;

    void LoadDocument(QString fileName);

    bool m_quit = false;



  //  void setupEditor();
    void SetupFileList();

    void RefreshFileList();
public slots:


      void OnQuit();

    void updateImage() {
        FormImageEditor* imageEdit = dynamic_cast<FormImageEditor*>(ui->tabMain->currentWidget());
        if (imageEdit==nullptr)
            return;


        imageEdit->UpdateImage();

    }
    void closeWindowSlot();
    void updatePalette() {
        setPalette(m_updateThread->m_pal);
    }

    void UpdateRecentProjects();
    void SaveAs();

    void RemoveTab(int);
    void LoadProject(QString filename);

    void CloseAll();
    QString getProjectPath();

signals:
   void ValueChanged();


private slots:


    void on_treeFiles_doubleClicked(const QModelIndex &index);



    void on_tabMain_currentChanged(int index);

    void on_btnSave_3_clicked();

    void on_actionRas_source_file_triggered();

    void on_actionDelete_file_triggered();

    void on_actionImage_triggered();

    void on_actionSave_As_triggered();

    void on_actionTRSE_Settings_triggered();

    void on_actionNew_project_triggered();

    void on_actionClose_all_triggered();

    void on_actionOpen_project_triggered();

    void on_lstRecentProjects_itemDoubleClicked(QListWidgetItem *item);

private:

    QString FindPathInProjectFolders(const QModelIndex &index);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
