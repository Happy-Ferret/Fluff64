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
    m_toolBox.Initialize(ui->lyToolbox_3);;
    UpdatePalette();
    m_grid.Initialize(320*2,200*2);
    m_grid.CreateGrid(40,25,m_gridColor,4, 1, QPoint(0,0));


    m_grid.ApplyToLabel(ui->lblGrid);


    //m_updateThread = new QThread(this);
//    m_updateThread->mw = this;
    //QObject::connect(m_updateThread, SIGNAL(valueChanged()), this, SLOT (Update()));

    m_updateThread = new WorkerThread(&m_work, ui, &m_toolBox);
    connect(m_updateThread, SIGNAL(updateImageSignal()), this, SLOT(updateImage()));

    connect(qApp, SIGNAL(aboutToQuit()), m_updateThread, SLOT(OnQuit()));

    m_updateThread->start();

    m_iniFile.Load("fluff64.ini");
    setupEditor();

    if (m_iniFile.getString("current_file")!="")
        LoadRasFile(m_iniFile.getString("current_file"));




#ifndef USE_LIBTIFF
    ui->btnTiff->setVisible(false);
#endif
//    m_updateThread->join();
//    m_updateThread->
}



MainWindow::~MainWindow()
{
    m_quit = true;
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::RightButton)
        m_updateThread->m_currentButton = 2;
    if(e->buttons() == Qt::LeftButton) {
        m_updateThread->m_currentButton = 1;
        m_work.m_currentImage->AddUndo();

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
    float f = event->delta()/100.0f;

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        m_updateThread->m_zoom -=f*0.05;
        m_updateThread->m_zoom = min(m_updateThread->m_zoom, 1.0f);
        m_updateThread->m_zoom = max(m_updateThread->m_zoom, 0.1f);
        float t = 0.0f;
        m_updateThread->m_zoomCenter = (m_updateThread->m_zoomCenter*t + (1-t)*m_updateThread->m_currentPos);//*(2-2*m_zoom);
        Data::data.redrawOutput = true;

        m_grid.CreateGrid(40,25,m_gridColor,4, m_updateThread->m_zoom, m_updateThread->m_zoomCenter);

    }
    else {
        m_toolBox.m_current->m_size +=f;
        m_toolBox.m_current->m_size = max(m_toolBox.m_current->m_size,1.0f);
        m_toolBox.m_current->m_size = min(m_toolBox.m_current->m_size,50.0f);
    }
    Data::data.redrawOutput = true;
    Data::data.forceRedraw = true;

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
//    qDebug() << (QApplication::keyboardModifiers() & Qt::ShiftModifier) << " + "  << rand()%100;
    if (e->key()==Qt::Key_Shift) {
        m_toolBox.m_current->m_type = 1;
        Data::data.redrawOutput = true;
        Data::data.forceRedraw = true;
    }

    if (e->key()==Qt::Key_F && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        if (ui->leSearch->hasFocus()) {
            m_searchFromPos=m_currentFromPos+1;
            SearchInSource();
        }
        else {
            ui->leSearch->setText("");
            ui->leSearch->setFocus();
            m_searchFromPos = 0;
        }
    }


    if (e->key()==Qt::Key_Z && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        m_work.m_currentImage->Undo();

    }
    if (e->key() == Qt::Key_Z  && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
       ui->chkGrid->setChecked(!ui->chkGrid->isChecked());
       ui->lblGrid->setVisible(ui->chkGrid->isChecked());

    }
    if (e->key() == Qt::Key_S &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        on_btnSave_2_clicked();
    }
    if (e->key() == Qt::Key_B &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
    }
    if (e->key() == Qt::Key_R &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
        Run();
    }
    Data::data.forceRedraw = true;
    Data::data.Redraw();



}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Shift) {
        m_toolBox.m_current->m_type = 0;
        Data::data.redrawOutput = true;
        Data::data.forceRedraw = true;
    }

}

void MainWindow::UpdatePalette()
{
    if (m_work.m_currentImage==nullptr)
        return;
    LColorList* l = &m_work.m_currentImage->m_image->m_colorList;

    l->CreateUI(ui->layoutColorsEdit_3,1);
    l->FillComboBox(ui->cmbBackgroundMain_3);
    l->FillComboBox(ui->cmbBorderMain_3);

}

void MainWindow::LoadRasFile(QString fileName)
{
    m_currentSourceFile = m_iniFile.getString("project_path") + "/" + fileName;

    QFile file(m_currentSourceFile);
    if (file.open(QFile::ReadOnly | QFile::Text))
        ui->txtEditor->setPlainText(file.readAll());


}


