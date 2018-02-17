#include "toolboxitem.h"
#include <QDebug>
#include <QPixmap>
#include "source/limage/limagefactory.h"
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
    m_imageFilename = imagefile;
    if (m_imageFilename!="") {
        m_image.load(m_imageFilename);
        qDebug() << m_image.width();
        m_icon.addPixmap( QPixmap::fromImage(m_image) );
    }


}

void ShapeBox::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    float m= m_size;
    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);

            bool ok = l<m/2.5;
            if (m_type==1)
                ok = abs(l-m/3)<1;
            if (ok)
            {

                img->setPixel(x+xx,y+yy,color);
            }
        }
}


void Circle::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{

}

void Spray::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    float m= m_size;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);
            if (l<m/2.5) {
                if (rand()%100<2)
                    img->setPixel(x+xx,y+yy,color);
            }
        }

}

void Dither::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    float m= m_size;
    int shift = m_type;

    x = ((int)x/2)*2;
    y = ((int)y/2)*2;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = (i-d);
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);
            if (l<m/2.5) {
                int s = (int)(xx + (int)yy%2)%2;
                if (s == 0 ) {
                    img->setPixel(x+xx+shift,y+yy,color);
                }
            }
        }

}

void Filler::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
//    if (isPreview)
  //      return;
    unsigned char testCol = img->getPixel(x,y);
    if (color == testCol)
        return;
    Fill(x,y,color, testCol, img);
}

void Filler::Fill(int i, int j, unsigned char col, unsigned char testCol, LImage *img)
{
    if (testCol == img->getPixel(i,j)) {
        img->setPixel(i,j, col);
        if (i<img->m_width-1)
            Fill(i+1,j, col, testCol, img);
        if (i>0)
            Fill(i-1,j, col, testCol, img);
        if (j<img->m_height-1)
            Fill(i,j+1, col, testCol, img);
        if (j>0)
            Fill(i,j-1, col, testCol, img);
    }

}

void Line::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    if (button==0) {
        m_start = QPoint(x,y);
        return;
    }
    if (m_type==1) {
        if (abs(m_start.x()-x) > abs(m_start.y()-y)) {
            y = m_start.y();
        }
        else x = m_start.x();
    }

    img->drawLine(m_start.x(), m_start.y(), x,y, color, m_size);


}

void CopyStamp::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{

    if (button==1 && m_status == Status::Idle) {
        m_status = Status::Selecting;
        m_start = QPoint(x,y);
        if (m_copy==nullptr)
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);
        // New from source
        if (m_copy->m_type!=img->m_type) {
            delete m_copy;
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);

        }
        m_copy->CopyFrom(img);
    }

    unsigned int frameCol = 1;

    if (m_status == Status::Selecting && button==1) {
        m_end = QPoint(x,y);
        img->drawLine(m_start.x(), m_start.y(), m_start.x(), m_end.y(), frameCol, 1);
        img->drawLine(m_start.x(), m_start.y(), m_end.x(), m_start.y(), frameCol, 1);
        img->drawLine(m_start.x(), m_end.y(), m_end.x(), m_end.y(), frameCol, 1);
        img->drawLine(m_end.x(), m_start.y(), m_end.x(), m_end.y(), frameCol, 1);
    }

    if (m_status== Status::Stamp) {
        StampImage(x,y, img);
    }

    if (button==-1 && m_status==Status::Selecting)
        m_status = Status::Stamp;


}

void CopyStamp::StampImage(int x, int y, LImage* img)
{
    int w = abs(m_end.x()-m_start.x());
    int h = abs(m_end.y()-m_start.y());
    for (int i=0;i<w;i++)
        for (int j=0;j<h;j++) {
            unsigned int col = m_copy->getPixel(m_start.x() + i, m_start.y()+j);
            if (col!=img->m_background)
                for (int xd=0;xd<m_copy->m_scale;xd++)
                    img->setPixel(i-w/2+x + xd,j-h/2+y, col);
        }
}


