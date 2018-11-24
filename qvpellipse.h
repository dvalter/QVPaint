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

#ifndef QVPEllipse_H
#define QVPEllipse_H

#include "qvpshape.h"

class QVPEllipse : public QVPShape {
Q_OBJECT

public:
    explicit QVPEllipse(QObject *parent = nullptr);

    QVPEllipse(QObject *parent, QColor penColor);

    explicit QVPEllipse(QColor penColor);

    QVPEllipse(QObject *parent, QColor penColor, QPointF center, qreal a, qreal b, int width = 2);

    void handleMouseMoveEvent(QMouseEvent *me) override;

    void handleMousePressEvent(QMouseEvent *me) override;

    void handleMouseReleaseEvent(QMouseEvent *me) override;

    QVPRasterizedShape &getRasterized() override {
        return *m_rasterized;
    }

    QString toString() override;

    void move(QPointF vec) override;

    QList<QVPShape *> cutLine(QPointF first, QPointF last) override;

    QList<QVPShape *> cutRect(QPointF first, QPointF last) override;


    QPointF getCenter() const;

    void setCenter(const QPointF &center);

    int getA() const;

    void setA(int a);

    int getB() const;

    void setB(int b);

protected:
    void update() override;

private:
    inline void initEllipseParams();

    inline qreal sin(QPointF point);

    inline qreal cos(QPointF point);

    QPointF m_center;
    int m_a;
    int m_b;
    QPointF m_firstPoint;
    QPointF m_lastPoint;

};

void bresenham_ellipse(QVector<QPoint> &ellipse, int x, int y, int a, int b);

#endif // QVPEllipse_H
