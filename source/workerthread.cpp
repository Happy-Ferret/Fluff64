#include "workerthread.h"


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

void WorkerThread::UpdateDrawing()
{
    if (m_isPanning)
        return;

    m_work->m_currentImage->m_temp->CopyFrom(m_work->m_currentImage->m_image);

    QPoint pos = (m_currentPos-m_zoomCenter)*m_zoom + m_zoomCenter ;


    if (pos.x()>=0 && pos.x()<m_work->m_currentImage->m_image->m_width &&
       pos.y()>=0 && pos.y()<m_work->m_currentImage->m_image->m_height) {

        LImage* img = (LImage*)m_work->m_currentImage->m_image;
        isPreview = false;

        //qDebug() << m_currentButton;
        if (m_currentButton == 0) {
            isPreview = true;
        }
        m_toolBox->m_current->IsPreview(m_currentButton, isPreview);
        unsigned int col = Data::data.currentColor;
        if (m_currentButton == 2) // draw background
            col = img->m_background;

        if (isPreview)
            img = (LImage*)m_work->m_currentImage->m_temp;

        m_toolBox->m_current->Perform(pos.x(), pos.y(), col, img, isPreview, m_currentButton);

        Data::data.Redraw();
    }
    else {
        Data::data.Redraw();
        m_currentButton = 0;
    }
    if (m_currentButton == -1)
        m_currentButton = 0;

}

void WorkerThread::UpdateMousePosition()
{
    QPointF pos = QCursor::pos() - ui->lblImage->mapToGlobal(ui->lblImage->rect().topLeft());
    pos.setX(pos.x()/(float)ui->lblImage->width()*m_work->m_currentImage->m_image->m_width);
    pos.setY(pos.y()/(float)ui->lblImage->height()*m_work->m_currentImage->m_image->m_height);
    m_prevPos = m_currentPos;
    m_currentPos = QPoint(pos.x(), pos.y());
    //    qDebug() << QApplication()::mouseButtons();
}

void WorkerThread::UpdatePanning()
{

    m_isPanning = false;
    if (m_currentButton == 2 && (QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
        QPoint delta = (m_prevPos - m_currentPos);
        m_zoomCenter+=(QPoint)delta*0.5;
//        qDebug() << delta;
        m_isPanning = true;
        Data::data.redrawOutput = true;
    }


}




void WorkerThread::UpdateImage(LImage *mc)
{
    if (m_tmpImage == nullptr)
        m_tmpImage = new QImage(320,200,QImage::Format_ARGB32);

   //m_tmpImage->fill(QColor(255,0,0,255));

    mc->ToQImage(m_work->m_colorList, m_tmpImage, m_zoom, m_zoomCenter);
    m_pixMapImage.convertFromImage(*m_tmpImage);
    emit updateImageSignal();
    //ui->lblImage->setPixmap(m_pixMapImage);
}

void WorkerThread::OnQuit()
{
    m_quit = true;
}

void WorkerThread::run()
{
    while (!m_quit) {

        UpdateMousePosition();
        UpdatePanning();
        UpdateDrawing();

        if (Data::data.redrawInput) {
            m_work->Convert();
            UpdateOutput();
            Data::data.redrawInput = false;
        }
        if (Data::data.redrawOutput) {
            LImage* img = m_work->m_currentImage->m_image;
            if (isPreview) {
                img = m_work->m_currentImage->m_temp;

            }


            UpdateImage(img);
            Data::data.redrawOutput = false;

        }
        int newShift = 0;
        if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
            newShift = 1;

        if (newShift!=m_toolBox->m_current->m_type) {
            m_toolBox->m_current->m_type = newShift;
            Data::data.redrawOutput = true;

        }



        QThread::msleep(2);
    }

}
