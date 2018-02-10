#include "toolboxitem.h"
#include <QDebug>

ToolboxItem::ToolboxItem()
{

}

void ToolboxItem::setSize(float f)
{
    m_size = f;
}


ToolboxItem::ToolboxItem(QString name, QString imagefile)
{
    m_name= name;
    m_imageFilename = m_imageFilename;

}

void ShapeBox::Perform(int x, int y, unsigned char color, MultiColorImage* img)
{

    float m= m_size;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*1.5 + yy*yy);
            if (l<m/2)
                img->setPixel(x+xx,y+yy,color);
        }
}


void Circle::Perform(int x, int y, unsigned char color, MultiColorImage *img)
{

}
