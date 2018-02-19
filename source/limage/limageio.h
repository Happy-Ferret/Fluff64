#ifndef LIMAGEIO_H
#define LIMAGEIO_H

#include "source/limage/limagefactory.h"
#include "source/limage/limage.h"
#include "source/lcolorlist.h"

class LImageIO
{
public:
    LImageIO();

    static QString m_fileExtension;
    static QString m_ID;

    static bool Save(QString filename, LImage* img);
    static LImage* Load(QString filename);


};

#endif // LIMAGEIO_H
