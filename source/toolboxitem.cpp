#include "toolboxitem.h"

ToolboxItem::ToolboxItem()
{

}

void ToolboxItem::setSize(float f)
{
    m_size = f;
}

void ToolboxItem::setRadius(float f)
{
    m_radius = f;
}

ToolboxItem::ToolboxItem(QString name, QString imagefile)
{
    m_name= name;
    m_imageFilename = m_imageFilename;

}

void ShapeBox::Perform(int x, int y, unsigned int color, MultiColorImage &img)
{
    for (int i=0;i<m_size;i++)
        for (int j=0;j<m_size;j++) {
            int d = m_size/2;
            img.setPixel(x+i-d,y+j-d,color);
        }
}

