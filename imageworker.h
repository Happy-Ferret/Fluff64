#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QVector>

#include "source/limage/limageqimage.h"
#include "source/lcolorlist.h"
#include "source/limage/multicolorimage.h"
#include "source/limage/limagefactory.h"


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


class ImageEditor {
public:
    ImageEditor(LImageFactory::Type t);
    LImageFactory::Type m_imageType;
    LColorList* m_colorList = nullptr;
    void Initialize();

    LImage* m_image;
    LImage* m_temp;
    QVector<LImage*> m_undo;
    void Undo();
    void AddUndo();
    const int m_undoMax = 10;


};


class ImageWorker
{
public:
    ImageWorker();
    ImageConverter m_converter = ImageConverter(LImageFactory::Type::QImage);
    ImageEditor m_editor = ImageEditor(LImageFactory::Type::QImage);

    LColorList m_colorList;

    void Import(QString filename);
    void Load(QString filename);

    void Blur();
    void Convert();


};

#endif // IMAGEWORKER_H
