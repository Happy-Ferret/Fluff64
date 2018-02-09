#include "imageworker.h"
#include <QDebug>

ImageWorker::ImageWorker()
{
    m_colorList.InitC64();
    m_editor.m_colorList = &m_colorList;
    m_converter.m_colorList = &m_colorList;
}

void ImageWorker::Import(QString filename)
{
    m_converter.m_input.Load(filename);
}

void ImageWorker::Load(QString filename)
{

}

void ImageWorker::Convert()
{
    m_converter.Convert();
}

void ImageWorker::Blur()
{
    m_converter.Blur();
}

ImageConverter::ImageConverter()
{

}

void ImageConverter::Convert()
{
    m_output.Release();
    m_output.m_qImage = m_work.Resize(320, 200, *m_colorList, m_contrast, m_shift, m_hsv, m_saturation);
    m_mcImage.Clear();
    m_mcImage.fromQImage(m_output.m_qImage, *m_colorList);

    if (m_output.m_qImage==nullptr)
         m_output.m_qImage = new QImage(320,200, QImage::Format_ARGB32);

    m_mcImage.ToQImage(*m_colorList,m_output.m_qImage);

}

void ImageConverter::Blur()
{
    m_work.Release();
    qDebug() << m_input.m_qImage->width();
    m_work.m_qImage = m_input.Blur(m_blur);

}

ImageEditor::ImageEditor()
{

}

void ImageEditor::Undo()
{
    if (m_undo.count()<1)
        return;
    m_image.CopyFrom(m_undo[m_undo.count()-1]);
    m_undo.remove(m_undo.count()-1);
    Data::data.redrawOutput = true;
}

void ImageEditor::AddUndo()
{
    m_undo.append(MultiColorImage());
    m_undo[m_undo.count()-1].CopyFrom(m_image);
    if (m_undo.count()>m_undoMax)
        m_undo.remove(0);

}
