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

void WorkerThread::UpdateInput()
{
    return;
    /*QPixmap p;
    p.convertFromImage(*m_work.m_input.m_qImage);
    ui->lblOne->setPixmap(p);*/
}

void WorkerThread::UpdateOutput()
{
    m_work->Convert();
    m_work->m_converter.m_output.ApplyToLabel(ui->lblTwo);
}

void MainWindow::Convert()
{

}

void WorkerThread::UpdateDrawing()
{
    m_work->m_editor.m_temp.CopyFrom(m_work->m_editor.m_image);

    QPoint pos = (m_currentPos-m_zoomCenter)*m_zoom + m_zoomCenter ;
    if (pos.x()>=0 && pos.x()<m_work->m_editor.m_image.m_width &&
       pos.y()>=0 && pos.y()<m_work->m_editor.m_image.m_height) {

        MultiColorImage* img = &m_work->m_editor.m_image;
        //qDebug() << m_currentButton;
        if (m_currentButton == 0) {
            img = &m_work->m_editor.m_temp;
        }

        m_toolBox->m_current->Perform(pos.x(), pos.y(), Data::data.currentColor, img);

        Data::data.redrawOutput =true;
    }

}

void WorkerThread::UpdateMousePosition()
{
    QPointF pos = QCursor::pos() - ui->lblImage->mapToGlobal(ui->lblImage->rect().topLeft());
    pos.setX(pos.x()/(float)ui->lblImage->width()*m_work->m_editor.m_image.m_width);
    pos.setY(pos.y()/(float)ui->lblImage->height()*m_work->m_editor.m_image.m_height);
    m_currentPos = QPoint(pos.x(), pos.y());

//    qDebug() << QApplication()::mouseButtons();
}




void WorkerThread::UpdateImage(MultiColorImage &mc)
{
    if (m_tmpImage==nullptr)
        m_tmpImage = new QImage(320,200,QImage::Format_ARGB32);

    m_tmpImage->fill(QColor(255,0,0,255));

    mc.ToQImage(m_work->m_colorList, m_tmpImage, m_zoom, m_zoomCenter);
    m_pixMapImage.convertFromImage(*m_tmpImage);
    emit updateImageSignal();
    //ui->lblImage->setPixmap(m_pixMapImage);
}

void WorkerThread::OnQuit()
{
    m_quit = true;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    m_updateThread->m_currentButton = 1;

    m_work.m_editor.AddUndo();

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    m_updateThread->m_currentButton = 0;

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
        m_toolBox.m_current->m_size = min(m_toolBox.m_current->m_size,30.0f);
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

void WorkerThread::run()
{
    while (!m_quit) {

        UpdateMousePosition();
        UpdateDrawing();

        if (Data::data.redrawInput) {
            m_work->Convert();
            UpdateOutput();
            Data::data.redrawInput = false;
        }
        if (Data::data.redrawOutput) {
            MultiColorImage* img = &m_work->m_editor.m_image;
            if (m_currentButton == 0) {
                img = &m_work->m_editor.m_temp;

            }


            UpdateImage(*img);
            Data::data.redrawOutput = false;

        }
        int newShift = 0;
        if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
            newShift = 1;

        if (newShift!=m_toolBox->m_current->m_type) {
            m_toolBox->m_current->m_type = newShift;
            Data::data.redrawOutput = true;

        }



        QThread::msleep(5);
    }

}
