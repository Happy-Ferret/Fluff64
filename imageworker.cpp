#include "imageworker.h"
#include <QDebug>
#include <QStandardItemModel>

ImageWorker::ImageWorker()
{
    m_colorList.InitC64();
    m_converter.m_colorList = &m_colorList;

    m_types.append(ImageType("Fake C64 png", LImageFactory::Type::QImage));
    m_types.append(ImageType("C64 MultiColor bitmap", LImageFactory::Type::MultiColorBitmap));
    m_types.append(ImageType("C64 Hires Bitmap", LImageFactory::Type::HiresBitmap));


    New(1);
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

void ImageWorker::New(int type)
{
    m_currentImage = new ImageEdit(&m_types[type], "Type " + QString::number(type));
    m_images.append(m_currentImage);
    Data::data.redrawFileList = true;
}

void ImageWorker::SetImage(int cur)
{
    if (cur>=0 && cur<m_images.count())
        m_currentImage = m_images[cur];
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

ImageConverter::ImageConverter(LImageFactory::Type t)
{
    m_imageType = t;
}


void ImageConverter::Convert()
{
    m_output.Release();
    m_output.m_qImage = m_work.Resize(320, 200, *m_colorList, m_contrast, m_shift, m_hsv, m_saturation);
    m_mcImage.Clear();
    m_mcImage.fromQImage(m_output.m_qImage, *m_colorList);

    if (m_output.m_qImage==nullptr)
         m_output.m_qImage = new QImage(320,200, QImage::Format_ARGB32);

    m_mcImage.ToQImage(*m_colorList,m_output.m_qImage,1, QPoint(0,0));
//    m_mcImage.ToQImage();

}

void ImageConverter::Blur()
{
    m_work.Release();
    qDebug() << m_input.m_qImage->width();
    m_work.m_qImage = m_input.Blur(m_blur);

}

