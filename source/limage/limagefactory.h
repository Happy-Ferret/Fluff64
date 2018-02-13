#ifndef LIMAGEFACTORY_H
#define LIMAGEFACTORY_H

#include "source/limage/limageqimage.h"
#include "source/limage/multicolorimage.h"
#include "source/limage/standardcolorimage.h"

class LImageFactory {
public:
    enum Type { QImage, MultiColorBitmap, HiresBitmap };

    static LImage* Create(Type t) {
        if (t == Type::QImage)
            return new LImageQImage();
        if (t == Type::MultiColorBitmap)
            return new MultiColorImage();
        if (t == Type::HiresBitmap)
            return new StandardColorImage();

        qDebug() << "ERROR: LImageFactory could not find type " << t;
        return nullptr;
    }



};




#endif // LIMAGEFACTORY_H
