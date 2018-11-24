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

#ifndef QVPSHAPE_H
#define QVPSHAPE_H


#include <QObject>
#include <QtWidgets>
#include "config.h"
#include "qvprasterizedshape.h"

class QVPShape : public QObject {

Q_OBJECT

public:
    explicit QVPShape(QObject *parent = nullptr, QColor penColor = QVP::penColor, int width = 2);

    virtual void handleMousePressEvent(QMouseEvent *me) = 0;

    virtual void handleMouseMoveEvent(QMouseEvent *me) = 0;

    virtual void handleMouseReleaseEvent(QMouseEvent *me) = 0;

    virtual bool isReady() {
        return !m_mousePressed;
    }

    void select(bool state);

    QVector<QPoint> *getPoints() {
        return m_shapePoints;
    }

    int testPoint(QPoint point);

    virtual QVPRasterizedShape &getRasterized() = 0;

    virtual QString toString() = 0;

    virtual void move(QPointF vec) = 0;

    virtual QList<QVPShape *> cutLine(QPointF first, QPointF last) = 0;

    virtual QList<QVPShape *> cutRect(QPointF first, QPointF last);

    QColor getColor() const;

    int getWidth() const;

    void setColor(const QColor &penColor);

    void setWidth(int width);

    ~QVPShape() override;


    bool getSelected() const;

signals:

public slots:


protected:

    virtual void update() = 0;

    QVector<QPoint> *m_shapePoints = new QVector<QPoint>;
    QColor m_penColor;
    bool m_mousePressed = false;
    bool m_selected = false;
    QVPRasterizedShape *m_rasterized = new QVPRasterizedShape;
    int m_width = 2;


};

#endif // QVPSHAPE_H
