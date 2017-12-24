#include "qvpshape.h"

QVPShape::QVPShape(QObject *parent, QColor penColor) :
    QObject(parent),
    m_penColor(penColor)/*,
    m_image(new QImage(QVP::imageWigth, QVP::imageHeight, QImage::Format_ARGB32_Premultiplied))*/
{

}

const QImage& QVPShape::getImage()
{
//    return *m_image;
}

void QVPShape::select(bool state)
{
    if (m_selected != state){
        m_selected = state;
        update();
        qDebug() << this << state;
    }
}


int QVPShape::testPoint(QPoint point)
{
    int min = 2 * QVP::searchWidth * QVP::searchWidth;
    for (QPoint shapePoint : *m_shapePoints){
        int distX = abs(shapePoint.x() - point.x());
        int distY = abs(shapePoint.y() - point.y());
        if (distX < QVP::searchWidth && distY < QVP::searchWidth){
            int square =  distX*distX + distY*distY;
            min = min < square ? min : square;
        }
    }
    return min;
}
