#include "imageworker.h"
#include <QDebug>
#include <QStandardItemModel>

ImageWorker::ImageWorker()
{

    m_types.append(ImageType("Fake C64 png Palette 1", LImage::Type::QImageBitmap, LColorList::Type::C64));
    m_types.append(ImageType("Fake C64 png Palette 2", LImage::Type::QImageBitmap, LColorList::Type::C64_ORG));
    m_types.append(ImageType("C64 MultiColor bitmap", LImage::Type::MultiColorBitmap,LColorList::Type::C64));
    m_types.append(ImageType("C64 Hires Bitmap", LImage::Type::HiresBitmap,LColorList::Type::C64));
    m_types.append(ImageType("CGA Palette 1 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA1_LOW));
    m_types.append(ImageType("CGA Palette 1 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA1_HIGH));
    m_types.append(ImageType("CGA Palette 2 Lo", LImage::Type::QImageBitmap,LColorList::Type::CGA2_LOW));
    m_types.append(ImageType("CGA Palette 2 Hi", LImage::Type::QImageBitmap,LColorList::Type::CGA2_HIGH));


    New(0);
}

ImageWorker::~ImageWorker()
{
    for (ImageEdit* ie: m_images)
        delete ie;
    m_images.clear();
}

void ImageWorker::UpdateListView(QListView *lst)
{
    QStandardItemModel *model = new QStandardItemModel( m_images.count(), 1, nullptr);
    for (int i=0;i<m_images.count();i++) {
        QStandardItem *item = new QStandardItem(m_images[i]->m_name);
        model->setItem(i,0, item);
    }

    lst->setModel(model);

}

QStringList ImageWorker::getImageTypes()
{
    QStringList l;
    for (ImageType it : m_types)
        l<< it.name;
    return l;
}

void ImageWorker::New(int type)
{

    m_currentImage = new ImageEdit(&m_types[type], "New Image");
    m_images.append(m_currentImage);
    Data::data.redrawFileList = true;
}

void ImageWorker::SetImage(int cur)
{
    if (cur>=0 && cur<m_images.count())
        m_currentImage = m_images[cur];

}



