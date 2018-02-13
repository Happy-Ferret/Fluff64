#ifndef LIMAGEQIMAGE_H
#define LIMAGEQIMAGE_H
#include "source/limage/limage.h"
#include <QImage>


class LImageQImage : public LImage {
public:
    LImageQImage();
    QImage* m_qImage = nullptr;


    void Initialize(int width, int height) override;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void Save(QString filename) override;
    bool Load(QString filename) override;


    void ToQImage(LColorList& lst, QImage* img, float zoom, QPoint center) override;


    void Release() override;

    void ApplyToLabel(QLabel* l) override;
    void Clear() override {
        if (m_qImage)
            m_qImage->fill(QColor(0,0,0,255));
    }



    // Specific stuff

    QImage* Resize(int x, int y, LColorList& lst, float contrast, float shift, float hsv, float sat);
    QImage* Blur(float rad);
    QImage* ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect);

    void CreateGrid(int x, int y, QColor color, int strip,float zoom, QPoint center);

};

#endif // LIMAGEQIMAGE_H