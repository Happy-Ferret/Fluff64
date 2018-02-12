#include "source/limage/multicolorimage.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

MultiColorImage::MultiColorImage()
{
    m_width = 160;
    m_height = 200;
    m_scaleX = 2.5f;
    m_fileExtension = "lmc";
    Clear();
}

void MultiColorImage::setPixel(int x, int y, unsigned int color)
{

    if (x>=m_width || x<0 || y>=m_height || y<0)
        return;
    PixelChar& pc = getPixelChar(x,y);

    pc.Reorganize();

    int ix = x % 4;//- (dx*40);
    int iy = y % 8;//- (dy*25);

    //if (ix==0 || ix == 2 || ix == 4 || ix == 6)
    pc.set(2*ix, iy, color);

}

unsigned int MultiColorImage::getPixel(int x, int y)
{

    if (x>=m_width || x<0 || y>=m_height || y<0)
        return 0;
    PixelChar& pc = getPixelChar(x,y);

    int ix = x % 4;//- (dx*40);
    int iy = y % 8;//- (dy*25);

    return pc.get(2*ix, iy);

}

void MultiColorImage::setForeground(unsigned int col)
{
    m_border = col;
}

void MultiColorImage::setBackground(unsigned int col)
{
    m_background = col;
    for (int i=0;i<40*25;i++) {
        m_data[i].c[0] = col;
    }
}

void MultiColorImage::Reorganize()
{
    for (int i=0;i<40*25;i++)
        m_data[i].Reorganize();
}

void MultiColorImage::Save(QString filename)
{
    QString headerID = m_ID;// + m_version;
    QByteArray array = headerID.toLocal8Bit();
    char* header = array.data();

    QFile file( filename);
    if( file.open( QFile::WriteOnly ) ) {
        file.write( ( char * )( header ), 3 );
        file.write( ( char * )( &m_version ),1 );
        file.write( ( char * )( &m_background ),  1 );
        file.write( ( char * )( &m_border ), 1 );
        file.write( ( char * )( &m_data ),  25*40*12 );

    }
    file.close();
}

bool MultiColorImage::Load(QString filename)
{
    QString headerID = m_ID;
    QFile file( filename);
    if( file.open( QFile::ReadOnly ) ) {
        char header [6];
        file.read( ( char * )( header ), 3);
        file.read( ( char * )( &m_version ),1 );
        QString head = "";
        for (int i=0;i<3;i++) {
            head +=header[i];
        }
        if (head!=headerID) {
            qDebug() << "Incorrect file type, not LMC";
            return false;
        }
        file.read( ( char * )( &m_background ),1 );
        file.read( ( char * )( &m_border ), 1);
        file.read( ( char * )( &m_data ),  25*40*12 );
    }

    return true;
}

void MultiColorImage::fromQImage(QImage *img, LColorList &lst)
{
    for (int i=0;i<160;i++)
        for (int j=0;j<200;j++) {
            unsigned char col = lst.getIndex(QColor(img->pixel(2*i, j)));
            setPixel(i,j,col);
        }
 //   Reorganize();

}

void MultiColorImage::CopyFrom(MultiColorImage &mc)
{
    m_background = mc.m_background;
    m_border = mc.m_border;
    for(int i=0;i<25*40;i++) {
      for (int j=0;j<8;j++)
        m_data[i].p[j] = mc.m_data[i].p[j];
      for (int j=0;j<4;j++)
        m_data[i].c[j] = mc.m_data[i].c[j];
    }
}

