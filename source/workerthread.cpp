#include "workerthread.h"
#include "source/util/util.h"

void WorkerThread::UpdateDrawing()
{
    if (m_work==nullptr)
        return;
    if (m_isPanning)
        return;



    if (!Data::data.forceRedraw && m_currentButton == 0)
    if ((abs(m_prevPos.x()-m_currentPos.x())<1) && (abs(m_prevPos.y()-m_currentPos.y()))<1)
        return;
    QPointF pos = (m_currentPos - QPointF(0.5, 0.5) -m_zoomCenter)*m_zoom + m_zoomCenter ;

    if ((pos.x()>=0 && pos.x()<m_work->m_currentImage->m_image->m_width &&
       pos.y()>=0 && pos.y()<m_work->m_currentImage->m_image->m_height) || Data::data.forceRedraw ) {
        m_work->m_currentImage->m_temp->CopyFrom(m_work->m_currentImage->m_image);
        Data::data.forceRedraw = false;

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

//        pos.setX(pos.x()-0.25f);
//        pos.setY(pos.y()-0.25f);

        m_toolBox->m_current->Perform(pos.x(), pos.y(), col, img, isPreview, m_currentButton);
        m_currentPosInImage = img->GetCurrentPosInImage(pos.x(), pos.y());
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
    if (m_work==nullptr)
        return;
    QPointF pos = QCursor::pos() -m_imgLabel->mapToGlobal(QPoint(0,0));
    pos.setX(pos.x()/(float)m_imgLabel->width()*m_work->m_currentImage->m_image->m_width);
    pos.setY(pos.y()/(float)m_imgLabel->height()*m_work->m_currentImage->m_image->m_height);
    m_prevPos = m_currentPos;
    m_currentPos = QPoint(pos.x(), pos.y());

}

void WorkerThread::UpdatePanning()
{
    if (!m_work)
        return;
    m_isPanning = false;
    if (m_currentButton == 2 && (QApplication::keyboardModifiers() & Qt::ShiftModifier)) {
        QPoint delta = (m_prevPos - m_currentPos);
        m_zoomCenter+=(QPoint)delta*0.5;
//        qDebug() << delta;
        m_isPanning = true;
        Data::data.Redraw();
        Data::data.forceRedraw = true;
    }


}




void WorkerThread::UpdateImage(LImage *mc)
{
   if (m_work == nullptr)
       return;
    if (m_tmpImage == nullptr) {
        m_tmpImage = new QImage(mc->m_width,mc->m_height,QImage::Format_ARGB32);
    }

    if (m_tmpImage->width()!=mc->m_width || m_tmpImage->height()!=mc->m_height) {
        delete m_tmpImage;
        m_tmpImage = new QImage(mc->m_width,mc->m_height,QImage::Format_ARGB32);

    }
   //m_tmpImage->fill(QColor(255,0,0,255));

    //qDebug() << "Updating image "<< m_work->m_currentImage;
//    qDebug() << "ToQImage";

    mc->ToQImage(m_work->m_currentImage->m_image->m_colorList, m_tmpImage, m_zoom, m_zoomCenter);
    m_pixMapImage.convertFromImage(*m_tmpImage);
    //ui->lblImageName->setText(m_work->m_currentImage->m_name  + "(" + m_work->m_currentImage->m_imageType->name + ")");


//    qDebug() << "Emit " << rand()%100;
    emit updateImageSignal();
    //ui->lblImage->setPixmap(m_pixMapImage);
}

void WorkerThread::UpdateMessages()
{

    if (Data::data.blink) {
        Data::data.blink = false;
        m_blinkTimer = 15;
        m_pal = m_orgPal;
        m_pal.setColor(QPalette::Window, QColor(90,90,70));
        emit updatePaletteSignal();
    }
    if (m_blinkTimer>0) {
        m_blinkTimer--;
        if (m_blinkTimer==0) {
            m_pal = m_orgPal;
            emit updatePaletteSignal();

        }

    }

    if (Data::data.requestSaveAs) {
        Data::data.requestSaveAs = false;
        emit requestSaveAs();
    }
    if (Data::data.requestCloseWindow) {
        Data::data.requestCloseWindow = false;
        emit requestCloseWindowSignal();
    }

}

void WorkerThread::OnQuit()
{
    m_quit = true;
}

void WorkerThread::run()
{
    while (!m_quit) {

/*        if (!ui->taMain->currentIndex()==1) {
            QThread::msleep(5);

            continue;
        }
*/
        UpdateMessages();
        UpdateMousePosition();
        UpdatePanning();
        UpdateDrawing();

        /*if (Data::data.redrawInput) {
            m_work->Convert();
            UpdateOutput();
            Data::data.redrawInput = false;
        }*/
        if (Data::data.redrawOutput && m_work!=nullptr) {

//            qDebug() << "Redraw" << rand()%100;
            LImage* img = m_work->m_currentImage->m_image;
            if (isPreview) {
                img = m_work->m_currentImage->m_temp;
            }
            UpdateImage(img);
            Data::data.redrawOutput = false;

        }
        int newShift = 0;
//        if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
//            newShift = 1;

//        qDebug() << (QApplication::keyboardModifiers() & Qt::ShiftModifier) << " + "  << rand()%100;




        QThread::msleep(5);
    }

}