void MainWindow::on_actionImport_triggered()
{

}

void MainWindow::on_btnConvert_clicked()
{
    //m_work.Convert();
    //m_updateThread->UpdateOutput();

}
void MainWindow::Update()
{

}

void MainWindow::MainWindow::setupEditor()
{
//    return;
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(12);


    // Set up file system model
    fileSystemModel = new QFileSystemModel(this);
    QString rootPath= m_iniFile.getString("project_path");
    fileSystemModel->setReadOnly(true);
    fileSystemModel->setRootPath(rootPath);
    fileSystemModel->setFilter(QDir::NoDotAndDotDot |
                            QDir::AllDirs |QDir::AllEntries);
    //fileSystemModel->setNameFilters(QStringList() << "*.ras");
    ui->treeFiles->setModel(fileSystemModel);
    ui->treeFiles->setRootIndex(fileSystemModel->index(rootPath));
    ui->treeFiles->hideColumn(1);
    ui->treeFiles->hideColumn(2);
    ui->treeFiles->hideColumn(3);

//    ui->txtEditor = &m_codeEditor;

    ui->txtEditor->setFont(font);
    //ui->txtEditor->setTextColor(QColor(220,210,190));
    highlighter = new Highlighter(ui->txtEditor->document());

    QFontMetrics metrics(font);
    ui->txtEditor->setTabStopWidth(m_iniFile.getInt("tab_size") * metrics.width(' '));


}

void MainWindow::Build()
{
    QString text = ui->txtEditor->toPlainText();
    ErrorHandler::e.m_level = ErrorHandler::e.ERROR_ONLY;
    ErrorHandler::e.m_teOut = "";
    ErrorHandler::e.exitOnError = false;
    QStringList lst = text.split("\n");
 //   text = text.replace("\n","");
//    SymbolTable::isInitialized = true;

    QElapsedTimer timer;
    timer.start();
    Lexer lexer = Lexer(text, lst);
    Parser parser = Parser(lexer);
    Interpreter interpreter = Interpreter(parser);
    interpreter.Parse();
//    interpreter.Interpret();

    //interpreter.Build(Interpreter::PASCAL);
    //interpreter.SaveBuild(m_outputFilename+".pmm");

    if (interpreter.Build(Interpreter::MOS6502)) {
        interpreter.SaveBuild(m_outputFilename+".asm");
        QString text ="Build <b>Successful</b>!<br>";
        text+="Compiled " + QString::number(parser.m_lexer.m_lines.count()) +" of Rascal to ";
        text+=QString::number(interpreter.m_assembler->m_source.count()) + " lines of DASM assembler<br>";
        text+="Time: " + Util::MilisecondToString(timer.elapsed())+"<br>";
        text+="**** DASM output:<br>";
        QProcess process;
        QString name = m_outputFilename;

        process.start(m_iniFile.getString("dasm"), QStringList()<<(name+".asm") << ("-o"+name+".prg"));
        process.waitForFinished();
        QString output(process.readAllStandardOutput());
        QString size = QString::number(QFile("program.prg").size());

        output +="<br>Assembled file size: <b>" + size + "</b> bytes";

        ui->txtOutput->setText(text + output);
    }
    else {
        ui->txtOutput->setText(ErrorHandler::e.m_teOut);
        int ln = Pmm::Data::d.lineNumber;
        QTextCursor cursor(ui->txtEditor->document()->findBlockByLineNumber(ln));
        ui->txtEditor->setTextCursor(cursor);

    }

}


void MainWindow::on_btnExportAsm_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Multicolor Assembler image"), "",
                                                    tr("Asm (*.asm);"));


    m_work.m_currentImage->m_image->ExportAsm(fileName);
}

void MainWindow::on_pushButton_clicked()
{
    /*    m_work.m_currentImage->m_image->CopyFrom(&m_work.m_converter.m_mcImage);
    ui->tabWidget->setCurrentIndex(0);
    m_updateThread->UpdateImage((LImage*)m_work.m_currentImage->m_image);*/

}

void MainWindow::on_btnLoad_clicked()
{
    QString f = "Image Files (*." + LImageIO::m_fileExtension + ")";
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", f);
    if (filename=="")
        return;

    LImage* img = LImageIO::Load(filename);

    m_work.New(img, filename);

    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

//    m_updateThread->UpdateImage(m_work.m_currentImage->m_image);
    //delete img;

}

