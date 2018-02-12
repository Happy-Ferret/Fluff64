#ifndef LIMAGE_H
#define LIMAGE_H

#include <QString>
#include <QImage>
#include "source/lcolorlist.h"
#include <QVector3D>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <QPoint>
#include <QDebug>

class LImage
{
public:
    LImage();
    ~LImage() {
        Release();
    }

    QString m_fileExtension="nonono";

    int m_width;
    int m_height;
    float m_scaleX = 0.6f;
    unsigned int m_border=0, m_background=0;

    virtual void Initialize(int width, int height) = 0;

    virtual void setPixel(int x, int y, unsigned int color) = 0;
    virtual unsigned int getPixel(int x, int y) = 0;

    virtual void Save(QString filename) = 0;
    virtual bool Load(QString filename) = 0;

    virtual void Release() {}

    virtual void ApplyToLabel(QLabel* l) = 0;

    virtual void setForeground(unsigned int col) { m_border = col; }
    virtual void setBackground(unsigned int col) {m_background = col; }

    void drawLine(float x0, float y0, float x1, float y1, unsigned int col, int size);

    void Box(int x, int y, unsigned char col, int size);

    virtual void ToQImage(LColorList& lst, QImage* img, float zoom = 1, QPoint center = QPoint(160,100)) = 0;

    void CopyFrom(LImage* img) {
        for (int i=0;i<m_width;i++)
            for (int j=0;j<m_height;j++)
                setPixel(i,j,img->getPixel(i,j));
    }

    void CopyTo(LImage* img);

    virtual void Clear() = 0;

    virtual void ExportAsm(QString filename)  { qDebug() << "ASM Write not supported"; }

};


#endif // LIMAGE_H
