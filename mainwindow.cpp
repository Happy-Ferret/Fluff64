#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QThread>
#include "data.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_work.m_colorList.CreateUI(ui);
    m_work.m_colorList.FillComboBox(ui->cmbForeground);
    m_work.m_colorList.FillComboBox(ui->cmbBackground);

    //m_updateThread = new QThread(this);
//    m_updateThread->mw = this;
    //QObject::connect(m_updateThread, SIGNAL(valueChanged()), this, SLOT (Update()));

    m_updateThread = new std::thread(&MainWindow::Update, this);

//    m_updateThread->join();
//    m_updateThread->
}



MainWindow::~MainWindow()
{
    m_quit = true;
    delete ui;
}

void MainWindow::UpdateInput()
{
    return;
    /*QPixmap p;
    p.convertFromImage(*m_work.m_input.m_qImage);
    ui->lblOne->setPixmap(p);*/
}

void MainWindow::UpdateOutput()
{

    m_work.Convert();
    QPixmap p;
    p.convertFromImage(*m_work.m_converter.m_output.m_qImage);
    ui->lblTwo->setPixmap(p);

}

void MainWindow::Convert()
{

}

void MainWindow::UpdateImage(MultiColorImage &mc)
{
    QImage* image = mc.ToQImage(m_work.m_colorList);
    QPixmap p;
    p.convertFromImage(*image);
    ui->lblImage->setPixmap(p);
    delete image;

}


void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    m_work.Import(fileName);
    UpdateInput();
    m_work.Blur();

    m_work.Convert();
    UpdateOutput();

}

void MainWindow::on_btnConvert_clicked()
{
    m_work.Convert();
    UpdateOutput();

}

void MainWindow::on_hsContrast_sliderMoved(int position)
{
    m_work.m_converter.m_contrast = (float)position/100.0*4;;
    m_work.Convert();
    UpdateOutput();

}

void MainWindow::on_hsShift_sliderMoved(int position)
{
    m_work.m_converter.m_shift = ((float)position/100.0 - 0.5) * 255;;
    m_work.Convert();
    UpdateOutput();

}


void MainWindow::Update()
{

    while (!m_quit) {
        if (Data::data.redraw) {
            m_work.Convert();
            UpdateOutput();
        }

        Data::data.redraw = false;
        thread()->sleep(0.1);
    }

}

void MainWindow::on_hsHsv_sliderMoved(int position)
{
    m_work.m_converter.m_hsv = ((float)position/100.0) * 1;;
    m_work.Convert();
    UpdateOutput();

}

void MainWindow::on_hsSat_sliderMoved(int position)
{
    m_work.m_converter.m_saturation = ((float)position/100.0) * 1;
    m_work.Convert();
    UpdateOutput();

}

void MainWindow::on_hsBlur_sliderMoved(int position)
{
    m_work.m_converter.m_blur = ((float)position/100.0) ;
    m_work.Blur();
    UpdateOutput();

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
    UpdateOutput();
}

void MainWindow::on_cmbBackground_activated(int index)
{
    m_work.m_converter.m_mcImage.setBackground(index);
    UpdateOutput();
}

void MainWindow::on_pushButton_clicked()
{
    m_work.m_editor.m_image.CopyFrom(m_work.m_converter.m_mcImage);
    ui->tabWidget->setCurrentIndex(0);
    UpdateImage(m_work.m_editor.m_image);
}

void MainWindow::on_btnLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open LMC image"), "", tr("Image Files (*.lmc)"));
    if (filename=="")
        return;
    m_work.m_editor.m_image.Load(filename);
    UpdateImage(m_work.m_editor.m_image);


}

void MainWindow::on_btnSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save LMC image"), "", tr("Image Files (*.lmc)"));
    if (filename=="")
        return;

    m_work.m_editor.m_image.Save(filename);

}
