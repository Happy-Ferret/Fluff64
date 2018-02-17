#ifndef LIMAGEFACTORY_H
#define LIMAGEFACTORY_H

#include "source/limage/limageqimage.h"
#include "source/limage/multicolorimage.h"
#include "source/limage/standardcolorimage.h"

class LImageFactory {
public:

    static LImage* Create(LImage::Type t, LColorList::Type colorType) {
        if (t == LImage::Type::QImageBitmap)
            return new LImageQImage(colorType);
        if (t == LImage::Type::MultiColorBitmap)
            return new MultiColorImage(colorType);
        if (t == LImage::Type::HiresBitmap)
            return new StandardColorImage(colorType);

        qDebug() << "ERROR: LImageFactory could not find type " << t;
        return nullptr;
    }



};




#endif // LIMAGEFACTORY_H
