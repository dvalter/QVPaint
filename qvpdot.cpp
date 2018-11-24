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

#include "qvpdot.h"
#include <sstream>


QVPDot::QVPDot(QColor penColor) :
        QVPShape(nullptr, std::move(penColor)) {
    m_shapePoints->reserve(1);
}

QVPDot::QVPDot(QObject *parent, QColor penColor) :
        QVPShape(parent, std::move(penColor)) {
    m_shapePoints->reserve(1);
}

QVPDot::QVPDot(QObject *parent) :
        QVPShape(parent, QVP::penColor) {
    m_shapePoints->reserve(1);
}


QVPDot::QVPDot(QObject *parent, QColor penColor, QPointF point, int width) :
        QVPShape(parent, std::move(penColor), width),
        m_firstPoint(point) {
    m_shapePoints->push_back(QPoint());
    update();
}


void QVPDot::update() {
    QColor color;
    if (m_selected) {
        color = QColor(0xFF - m_penColor.red(), 0xFF - m_penColor.green(), 0xFF - m_penColor.blue(), 0xFF);
        if (color == QVP::backgroundColor) {
            color = QColor(0xFF, 0x0, 0x0, 0xFF); // shoud be red
        }
        qDebug() << color;
    } else {
        color = m_penColor;
    }


    delete m_rasterized;
    (*m_shapePoints)[0] = QPoint(int(m_firstPoint.x()), int(m_firstPoint.y()));
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, m_width);
}

QPointF QVPDot::getCenter() const {
    return m_firstPoint;
}

void QVPDot::setCenter(const QPointF &firstPoint) {
    m_firstPoint = firstPoint;
    update();
}


void QVPDot::handleMousePressEvent(QMouseEvent *me) {
    m_firstPoint = me->pos();
    if (m_shapePoints->empty()) {
        m_shapePoints->push_back(me->pos());
    }
    m_mousePressed = true;
    update();
}

void QVPDot::handleMouseMoveEvent(QMouseEvent *me) {
    m_firstPoint = me->pos();
    update();
}

void QVPDot::handleMouseReleaseEvent(QMouseEvent *me) {
    m_firstPoint = me->pos();
    m_mousePressed = false;
    update();
}

QVPRasterizedShape &QVPDot::getRasterized() {
    if (!m_rasterized)
        update();
    return *m_rasterized;
}

QString QVPDot::toString() {
    std::stringstream ss;
    ss << "D;" << m_firstPoint.x() << ";" << m_firstPoint.y() << ";" <<
       QString("%1").arg(((m_penColor.red() * 7u / 255) << 5 |
                          m_penColor.green() * 7u / 255 << 2 |
                          m_penColor.blue() * 3u / 255), 0, 8
       ).toStdString() << ";" << m_width << "\n";
    return QString::fromStdString(std::string(ss.str()));
}

void QVPDot::move(QPointF vec) {
    m_firstPoint.rx() += vec.x();
    m_firstPoint.ry() += vec.y();
    update();
}

QList<QVPShape *> QVPDot::cutLine(QPointF first, QPointF last) {
    if (first.x() != 0 || last.x() != 0)
        return QList<QVPShape *>({(QVPShape *) new QVPDot(parent(), m_penColor, m_firstPoint)});
    else
        return QList<QVPShape *>({(QVPShape *) new QVPDot(parent(), m_penColor, m_firstPoint)});
}
