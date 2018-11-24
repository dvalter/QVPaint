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

#ifndef QVPDOT_H
#define QVPDOT_H

#include "qvpshape.h"

class QVPDot : public QVPShape {
Q_OBJECT

public:
    explicit QVPDot(QObject *parent = nullptr);

    QVPDot(QObject *parent, QColor penColor);

    explicit QVPDot(QColor penColor);

    QVPDot(QObject *parent, QColor penColor, QPointF point, int width = 2);

    void handleMouseMoveEvent(QMouseEvent *me) override;

    void handleMousePressEvent(QMouseEvent *me) override;

    void handleMouseReleaseEvent(QMouseEvent *me) override;

    QVPRasterizedShape &getRasterized() override;

    QString toString() override;

    void move(QPointF vec) override;

    QList<QVPShape *> cutLine(QPointF first, QPointF last) override;

    QPointF getCenter() {
        return m_firstPoint;
    }

    QPointF getCenter() const;

    void setCenter(const QPointF &firstPoint);

protected:
    void update() override;

private:

    QPointF m_firstPoint;

};


#endif // QVPDOT_H
