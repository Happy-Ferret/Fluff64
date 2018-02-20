#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QThread>
#include "source/data.h"
#include "source/util/util.h"
#include <QWheelEvent>
#include "dialognewimage.h"
#include "source/limage/limageio.h"

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

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Z && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        m_work.m_currentImage->Undo();
    }
    if (e->key() == Qt::Key_Z  && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
       ui->chkGrid->setChecked(!ui->chkGrid->isChecked());
       ui->lblGrid->setVisible(ui->chkGrid->isChecked());
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
