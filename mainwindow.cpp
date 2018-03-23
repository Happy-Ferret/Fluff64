#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QElapsedTimer>
#include <QTextCursor>
#include <QFontMetrics>
#include "source/data.h"
#include "source/util/util.h"
#include <QWheelEvent>
#include "dialognewimage.h"
#include "source/limage/limageio.h"

#include "source/errorhandler.h"
#include "source/parser.h"
#include "source/interpreter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // m_work.m_colorList.CreateUI(ui->layoutColors,0);



    //QObject::connect(m_updateThread, SIGNAL(valueChanged()), this, SLOT (Update()));
    m_updateThread = new WorkerThread();
    connect(m_updateThread, SIGNAL(updateImageSignal()), this, SLOT(updateImage()));


    connect( ui->tabMain, SIGNAL(tabCloseRequested(int)),this, SLOT(RemoveTab(int)));
    connect(qApp, SIGNAL(aboutToQuit()), m_updateThread, SLOT(OnQuit()));

    m_updateThread->start();

  //  ui->centralWidget->setLayout(new QGridLayout());

    m_iniFile.Load(m_iniFileName);
    m_iniFile.setString("project_path", m_iniFile.getString("project_path").replace("\\","/"));
    //setupEditor();
    SetupFileList();
//    if (m_iniFile.getString("current_file")!="")
  //      LoadRasFile(m_iniFile.getString("current_file"));




#ifndef USE_LIBTIFF
    //ui->btnTiff->setVisible(false);
#endif
//    m_updateThread->join();
//    m_updateThread->
}



MainWindow::~MainWindow()
{
    m_quit = true;
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    FormImageEditor* fe = dynamic_cast<FormImageEditor*>(m_currentDoc);
    if (fe==nullptr)
        return;
//    m_updateThread->pos = mapToGlobal( fe->getLabelImage()->rect().topLeft());
  //  qDebug() << m_updateThread->pos;
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::RightButton)
        m_updateThread->m_currentButton = 2;
    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
    }


}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_updateThread->m_currentButton==2)
        m_updateThread->m_currentButton = 0;
    else
        m_updateThread->m_currentButton = -1;

}

void MainWindow::wheelEvent(QWheelEvent *event)
{


}

void MainWindow::keyPressEvent(QKeyEvent *e)
{

    Data::data.forceRedraw = true;
    Data::data.Redraw();

}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{

}




void MainWindow::LoadDocument(QString fileName, QString type)
{
    for (TRSEDocument* d: m_documents) {
        if (d->m_currentFileShort==fileName) {
            ui->tabMain->setCurrentWidget(d);
            return;
        }
    }

    TRSEDocument* editor = nullptr;
    if (type=="flf") {
        editor = new FormImageEditor(this);
        FormImageEditor* fe = (FormImageEditor*)editor;
        m_updateThread->SetCurrentImage(&fe->m_work, &fe->m_toolBox, fe->getLabelImage());

    }
    if (type=="ras") {
        editor = new FormRasEditor(this);
    }
    editor->InitDocument(m_updateThread, &m_iniFile);
    editor->m_currentSourceFile = m_iniFile.getString("project_path") + "/" + fileName;
    editor->m_currentFileShort = fileName;
    ui->tabMain->addTab(editor, fileName);
    editor->Load(editor->m_currentSourceFile);


    editor->setFocus();
    editor->showMaximized();
    ui->tabMain->setCurrentWidget(editor);

    //m_iniFile.setString("current_file", fileName);
    //m_buildSuccess = false;
    ui->tabMain->setTabsClosable(true);
    m_documents.append(editor);
    m_currentDoc = editor;


}

void MainWindow::SetupFileList()
{
    RefreshFileList();
    ui->treeFiles->hideColumn(1);
    ui->treeFiles->hideColumn(2);
    ui->treeFiles->hideColumn(3);
}





