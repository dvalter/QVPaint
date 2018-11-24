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

#ifndef QVPDOCUMENT_H
#define QVPDOCUMENT_H

#include <QtWidgets>
#include "qvpshape.h"
#include "config.h"
#include "qvpdot.h"
#include "qvpline.h"
#include "qvpellipse.h"
#include "qvpellipticarc.h"
#include "qvpshapeactions.h"


class QVPDocument : public QWidget {
Q_OBJECT
public:
    explicit QVPDocument(QWidget *parent = nullptr);

    bool saveToFile(const QString &fileName);

    bool loadFromFile(const QString &fileName);

    ~QVPDocument() override {
        for (auto shape : m_shapesList) {
            delete shape;
        }
        delete m_tmpShape;
        delete m_mainImage;
    }

public slots:

    void setEditorMode(QVP::editorMode em);

    void receiveParams(QColor color, int width, QPointF first, QPointF last,
                       qreal a, qreal b, qreal ang1, qreal ang2);

    void acceptParamsClose();

private:
    QList<QVPShape *> m_shapesList;
    QList<QVPShape *> m_selectedShapesList;
    QVPShape *m_tmpShape = nullptr;
    QImage *m_mainImage;
    QVPShapeActions *m_shapeActions = nullptr;
    QVP::editorMode m_currentMode = QVP::drawLine;

    bool previousWasFail = false;

    void unSelect();

    void parseString(QString inStr);

    void updateImage();

    void searchPixel(QPoint);


protected:
    void mousePressEvent(QMouseEvent *me) override;

    void mouseMoveEvent(QMouseEvent *me) override;

    void mouseReleaseEvent(QMouseEvent *me) override;

    void paintEvent(QPaintEvent *event) override;


signals:

    void updateCoord(QPoint coord);

    void shapeSelected(bool state);

    void switchToSelection();

    void sendMsgToUI(QString text, bool isError);

    void showSetupWindow(QWidget *window);

};

#endif // QVPDOCUMENT_H
