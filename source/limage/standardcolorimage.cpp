#include "standardcolorimage.h"

StandardColorImage::StandardColorImage(LColorList::Type t) : MultiColorImage(t)
{
    m_width = 320;
    m_height = 200;
    m_scaleX = 1.2f;
    m_fileExtension = "lsc";
    m_bitMask = 0b1;
    m_noColors = 2;
    m_scale = 1;
    m_minCol = 0;
    Clear();
    m_type = LImage::Type::HiresBitmap;

}
