#ifndef TOOLBOXITEM_H
#define TOOLBOXITEM_H

#include "source/multicolorimage.h"
#include <QPixmap>
#include <QString>


class ToolboxItem
{
public:
    ToolboxItem();
    ToolboxItem(QString name, QString imagefile);

    float m_size = 4;
    float m_type = 0;

    virtual void Perform(int x, int y, unsigned char color, MultiColorImage* img) = 0;

    void setSize(float f);
    void setRadius(float f);

    QString m_name;
    QString m_imageFilename;

    QIcon m_ButtonIcon;
};


class ShapeBox : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, MultiColorImage *img) override;

    ShapeBox();
    ShapeBox(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Circle : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, MultiColorImage *img) override;

    Circle() {}
    Circle(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Spray : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, MultiColorImage *img) override;

    Spray() {}
    Spray(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Dither : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, MultiColorImage *img) override;

    Dither() {}
    Dither(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};


#endif // TOOLBOXITEM_H
