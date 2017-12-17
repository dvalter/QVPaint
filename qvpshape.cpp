#include "qvpshape.h"

QVPShape::QVPShape(QObject *parent, QColor penColor) :
    QObject(parent),
    m_penColor(penColor),
    m_image(new QImage(QVP::imageWigth, QVP::imageHeight, QImage::Format_ARGB32_Premultiplied))
{

}

const QImage& QVPShape::getImage()
{
    return *m_image;
}
