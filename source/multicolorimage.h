#ifndef MULTICOLORIMAGE_H
#define MULTICOLORIMAGE_H

#include <QVector>
#include "source/lcolorlist.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

class PixelChar {
public:
    PixelChar();
    unsigned char p[8];
    unsigned char c[4];
    unsigned char get(int x, int y);
    void set(int x, int y, unsigned char color);
    void Clear(unsigned char background);
    QString bitmapToAssembler();
    QString colorMapToAssembler();
    QString colorToAssembler();
    unsigned char reverse(unsigned char b) {
       b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
       b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
       b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
       return b;
    }
};


class MultiColorImage
{
public:
    unsigned char m_version = 1;
    QString m_ID = "LMC";

    MultiColorImage();
    PixelChar m_data[40*25];
    PixelChar& getPixelChar(int x, int y);
    void Clear();

    unsigned char m_border=0, m_background=0;

    void setPixel(int x, int y, unsigned char color);
    unsigned char getPixel(int x, int y);

    void setForeground(int col);
    void setBackground(int col);

    void Save(QString filename);
    bool Load(QString filename);


    QImage* ToQImage(LColorList& lst);
    void fromQImage(QImage* img, LColorList& lst);

    void CopyFrom(MultiColorImage& mc);


    void ExportAsm(QString filename);

};

#endif // MULTICOLORIMAGE_H
