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

#ifndef QVPRASTERIZEDSHAPE_H
#define QVPRASTERIZEDSHAPE_H

#include <QtWidgets>

class QVPShape;

class QVPRasterizedShape : public QObject {
Q_OBJECT
public:
    QVPRasterizedShape();

    QVPRasterizedShape(QVector<QPoint> *points, QColor color, int width, QVector<QPoint> *keyPoints = nullptr);

    void operator()(QImage *image) const;

private:
    QVector<QPoint> *m_points;
    QPen m_pen;
    QVector<QPoint> *m_keyPoints;
    QPen m_keyPen;

    friend class QVPShape;
};


#endif // QVPRASTERIZEDSHAPE_H
