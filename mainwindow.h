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

#include "ui_mainwindow.h"
namespace Ui {
class MainWindow;

}





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    CIniFile m_iniFile;
 //   CodeEditor m_codeEditor;
    QFileSystemModel *fileSystemModel;
    QString m_iniFileName = "fluff64.ini";

    QVector<TRSEDocument*> m_documents;


    TRSEDocument* m_currentDoc;

    void SearchInSource();
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent *e);

    WorkerThread* m_updateThread;

    void LoadDocument(QString fileName, QString type);

    bool m_quit = false;


  //  void setupEditor();
    void SetupFileList();

    void RefreshFileList();
public slots:

    //   void OnQuit();

    void updateImage() {
        FormImageEditor* imageEdit = dynamic_cast<FormImageEditor*>(ui->tabMain->currentWidget());
        if (imageEdit==nullptr)
            return;


        imageEdit->UpdateImage();

    }


    void RemoveTab(int);

signals:
   void ValueChanged();


private slots:


    void on_treeFiles_doubleClicked(const QModelIndex &index);



    void on_tabMain_currentChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
