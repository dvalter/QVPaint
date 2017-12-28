#include "qvpshape.h"

QVPShape::QVPShape(QObject *parent, QColor penColor, int width) :
    QObject(parent),
    m_penColor(penColor),
    m_width(width)
{

}

QVPShape::~QVPShape()
{
    delete m_shapePoints;
    if (m_rasterized){
        delete m_rasterized;
    }
}

bool QVPShape::getSelected() const
{
    return m_selected;
}

QColor QVPShape::getColor() const
{
    return m_penColor;
}

void QVPShape::setColor(const QColor &penColor)
{
    m_penColor = penColor;
    update();
}

void QVPShape::setWidth(int width)
{
    m_width = width;
    update();
}

int QVPShape::getWidth() const
{
    return m_width;
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

QList<QVPShape *> QVPShape::cutRect(QPointF first, QPointF last)
{
    QList<QVPShape *> result;
    QList<QVPShape *> a = cutLine(first, QPoint(last.x(), first.y()));
    for (auto shape : a){
        QList<QVPShape *> b = shape->cutLine(QPoint(last.x(), first.y()) , last);
        delete shape;
        for (auto shape : b){
            QList<QVPShape *> c = shape->cutLine(last , QPoint(first.x(), last.y()));
            delete shape;
            for (auto shape : c){
                QList<QVPShape *> d = shape->cutLine(QPoint(first.x(), last.y()) , first);
                result.append(d);
                delete shape;
            }
        }
    }
    return result;
}
