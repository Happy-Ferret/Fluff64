#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QString>
#include <QVector>
#include "source/limage/limagefactory.h"


class ImageType {
public:
    QString name;
    LImageFactory::Type type;
    ImageType() {

    }

    ImageType(QString n,LImageFactory::Type t) {
        type = t;
        name = n;
    }
};



class ImageEdit {
public:
    ImageEdit(ImageType* t, QString name);


    ImageType* m_imageType;
    QString m_name;
    LColorList* m_colorList = nullptr;
    void Initialize();

    LImage* m_image;
    LImage* m_temp;
    QVector<LImage*> m_undo;
    void Undo();
    void AddUndo();
    const int m_undoMax = 10;


};


#endif // IMAGEEDITOR_H
