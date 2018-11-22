#include "qvprasterizedshape.h"


QVPRasterizedShape::QVPRasterizedShape() :
        m_points(nullptr),
        m_keyPoints(nullptr) {

}

QVPRasterizedShape::QVPRasterizedShape(QVector<QPoint> *points, QColor color, int width, QVector<QPoint> *keyPoints) :
        m_points(points),
        m_pen(color),
        m_keyPoints(keyPoints),
        m_keyPen(QColor(0xFF, 0xFF, 0, 0xFF)) {
    m_pen.setWidth(width);
    m_keyPen.setWidth(width > 5 ? 2 * width : 5 * width);
}

void QVPRasterizedShape::operator()(QImage *image) const {
    if (!m_points)
        return;
    QPainter painter(image);
    painter.setPen(m_pen);
    painter.drawPoints(m_points->data(), m_points->size());
    if (m_keyPoints) {
        painter.setPen(m_keyPen);
        painter.drawPoints(m_keyPoints->data(), m_keyPoints->size());
    }
    painter.end();
}
