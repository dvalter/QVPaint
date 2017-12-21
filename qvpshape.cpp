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

void QVPShape::select(bool state)
{
    qDebug() << this << state;
    if (m_selected != state){
        m_selected = state;
        update();
    }
}
