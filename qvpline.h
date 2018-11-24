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

#ifndef QVPLINE_H
#define QVPLINE_H

#include "qvpshape.h"

class QVPLine : public QVPShape {
Q_OBJECT

public:
    explicit QVPLine(QObject *parent = nullptr, bool rectangle = false);

    QVPLine(QObject *parent, QColor penColor, bool rectangle = false);

    explicit QVPLine(QColor penColor, bool rectangle = false);

    QVPLine(QObject *parent, QColor penColor, QPointF first, QPointF last, int width = 2);

    void handleMouseMoveEvent(QMouseEvent *me) override;

    void handleMousePressEvent(QMouseEvent *me) override;

    void handleMouseReleaseEvent(QMouseEvent *me) override;

    QVPRasterizedShape &getRasterized() override;

    QString toString() override;

    void move(QPointF vec) override;

    QList<QVPShape *> cutLine(QPointF first, QPointF last) override;

    QPointF getFirst() const;

    QPointF getLast() const;

    void setFirst(const QPointF &firstPoint);

    void setLast(const QPointF &lastPoint);

protected:
    void update() override;

private:

    QPointF m_firstPoint;
    QPointF m_lastPoint;
    bool m_rectangle = false;

};

void bresenham_line(QVector<QPoint> &line, int x1, int y1, int x2, int y2);

#endif // QVPLINE_H
