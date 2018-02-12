#ifndef LIMAGEFACTORY_H
#define LIMAGEFACTORY_H

#include "source/limage/limageqimage.h"
#include "source/limage/multicolorimage.h"

class LImageFactory {
public:
    enum Type { QImage, MultiColor };

    static LImage* Create(Type t) {
        if (t == Type::QImage)
            return new LImageQImage();
        if (t == Type::MultiColor)
            return new MultiColorImage();

        qDebug() << "ERROR: LImageFactory could not find type " << t;
        return nullptr;
    }



};




#endif // LIMAGEFACTORY_H
