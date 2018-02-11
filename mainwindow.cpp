#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QThread>
#include "data.h"
#include "source/util/util.h"
#include <QWheelEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_work.m_colorList.CreateUI(ui->layoutColors,0);
    m_work.m_colorList.CreateUI(ui->layoutColorsEdit,1);
    m_work.m_colorList.FillComboBox(ui->cmbForeground);
    m_work.m_colorList.FillComboBox(ui->cmbBackground);
    m_work.m_colorList.FillComboBox(ui->cmbBackgroundMain);
    m_work.m_colorList.FillComboBox(ui->cmbBorderMain);
    m_toolBox.Initialize(ui->lyToolbox);;

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
        m_work.m_editor.AddUndo();

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
        m_work.m_editor.Undo();
    }
    if (e->key() == Qt::Key_Z  && !(QApplication::keyboardModifiers() & Qt::ControlModifier)) {
       ui->chkGrid->setChecked(!ui->chkGrid->isChecked());
       ui->lblGrid->setVisible(ui->chkGrid->isChecked());
    }


}


void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    m_work.Import(fileName);
    m_updateThread->UpdateInput();
    m_work.Blur();

    m_work.Convert();
    m_updateThread->UpdateOutput();

}

void MainWindow::on_btnConvert_clicked()
{
    m_work.Convert();
    m_updateThread->UpdateOutput();

}

void MainWindow::on_hsContrast_sliderMoved(int position)
{
    m_work.m_converter.m_contrast = (float)position/100.0*4;;
    m_work.Convert();
    m_updateThread->UpdateOutput();

}

void MainWindow::on_hsShift_sliderMoved(int position)
{
    m_work.m_converter.m_shift = ((float)position/100.0 - 0.5) * 255;;
    m_work.Convert();
    m_updateThread->UpdateOutput();

}


void MainWindow::Update()
{

}


void MainWindow::on_hsHsv_sliderMoved(int position)
{
    m_work.m_converter.m_hsv = ((float)position/100.0) * 1;;
    m_work.Convert();
    m_updateThread->UpdateOutput();

}

void MainWindow::on_hsSat_sliderMoved(int position)
{
    m_work.m_converter.m_saturation = ((float)position/100.0) * 1;
    m_work.Convert();
    m_updateThread->UpdateOutput();

}

void MainWindow::on_hsBlur_sliderMoved(int position)
{
    m_work.m_converter.m_blur = ((float)position/100.0) ;
    m_work.Blur();
    m_updateThread->UpdateOutput();

}


void MainWindow::on_btnExportAsm_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
          tr("Export Multicolor Assembler image"), "",
          tr("Asm (*.asm);"));


    m_work.m_editor.m_image.ExportAsm(fileName);
}

void MainWindow::on_cmbForeground_activated(int index)
{
    m_work.m_converter.m_mcImage.setForeground(index);
    m_updateThread->UpdateOutput();
}

void MainWindow::on_cmbBackground_activated(int index)
{
    m_work.m_converter.m_mcImage.setBackground(index);
    m_updateThread->UpdateOutput();
}

void MainWindow::on_pushButton_clicked()
{
    m_work.m_editor.m_image.CopyFrom(m_work.m_converter.m_mcImage);
    ui->tabWidget->setCurrentIndex(0);
    m_updateThread->UpdateImage(m_work.m_editor.m_image);
}

void MainWindow::on_btnLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open LMC image"), "", tr("Image Files (*.lmc)"));
    if (filename=="")
        return;
    m_work.m_editor.m_image.Load(filename);
    m_updateThread->UpdateImage(m_work.m_editor.m_image);


}

void MainWindow::on_btnSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save LMC image"), "", tr("Image Files (*.lmc)"));
    if (filename=="")
        return;

    m_work.m_editor.m_image.Save(filename);

}

void MainWindow::on_chkGrid_clicked(bool checked)
{
//    if (checked)
        ui->lblGrid->setVisible(checked);
}


void MainWindow::on_cmbBackgroundMain_currentIndexChanged(int index)
{
    m_work.m_editor.m_image.setBackground(index);
    Data::data.redrawOutput = true;
}


void MainWindow::on_btnNew_clicked()
{
    m_work.m_editor.m_image.Clear();
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
