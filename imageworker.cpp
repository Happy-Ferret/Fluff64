#include "imageworker.h"
#include <QDebug>
#include <QStandardItemModel>

ImageWorker::ImageWorker()
{
    m_colorList.InitC64();

    m_types.append(ImageType("Fake C64 png", LImageFactory::Type::QImage));
    m_types.append(ImageType("C64 MultiColor bitmap", LImageFactory::Type::MultiColorBitmap));
    m_types.append(ImageType("C64 Hires Bitmap", LImageFactory::Type::HiresBitmap));


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



