/*
 * Copyright (C) 2017-2018 Dmitry Valter
 *
 * QVPaint is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License, version 3
 *  or the GNU Lesser General Public License, version 3 as published
 *  by the Free Software Foundation or either under the terms of the
 *  Do What the Fuck You Want to Public License, Version 2 with
 *  The No Warranty Exception 1.0.
 *
 *  QVPaint is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License,  for more details.
 *
 *  You should have received a copy of the GNU General Public License,
 *  GNU Lesser General Public License and Do What the Fuck You
 *  Want to Public License along with QVPaint. If not, see
 *  <https://www.gnu.org/licenses/> and <http://www.wtfpl.net>.
 */

#include <qvprasterizedshape.h>


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
