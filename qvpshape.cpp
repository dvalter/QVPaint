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


int QVPShape::testPoint(QPoint point)
{
    int min = 72;
    for (QPoint shapePoint : *m_shapePoints){
        int distX = abs(shapePoint.x() - point.x()) < 6;
        int distY = abs(shapePoint.y() - point.y()) < 6;
        if (distX < 6 && distY < 6){
            int square =  distX*distX + distY*distY;
            min = min > square ? min : square;
        }
    }
    return min;
}
