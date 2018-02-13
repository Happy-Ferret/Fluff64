#include "imageeditor.h"


ImageEdit::ImageEdit(ImageType* t, QString name)
{
    m_imageType = t;
    m_name = name;
    Initialize();
}

void ImageEdit::Initialize()
{
    m_image = LImageFactory::Create(m_imageType->type);
    m_temp = LImageFactory::Create(m_imageType->type);
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
    m_undo.append(LImageFactory::Create(m_imageType->type));
    m_undo[m_undo.count()-1]->CopyFrom(m_image);
    if (m_undo.count()>m_undoMax) {
        delete m_undo[0];
        m_undo.remove(0);
    }

}