void MainWindow::on_btnSave_clicked()
{
    QString filename = m_work.m_currentImage->m_fileName;

    if (filename=="")
        filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), "", ("Image Files (*." + LImageIO::m_fileExtension + ")"));
    if (filename=="")
        return;

    LImageIO::Save(filename,m_work.m_currentImage->m_image);

}

void MainWindow::on_chkGrid_clicked(bool checked)
{
//    if (checked)
        ui->lblGrid->setVisible(checked);
}


void MainWindow::on_cmbBackgroundMain_currentIndexChanged(int index)
{
    m_work.m_currentImage->m_image->setBackground(index);
    Data::data.redrawOutput = true;
}


void MainWindow::on_btnNew_clicked()
{
    m_work.m_currentImage->m_image->Clear();
    Data::data.Redraw();
}

void MainWindow::on_btnExportImage_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "", tr("Image Files (*.png *.jpg *.bmp )"));

    if (fileName == "")
        return;

    m_updateThread->m_tmpImage->save(fileName);


}

void MainWindow::on_b_clicked()
{


    DialogNewImage* dNewFile = new DialogNewImage(this);
    dNewFile->Initialize(m_work.getImageTypes());
    dNewFile->setModal(true);
    dNewFile->exec();
    if (dNewFile->retVal!=-1) {
        m_work.New(dNewFile->retVal);
    }



    UpdatePalette();

    delete dNewFile;
}

void MainWindow::on_lstImages_clicked(const QModelIndex &index)
{
    m_work.SetImage(index.row());
    Data::data.redrawFileList = true;
    UpdatePalette();
}

void MainWindow::on_btnImport_clicked()
{
    DialogImport* di = new DialogImport(this);
    di->Initialize(m_work.m_currentImage->m_imageType->type, m_work.m_currentImage->m_image->m_colorList.m_type);
    di->exec();
    if (di->m_ok) {
        m_work.m_currentImage->m_image->CopyFrom(di->m_image);
        Data::data.redrawOutput = true;
    }

}

void MainWindow::on_btnTiff_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Tiled Tiff"), "", tr("Image Files (*.tif *.tiff )"));

    if (fileName == "")
        return;


    LImageTiff *tif = (LImageTiff*)LImageFactory::Create(LImage::Tiff, LColorList::TIFF);
    tif->Initialize(640,480);
    tif->LoadTiff(fileName);
//    tif->m_type = LImage::Tiff;
    m_work.New(tif, fileName);
    Data::data.redrawFileList = true;
    Data::data.Redraw();
    UpdatePalette();

}

void MainWindow::on_btnSaveAs_clicked()
{

    QString filename = QFileDialog::getSaveFileName(this,
                                                tr("Save Image"), "", ("Image Files (*." + LImageIO::m_fileExtension + ")"));
    if (filename=="")
        return;
    m_work.m_currentImage->m_fileName  =filename;

    LImageIO::Save(filename,m_work.m_currentImage->m_image);

}

void MainWindow::on_btnBuild_clicked()
{
    Build();
}

void MainWindow::on_btnBuild_2_clicked()
{
    Run();
}

void MainWindow::Run() {
    QProcess process;
    process.waitForFinished();
    process.startDetached(m_iniFile.getString("emulator"), QStringList() << (m_outputFilename+".prg"));
    QString output(process.readAllStandardOutput());
    qDebug() << output;

}

void MainWindow::on_btnSave_2_clicked()
{
    if (QFile::exists(m_currentSourceFile))
        QFile::remove(m_currentSourceFile);
    QString txt = ui->txtEditor->document()->toPlainText();
    QFile file(m_currentSourceFile);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << txt;
    }
    file.close();
    m_iniFile.Save("fluff64.ini");
}

void MainWindow::on_treeFiles_doubleClicked(const QModelIndex &index)
{
    //qDebug() << ui->treeFiles->model()->index()
    //qDebug() << index.data().toString();
    LoadRasFile(index.data().toString());
    m_iniFile.setString("current_file", index.data().toString());
}

void MainWindow::on_leSearch_textChanged()
{
    QString i;
    SearchInSource();
}

void MainWindow::SearchInSource()
{
    m_currentFromPos = ui->txtEditor->document()->toPlainText().toLower().indexOf(ui->leSearch->text().toLower(), m_searchFromPos);
    QTextCursor cursor(ui->txtEditor->document()->findBlock(m_currentFromPos));
    ui->txtEditor->setTextCursor(cursor);
}


void MainWindow::on_leSearch_returnPressed()
{
    ui->txtEditor->setFocus();
}