void MainWindow::RefreshFileList()
{
    fileSystemModel = new QFileSystemModel(this);
    QString rootPath= m_iniFile.getString("project_path");
    fileSystemModel->setReadOnly(true);
    fileSystemModel->setRootPath(rootPath);
    fileSystemModel->setFilter(QDir::NoDotAndDotDot |
                            QDir::AllDirs |QDir::AllEntries);
    fileSystemModel->setNameFilters(QStringList() << "*.ras" << "*.asm" << "*.txt" << "*.prg" << "*.inc" << "*.flf");
    fileSystemModel->setNameFilterDisables(false);
    ui->treeFiles->setModel(fileSystemModel);
    ui->treeFiles->setRootIndex(fileSystemModel->index(rootPath));

}

void MainWindow::RemoveTab(int idx)
{
    if (idx==0)
        return;
    idx--;
    TRSEDocument* doc = m_documents[idx];
    m_documents[idx]->Destroy();
    m_documents.remove(idx);
   ui->tabMain->removeTab(idx+1);


   m_updateThread->SetCurrentImage(nullptr, nullptr, nullptr);
   qDebug() << "A;";
    TRSEDocument* d = (TRSEDocument*)ui->tabMain->currentWidget();
    FormImageEditor* fe = dynamic_cast<FormImageEditor*>(d);
    qDebug() << "B;";

    if (d!=nullptr)
   //if (ui->tabMain->currentIndex()==1)
   {
       m_updateThread->SetCurrentImage(&fe->m_work, &fe->m_toolBox,fe->getLabelImage());
   }

   delete doc;
}


void MainWindow::on_treeFiles_doubleClicked(const QModelIndex &index)
{
    // Find file in path.. ugh
    QString path = "";
    QStringList pathSplit = m_iniFile.getString("project_path").toLower().replace("\\", "/").split("/");
    QString test = pathSplit.last();
    if (test=="")
        test = pathSplit[pathSplit.count()-2];

    QModelIndex cur = index.parent();
    int cnt=0;
    while (cur.data().toString().toLower()!=test) {

        path=cur.data().toString() + "/" + path;
        cur = cur.parent();
        if (cnt++>20)
            return;
    }


    // Finally load file!
    QString file = index.data().toString();
    if (file.toLower().endsWith(".ras") || file.toLower().endsWith(".asm")
            || file.toLower().endsWith(".inc")) {
        LoadDocument(path + file,"ras");
    }
    if (file.toLower().endsWith(".flf")) {
        LoadDocument(path + file,"flf");
    }
    if (file.toLower().endsWith(".prg")) {
        //ExecutePrg(m_iniFile.getString("project_path")+"/" + file);
    }

    Data::data.Redraw();
    Data::data.forceRedraw = true;
}


/*
QFileDialog dialog;
dialog.setFileMode(QFileDialog::AnyFile);
QString f = "Ras Files (*.ras)";
QString filename = dialog.getSaveFileName(NULL, "Create New File",m_iniFile->getString("project_path"),f);

if (filename=="")
    return;
QString orgFile;
//filename = filename.split("/").last();
filename = filename.toLower().remove(m_iniFile->getString("project_path").toLower());

//qDebug() << filename;
QString fn = m_iniFile->getString("project_path") + filename;
if (QFile::exists(fn))
    QFile::remove(fn);
QFile file(fn);
qDebug() << "creating new file: " + fn;
if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream s(&file);
    s<< "program spankme;\n";
    s<< "var  \n";
    s<< "   index: byte; \n";
    s<< "begin\n\n";
    s<< "end.\n";
    qDebug() << "Done writing;";
}

file.close();
LoadRasFile(filename);
RefreshFileList();
*/

void MainWindow::on_tabMain_currentChanged(int index)
{
    FormImageEditor* imageedit = dynamic_cast<FormImageEditor*>(ui->tabMain->widget(index));
    FormRasEditor* rasedit = dynamic_cast<FormRasEditor*>(ui->tabMain->widget(index));
    if (rasedit!=nullptr) {
        m_updateThread->SetCurrentImage(nullptr, nullptr, nullptr);
    }
    if (imageedit!=nullptr) {
        m_updateThread->SetCurrentImage(&imageedit->m_work, &imageedit->m_toolBox, imageedit->getLabelImage());
    }
    m_currentDoc = (TRSEDocument*)ui->tabMain->widget(index);
}