void MultiColorImage::ExportAsm(QString filename)
{
    if (QFile::exists(filename))
         QFile::remove(filename);
    QFile file(filename);
    if (!file.open(QFile::Append))
        return;
    QTextStream s(&file);


    s << "*=$0801\n";
    s << "  BYTE    $0E, $08, $0A, $00, $9E, $20, $28,  $32, $33\n";
    s << "  BYTE    $30, $34, $29, $00, $00, $00\n";
    s << "*=$0900\n";


    s << "IMG_CHARSET = $2000\n";
    s << "IMG_SCREENDATA = $4000\n";
    s << "IMG_COLORDATA = IMG_SCREENDATA + 1002\n\n";

    s << "display_image\n";

    s << "  lda IMG_SCREENDATA\n";
    s << "  sta $d020 ; background \n";
    s << "  lda IMG_SCREENDATA+1 \n";
    s << "  sta $d021; foreground \n";
    s << "  ldx #$00 \n";
    s << "loaddccimage\n";
    s << "  lda IMG_SCREENDATA+2,x\n";
    s << "  sta $0400,x \n";
    s << "  lda IMG_SCREENDATA + #$102,x \n";
    s << "  sta $0500,x \n";
    s << "  lda IMG_SCREENDATA + #$202,x \n";
    s << "  sta $0600,x \n";
    s << "  lda IMG_SCREENDATA + #$302,x \n";
    s << "  sta $0700,x \n";
    s << "\n";
    s << "; Color \n";
    s << "  lda IMG_COLORDATA,x \n";
    s << "  sta $d800,x \n";
    s << "  lda IMG_COLORDATA+ #$100,x \n";
    s << "  sta $d900,x \n";
    s << "  lda IMG_COLORDATA+ #$200,x \n";
    s << "  sta $da00,x \n";
    s << "  lda IMG_COLORDATA+ #$300,x \n";
    s << "  sta $db00,x \n\n";
    s << "  inx \n";
    s << "  bne loaddccimage \n";

    s << " ; set bitmap mode \n";
    s << "  lda #$3b \n";
    s << "  sta $d011 \n";
    s << "; Set multicolor mode \n";
    s << "  lda #$18 \n";
    s << "  sta $d016 \n";
    s << "; Set bitmap position ($400 bytes) \n";
    s << "  lda #$18 \n";
    s << "  sta $d018 \n";
    s << "loop \n";
    s << "  jmp loop \n";
    s << "  rts \n \n";
    s << "*=IMG_CHARSET \n";

    for (int y=0;y<25;y++)
        for (int x=0;x<40;x++)
        {
        s << m_data[y*40 + x].bitmapToAssembler();
    }
    s << "\n*=IMG_SCREENDATA \n";
    s << "  byte " << m_border << ", " << m_background << "\n";
    for (int y=0;y<25;y++)
    {
        QString str = "   byte ";
        for (int x=0;x<40;x++) {
           str = str +  m_data[x + y*40].colorMapToAssembler();
            if (x!=39)
                str = str + ", ";
        }
        str = str + "\n";
        s << str;
    }

    s << "\n*=IMG_COLORDATA \n";
    for (int y=0;y<25;y++)
    {
        QString str = "   byte ";
        for (int x=0;x<40;x++) {
           str = str +  m_data[x + y*40].colorToAssembler();
            if (x!=39)
                str = str + ", ";
        }
        str = str + "\n";
        s << str;
    }



    file.close();

}

void MultiColorImage::Clear()
{
    for (int i=0;i<40*25;i++) {
        m_data[i].Clear(m_background);
    }
}


PixelChar &MultiColorImage::getPixelChar(int x, int y)
{
    int dx = x/4;
    int dy = y/8;
    return m_data[dx + 40*dy];

}

PixelChar::PixelChar()
{
    Clear(0);
}

unsigned char PixelChar::get(int x, int y)
{
    if (x<0 || x>=8 || y<0 || y>=8)
        return 0;

    unsigned char pp = (p[y]>>x) & 0b11;

    return c[pp];

}

void PixelChar::set(int x, int y, unsigned char color)
{
    if (x<0 || x>=8 || y<0 || y>=8) {
        qDebug() << "Trying to set " << x << ", " << y << " out of bounds";
        return;
    }


     unsigned char winner = 254;
    // Does color exist in map?
    for (int i=0;i<4;i++) {
        if (c[i] == color) {
            winner = i;
            break;
        }
    }

    if (winner==254) {// && color!=c[0]) {

        if (c[1]==255) winner=1;
        else
        if (c[2]==255) winner=2;
        else
        if (c[3]==255) winner=3;
        else {
            //winner = 3;
            winner = (p[y]>>x) & 0b11;
            if (winner==0)
                winner = 3;

        }

        if (winner>=1)
            c[winner] = color;

    }

    // Clear
    unsigned int f = ~(0b11 << x);
    p[y] &= f;
    // Add
    p[y] |= winner<<x;

}

void PixelChar::Clear(unsigned char bg)
{
    for (int i=0;i<8;i++)
        p[i] = 0;
    for (int i=1;i<4;i++)
        c[i] = 255;
    c[0] = bg;

}

QString PixelChar::bitmapToAssembler()
{
    QString s = "   byte ";
    for (int i=0;i<8;i++) {
        s = s + QString::number(reverse(p[i]));
        if (i!=7)
            s = s+", ";
    }
    s=s+"\n";
    return s;
}

QString PixelChar::colorMapToAssembler()
{
    if (c[1]==255) c[1] = 0;
    if (c[2]==255) c[2] = 0;
    return QString(QString::number(c[1] | c[2]<<4));
}

QString PixelChar::colorToAssembler()
{
    if (c[3]==255) c[3] = 0;
    return QString(QString::number(c[3]));

}

void PixelChar::Reorganize()
{
    for (int i=1;i<4;i++) {
        unsigned int cnt = Count(i);
        if ((cnt == 0)) {
            c[i] = 255;
           // qDebug() << "REMOVING COLOR";
        }
    }
}

int PixelChar::Count(unsigned int val)
{
    int cnt=0;
    for (int i=0;i<4;i++)
        for (int j=0;j<8;j++)
            if ( ((p[j]>>2*i) & 0b11)==val)
                cnt++;
    return cnt;
}

void MultiColorImage::ToQImage(LColorList& lst, QImage* img, float zoom, QPoint center)
{
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int col = getPixel(xp,yp);
            img->setPixel(2*i,j,(lst.m_list[col].color).rgb());
            img->setPixel(2*i+1,j,lst.m_list[col].color.rgb());
        }
    //return img;
}
