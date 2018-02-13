#include "limageqimage.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include "source/util/util.h"

LImageQImage::LImageQImage()
{
    Initialize(320,200);
    m_fileExtension = "png";
}

bool LImageQImage::Load(QString filename)
{
    m_qImage = new QImage();
    m_fileExtension = "png";
    return m_qImage->load(filename);

}

void LImageQImage::Initialize(int width, int height)
{
    if (m_qImage != nullptr)
        delete m_qImage;

    m_width = width;
    m_height = height;

    m_qImage = new QImage(width, height, QImage::Format_ARGB32);
    m_fileExtension = "png";

}

void LImageQImage::Save(QString filename)
{
    if (m_qImage==nullptr)
        return;
    m_qImage->save(filename);
}

QImage* LImageQImage::ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect)
{
//    if(src.isNull()) return QImage(); //No need to do anything else!
    if (effect==nullptr)
        return &src; //No need to do anything else!

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    // QSize(extent2, extent2
    QImage* res = new QImage(src.width(), src.height(), QImage::Format_ARGB32);
    res->fill(Qt::transparent);
    QPainter ptr(res);
    scene.render(&ptr, QRectF(), QRectF( 0, 0, src.width(), src.height() ) );
    return res;
}

void LImageQImage::CreateGrid(int x, int y,  QColor color, int strip, float zoom, QPoint center)
{

    int width = m_qImage->width();
    int height = m_qImage->height();
    m_qImage->fill(QColor(0,0,0,0));
    center.setX(center.x()/320.0*width);
    center.setY(center.y()/200.0*height);
    for (int i=1;i<x;i++)
        for (int j = 0;j<height;j++) {
            float xp = (width/(x))*(i);

            xp = (xp - 2*center.x())/zoom + 2*center.x();
           float yp = (j - center.y())/zoom + center.y();


       //     if (j%strip>strip/2)
           if (xp>=0 && xp<width && yp>=0 && yp<height)
            m_qImage->setPixel(xp, yp, color.rgba());
        }

    // width lines
    for (int i=1;i<y;i++)
        for (int j = 0;j<width;j++) {
            float yp = (height/(y))*(i);

            yp = (yp - center.y())/zoom + center.y();
            float xp = (j - 2*center.x())/zoom + 2*center.x();

     //       if (j%strip>strip/2)
            if (xp>=0 && xp<width && yp>=0 && yp<height)
                m_qImage->setPixel(xp, yp, color.rgba());
        }



}

void LImageQImage::ApplyToLabel(QLabel *l)
{
    QPixmap p;
    if (m_qImage!=nullptr)
        p.convertFromImage(*m_qImage);
    l->setPixmap(p);
}

void LImageQImage::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;
    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height())
        m_qImage->setPixel(x,y,QRgb(color));
}

unsigned int LImageQImage::getPixel(int x, int y)
{
    if (m_qImage==nullptr)
        return 0;
    if (x>=0 && x<m_qImage->width() && y>=0 && y<m_qImage->height())
        return m_qImage->pixel(x,y);

}

void LImageQImage::Release()
{
    if (m_qImage)
        delete m_qImage;
    m_qImage = nullptr;
}



QImage* LImageQImage::Resize(int x, int y, LColorList& lst, float gamma, float shift, float hsvShift, float sat)
{
    QImage* other = new QImage(x,y,QImage::Format_ARGB32);
    float aspect = m_qImage->width()/(float)m_qImage->height();
    float m = max(m_qImage->width(), m_qImage->height());
    float sx = (float)x/m/2.0;
    float sy = (float)y/m;//*aspect;
    hsvShift-=0.5;
    sat-=0.5;
    int addy=0;// -100/sx/2;
    int addx = 0;
    if (m_qImage->width()>m_qImage->height()) {
        int d = m_qImage->width()-m_qImage->height();
        addy=-d/2;
    }
    if (m_qImage->width()<m_qImage->height()) {
        int d = m_qImage->height()-m_qImage->width();
        addx=-d/2;
    }

    float aspectX = 1;
    float aspectY = 1.4*0.5;//1/aspect;
    if (aspect<1) {
        aspectX = 1*(1.4);
        aspectY = 1;
    }
    QColor black(0,0,0);
    for (int i=0;i<x/2;i++)
        for (int j=0;j<y;j++) {
            QColor color = black;


            int xx = ((i-x/4)*aspectX + x/4)/sx + addx;
            int yy = ((j-y/2)*aspectY + y/2)/sy + addy;
//            int xx = ((i-x/4)*1.4 + x/4)/sx + addx;
//            int yy = j/sy + addy;

            if (xx>=0 && xx<m_qImage->width() && yy>=0 && yy<m_qImage->height())
                color = QColor(m_qImage->pixel(xx,yy));
            QColor org = color;
            //color = color.toHsv();


            QColor r = color.toHsl();
            float hsv = Util::clamp(r.hslHue()+(hsvShift*360),0,359);
            float saturation = Util::clamp(r.hslSaturation()+(sat*255),0,255);

            color = QColor::fromHsl(hsv, saturation, r.lightness());
            QVector3D v = Util::fromColor(color);

            v.setX( pow(color.red() + shift, gamma));
            v.setY( pow(color.green() + shift, gamma));
            v.setZ( pow(color.blue()+ shift, gamma));

            v = Util::clamp(v,0,255);
            QColor newCol = lst.getClosestColor(Util::toColor(v));
 //           QColor newCol = lst.getClosestColor(org);

            other->setPixel(2*i,j,newCol.rgb());
            other->setPixel(2*i+1,j,newCol.rgb());

        }


    return other;
}

QImage *LImageQImage::Blur(float blurRadius)
{
    if (blurRadius==0) {
        QImage* q = new QImage();
        *q = m_qImage->copy();
        return q;
    }
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect();
    blur->setBlurRadius((int)((m_qImage->width())*blurRadius/10.0));
    return ApplyEffectToImage(*m_qImage, blur);

}

void LImageQImage::ToQImage(LColorList& lst, QImage* img, float zoom, QPoint center)
{
#pragma omp parallel for

    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int col = getPixel(xp,yp) % 16;

//            img->setPixel(i,j,QRgb(col));
            img->setPixel(i,j,lst.m_list[col%16].color.rgb());
        }
    //return img;
}
