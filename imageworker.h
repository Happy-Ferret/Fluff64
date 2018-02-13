#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QVector>

#include "source/limage/limageqimage.h"
#include "source/lcolorlist.h"
#include "source/limage/multicolorimage.h"
#include "source/limage/limagefactory.h"
#include "imageeditor.h"
#include <QListView>

class ImageConverter {
public:
    LColorList* m_colorList = nullptr;
    LImageFactory::Type m_imageType;

    ImageConverter(LImageFactory::Type type);

    LImageQImage m_input;
    LImageQImage m_work;
    LImageQImage m_output;
    MultiColorImage m_mcImage;

    float m_contrast = 1;
    float m_shift = 0;
    float m_hsv = 0.5;
    float m_saturation = 0.5;
    float m_blur;



    void Load(QString filename);
    void Convert();
    void Blur();


};



class ImageWorker
{
public:
    ImageWorker();
    ~ImageWorker();
    LImageFactory::Type imageType = LImageFactory::MultiColorBitmap;

    ImageConverter m_converter = ImageConverter(imageType);
    //ImageEdit m_editor = ImageEdit(imageType);

    LColorList m_colorList;

    ImageEdit* m_currentImage;// = ImageEdit(LImageFactory::HiresBitmap);
    QVector<ImageEdit*> m_images;
    QVector<ImageType> m_types;
    void UpdateListView(QListView* lst);

    void New(int type);
    void SetImage(int cur);

    void Import(QString filename);
    void Load(QString filename);

    void Blur();
    void Convert();


};

#endif // IMAGEWORKER_H
