#include "limage.h"
#include <algorithm>

#include "source/util/util.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>


LImage::LImage(LColorList::Type t)
{
    m_colorList.Initialize(t);
}

unsigned char LImage::TypeToChar(LImage::Type t)
{
    if (t==QImageBitmap)
        return 0;
    if (t==MultiColorBitmap)
        return 1;
    if (t==HiresBitmap)
        return 2;

    return 255;
}

LImage::Type LImage::CharToType(unsigned char c)
{
    if (c==0)
        return QImageBitmap;
    if (c==1)
        return MultiColorBitmap;
    if (c==2)
        return HiresBitmap;


    return NotSupported;

}





void LImage::drawLine(float x0, float y0, float x1, float y1, unsigned int col, int size)
{
        float x{x1 - x0}, y{y1 - y0};
        const float max{std::max(std::fabs(x), std::fabs(y))};
        x /= max; y /= max;
        for (float n{0}; n < max; ++n)
        {
            // draw pixel at ( x0, y0 )
            Box(x0,y0, col, size);
            x0 += x; y0 += y;
        }
}

void LImage::Box(int x, int y, unsigned char col, int size)
{
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++) {
            setPixel(x+i-size/2, y+j-size/2, col);
        }
}


