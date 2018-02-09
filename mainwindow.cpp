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
    m_toolBox.Initialize(ui->lyToolbox);;

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

void MainWindow::UpdateDrawing()
{
    m_work.m_editor.m_temp.CopyFrom(m_work.m_editor.m_image);

    QPoint pos = m_currentPos;
    if (pos.x()>=0 && pos.x()<m_work.m_editor.m_image.m_width &&
       pos.y()>=0 && pos.y()<m_work.m_editor.m_image.m_height) {

        MultiColorImage* img = &m_work.m_editor.m_image;
        //qDebug() << m_currentButton;
        if (m_currentButton == 0) {
            img = &m_work.m_editor.m_temp;
        }


        m_toolBox.m_current->Perform(pos.x(), pos.y(), Data::data.currentColor, img);

        Data::data.redrawOutput =true;
    }

}

void MainWindow::UpdateMousePosition()
{
    QPointF pos = QCursor::pos() - ui->lblImage->mapToGlobal(ui->lblImage->rect().topLeft());
    pos.setX(pos.x()/(float)ui->lblImage->width()*m_work.m_editor.m_image.m_width);
    pos.setY(pos.y()/(float)ui->lblImage->height()*m_work.m_editor.m_image.m_height);
    m_currentPos = QPoint(pos.x(), pos.y());

//    qDebug() << QApplication()::mouseButtons();
}




void MainWindow::UpdateImage(MultiColorImage &mc)
{
    if (m_tmpImage==nullptr)
        m_tmpImage = new QImage(320,200,QImage::Format_ARGB32);


    mc.ToQImage(m_work.m_colorList, m_tmpImage);
    QPixmap p;
    p.convertFromImage(*m_tmpImage);
    ui->lblImage->setPixmap(p);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    m_currentButton = 1;
    m_buttonDown = 1;
    m_work.m_editor.AddUndo();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    m_currentButton = 0;
    m_buttonDown = 0;

}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    float f = event->delta()/100.0f;
    m_toolBox.m_current->m_size +=f;
    m_toolBox.m_current->m_size = max(m_toolBox.m_current->m_size,1.0f);
    m_toolBox.m_current->m_size = min(m_toolBox.m_current->m_size,20.0f);

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key()==Qt::Key_Z && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        m_work.m_editor.Undo();
    }
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

        UpdateMousePosition();
        UpdateDrawing();

        if (Data::data.redrawInput) {
            m_work.Convert();
            UpdateOutput();
            Data::data.redrawInput = false;
        }
        if (Data::data.redrawOutput) {
            MultiColorImage* img = &m_work.m_editor.m_image;
            if (m_currentButton == 0) {
                img = &m_work.m_editor.m_temp;

            }

            UpdateImage(*img);
            Data::data.redrawOutput = false;

        }

        QThread::msleep(10);
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
