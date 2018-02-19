#include "imageeditor.h"


ImageEdit::ImageEdit(ImageType* t, QString name)
{
    m_imageType = t;
    m_name = name;
    Initialize();
}

ImageEdit::ImageEdit(LImage* image, ImageType* it, QString name)
{
    m_imageType = it;
    m_name = name;
    m_image = image;
    m_temp = LImageFactory::Create(m_imageType->type,m_imageType->colorType);

}

void ImageEdit::Initialize()
{
    m_image = LImageFactory::Create(m_imageType->type, m_imageType->colorType);
    m_temp = LImageFactory::Create(m_imageType->type,m_imageType->colorType);
}

void ImageEdit::Undo()
{
    if (m_undo.count()<1)
        return;
    m_image->CopyFrom(m_undo[m_undo.count()-1]);
    delete m_undo[m_undo.count()-1];
    m_undo.remove(m_undo.count()-1);
    Data::data.redrawOutput = true;
}

void ImageEdit::AddUndo()
{
    m_undo.append(LImageFactory::Create(m_imageType->type, m_imageType->colorType));
    m_undo[m_undo.count()-1]->CopyFrom(m_image);
    if (m_undo.count()>m_undoMax) {
        delete m_undo[0];
        m_undo.remove(0);
    }

}
