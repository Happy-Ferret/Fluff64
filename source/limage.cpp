#include "limage.h"
#include <algorithm>

#include "source/util/util.h"
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>


LImage::LImage()
{

}

void LImage::Load(QString filename)
{
    m_qImage = new QImage();
    m_qImage->load(filename);

}

QImage* LImage::ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect)
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

QImage* LImage::Resize(int x, int y, LColorList& lst, float gamma, float shift, float hsvShift, float sat)
{
    QImage* other = new QImage(x,y,QImage::Format_ARGB32);
    float aspect = 320.0/200.0;
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

    QColor black(0,0,0);
    for (int i=0;i<x/2;i++)
        for (int j=0;j<y;j++) {
            QColor color = black;

            int xx = ((i-x/4)*1.4 + x/4)/sx + addx;
            int yy = j/sy + addy;

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

QImage *LImage::Blur(float blurRadius)
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

void LImage::Release()
{
    if (m_qImage)
        delete m_qImage;
}
