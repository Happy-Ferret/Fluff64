#ifndef IMAGEWORKER_H
#define IMAGEWORKER_H

#include <QVector>

#include "source/limage.h"
#include "source/lcolorlist.h"
#include "source/multicolorimage.h"

class ImageConverter {
public:
    LColorList* m_colorList = nullptr;

    ImageConverter();

    LImage m_input;
    LImage m_work;
    LImage m_output;
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
    ImageEditor();
    LColorList* m_colorList = nullptr;

    MultiColorImage m_image;
    MultiColorImage m_temp;
    QVector<MultiColorImage> m_undo;
    void Undo();
    void AddUndo();
    const int m_undoMax = 10;


};


class ImageWorker
{
public:
    ImageWorker();
    ImageConverter m_converter;
    ImageEditor m_editor;

    LColorList m_colorList;

    void Import(QString filename);
    void Load(QString filename);

    void Blur();
    void Convert();


};

#endif // IMAGEWORKER_H
