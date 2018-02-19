#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QVector>

#include "source/limage/limageqimage.h"
#include "source//limage/lcolorlist.h"
#include "source/limage/multicolorimage.h"
#include "source/limage/limagefactory.h"
#include "imageeditor.h"
#include "source/limage/limagefactory.h"
#include <QListView>


class ImageWorker
{
public:
    ImageWorker();
    ~ImageWorker();
    //LImageFactory::Type imageType = LImageFactory::MultiColorBitmap;

    //ImageEdit m_editor = ImageEdit(imageType);

    //LColorList m_colorList;

    ImageEdit* m_currentImage;// = ImageEdit(LImageFactory::HiresBitmap);
    QVector<ImageEdit*> m_images;
    QVector<ImageType> m_types;

    ImageType* findType(LImage::Type imageType, LColorList::Type colType);

    void UpdateListView(QListView* lst);

    QStringList getImageTypes();

    void New(int type);
    void New(LImage* image);
    void SetImage(int cur);

//    void Load(QString filename);

    void Blur();
    void Convert();


};

#endif // IMAGEWORKER_H
