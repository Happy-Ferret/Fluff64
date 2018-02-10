#ifndef LIMAGE_H
#define LIMAGE_H

#include <QString>
#include <QImage>
#include "source/lcolorlist.h"
#include <QVector3D>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QPoint>
class LImage
{
public:
    LImage();
    ~LImage() {
        Release();
    }

    QImage* m_qImage = nullptr;

    float* m_imageData = nullptr;

    void Load(QString filename);
    void Initialize(int width, int height);
    QImage* Resize(int x, int y, LColorList& lst, float contrast, float shift, float hsv, float sat);
    QImage* Blur(float rad);

    QImage* ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect);
    void CreateGrid(int x, int y, QColor color, int strip,float zoom, QPoint center);
    void ApplyToLabel(QLabel* l);


    void Release();
};

#endif // LIMAGE_H
