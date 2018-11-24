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

#include "qvpshape.h"

QVPShape::QVPShape(QObject *parent, QColor penColor, int width) :
        QObject(parent),
        m_penColor(std::move(penColor)),
        m_width(width) {

}

QVPShape::~QVPShape() {
    delete m_shapePoints;
    delete m_rasterized;
}

bool QVPShape::getSelected() const {
    return m_selected;
}

QColor QVPShape::getColor() const {
    return m_penColor;
}

void QVPShape::setColor(const QColor &penColor) {
    m_penColor = penColor;
    update();
}

void QVPShape::setWidth(int width) {
    m_width = width;
    update();
}

int QVPShape::getWidth() const {
    return m_width;
}

void QVPShape::select(bool state) {
    if (m_selected != state) {
        m_selected = state;
        update();
        qDebug() << this << state;
    }
}


int QVPShape::testPoint(QPoint point) {
    int min = 2 * QVP::searchWidth * QVP::searchWidth;
    for (QPoint shapePoint : *m_shapePoints) {
        int distX = abs(shapePoint.x() - point.x());
        int distY = abs(shapePoint.y() - point.y());
        if (distX < QVP::searchWidth && distY < QVP::searchWidth) {
            int square = distX * distX + distY * distY;
            min = min < square ? min : square;
        }
    }
    return min;
}

QList<QVPShape *> QVPShape::cutRect(QPointF first, QPointF last) {
    QList<QVPShape *> result;
    QList<QVPShape *> a = cutLine(first, QPoint(int(last.x()), int(first.y())));
    for (auto shape : a) {
        QList<QVPShape *> b = shape->cutLine(QPoint(int(last.x()), int(first.y())), last);
        delete shape;
        for (auto shape : b) {
            QList<QVPShape *> c = shape->cutLine(last, QPoint(int(first.x()), int(last.y())));
            delete shape;
            for (auto shape : c) {
                QList<QVPShape *> d = shape->cutLine(QPoint(int(first.x()), int(last.y())), first);
                result.append(d);
                delete shape;
            }
        }
    }
    return result;
}
