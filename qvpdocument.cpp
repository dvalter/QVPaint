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

#include "qvpdocument.h"

QColor colorFrom8BitStr(const QString &str);


QVPDocument::QVPDocument(QWidget *parent) :
        QWidget(parent),
        m_mainImage(new QImage(QVP::imageWigth, QVP::imageHeight, QImage::Format_ARGB32_Premultiplied))/*,
    m_imgLbl(new QLabel)*/
{
    qDebug() << __FUNCTION__;
    m_mainImage->fill(QVP::backgroundColor);
    resize(QVP::imageWigth, QVP::imageHeight);

    setMouseTracking(true);
    emit shapeSelected(false);
}


void QVPDocument::searchPixel(QPoint point) {
    int minDist = 2 * QVP::searchWidth * QVP::searchWidth;
    QVPShape *selectedShape = nullptr;
    for (auto shape : m_shapesList) {
        int dist = shape->testPoint(point);
        if (dist < minDist) {
            selectedShape = shape;
            minDist = dist;
        }
    }
    if (selectedShape && !selectedShape->getSelected()) {
        selectedShape->select(true);
        m_selectedShapesList.append(selectedShape);
        emit shapeSelected(true);
    } elif (selectedShape) {
        selectedShape->select(false);
        m_selectedShapesList.removeAll(selectedShape);
        if (m_selectedShapesList.empty())
                emit shapeSelected(false);
    }
    update();
}

void QVPDocument::mousePressEvent(QMouseEvent *me) {
//    qDebug() << __FUNCTION__ << me;
    if (me->button() == Qt::LeftButton) {

        if (m_currentMode == QVP::drawDot) {
            emit sendMsgToUI("Нарисуйте точку", false);

            m_tmpShape = new QVPDot(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawLine || m_currentMode == QVP::move ||
                m_currentMode == QVP::crossLine) {
            emit sendMsgToUI("Нарисуйте линию", false);

            m_tmpShape = new QVPLine(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawEllipse) {
            emit sendMsgToUI("Нарисуйте эллипс", false);

            m_tmpShape = new QVPEllipse(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawEllipticCurve) {
            emit sendMsgToUI("Нарисуйте эллипс и линиями ограничьте дугу", false);

            if (m_tmpShape == nullptr) {
                m_tmpShape = new QVPEllipticArc(this);
            }
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::selectShape) {
            searchPixel(me->pos());
            updateImage();
            if (!m_selectedShapesList.empty()) {
                emit sendMsgToUI("Выберите ещё фигуры", false);
            }
        } elif (m_currentMode == QVP::makeOrtho) {
            if (m_selectedShapesList.isEmpty()) {
                previousWasFail = true;
                emit sendMsgToUI("Ничего не выбрано!", true);
                emit switchToSelection();
            }
            auto *line = qobject_cast<QVPLine *>(m_selectedShapesList.last());
            if (line) {
                emit sendMsgToUI("Нарисуйте линию ожидаемой длины", false);
                m_tmpShape = new QVPLine(this);
                m_tmpShape->handleMousePressEvent(me);
                updateImage();
            } else {
                previousWasFail = true;
                emit sendMsgToUI("Выбрана не линия", true);
                emit switchToSelection();

            }
        } elif (m_currentMode == QVP::clipRectangle) {
            emit sendMsgToUI("Нарисуйте прямоугольник", false);

            m_tmpShape = new QVPLine(this, true);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        }
    }
    qDebug() << "Shapes: " << m_shapesList.size();
}


void QVPDocument::mouseMoveEvent(QMouseEvent *me) {

    emit updateCoord(me->pos());

    if (m_currentMode == QVP::drawLine || m_currentMode == QVP::drawEllipse
        || m_currentMode == QVP::drawDot || m_currentMode == QVP::move
        || m_currentMode == QVP::makeOrtho || m_currentMode == QVP::crossLine
        || m_currentMode == QVP::clipRectangle) {
        if (me->buttons() & Qt::LeftButton) {
            m_tmpShape->handleMouseMoveEvent(me);
        }
        updateImage();
    } elif (m_currentMode == QVP::drawEllipticCurve && m_tmpShape != nullptr) {
        qDebug() << "mme=" << me;
        m_tmpShape->handleMouseMoveEvent(me);
        updateImage();
    }
}

void QVPDocument::mouseReleaseEvent(QMouseEvent *me) {
    if (m_currentMode == QVP::drawLine || m_currentMode == QVP::drawEllipse
        || m_currentMode == QVP::drawDot) {
        if (me->button() == Qt::LeftButton) {
            m_tmpShape->handleMouseReleaseEvent(me);
            m_shapesList.append(m_tmpShape);
            m_tmpShape = nullptr;
        }
        updateImage();
    } elif (m_currentMode == QVP::drawEllipticCurve) {
        if (me->button() == Qt::LeftButton) {
            m_tmpShape->handleMouseReleaseEvent(me);
            if (m_tmpShape->isReady()) {
                m_shapesList.append(m_tmpShape);
                m_tmpShape = nullptr;
            }
        }
        updateImage();
    } elif (m_currentMode == QVP::move) {
        auto *linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        QPointF vec(b.x() - a.x(), b.y() - a.y());
        for (auto shape : m_selectedShapesList) {
            shape->move(vec);
        }
        delete m_tmpShape;
        m_tmpShape = nullptr;
        updateImage();
    } elif (m_currentMode == QVP::makeOrtho) {
        if (m_selectedShapesList.isEmpty()) {
            previousWasFail = true;
            emit sendMsgToUI("Ничего не выбрано!", true);
            emit switchToSelection();
        }
        auto *linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        double targetLenght = sqrt((b.x() - a.x()) * (b.x() - a.x()) +
                                   (b.y() - a.y()) * (b.y() - a.y()));
        auto *selectedLinePtr = qobject_cast<QVPLine *>(m_selectedShapesList.last());
        QPointF sa = selectedLinePtr->getFirst();
        QPointF sb = selectedLinePtr->getLast();
        double x = sb.x() - sa.x();
        double y = sb.y() - sa.y();
        double lenght = sqrt(x * x + y * y);
        double scale = targetLenght / lenght;
        b.setX(a.x() + y * scale);
        b.setY(a.y() - x * scale);
        m_shapesList.append(new QVPLine(this, QVP::penColor, a, b));
        delete m_tmpShape;
        m_tmpShape = nullptr;
        updateImage();
        emit switchToSelection();
    } elif (m_currentMode == QVP::crossLine) {
        auto *linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        QStack<QVPShape *> removeStack;
        QList<QVPShape *> addList;
        for (auto shape : m_selectedShapesList) {
            auto tmpList = shape->cutLine(a, b);
            if (!tmpList.isEmpty()) {
                removeStack.append(shape);
                addList.append(tmpList);
            }
        }
        unSelect();

        while (!removeStack.isEmpty()) {
            auto shape = removeStack.pop();
            delete shape;
            m_shapesList.removeAll(shape);
        }
        m_shapesList.append(addList);
        emit switchToSelection();
        updateImage();
    } elif (m_currentMode == QVP::clipRectangle) {
        auto *linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        QStack<QVPShape *> removeStack;
        QList<QVPShape *> addList;
        for (auto shape : m_selectedShapesList) {
            auto tmpList = shape->cutRect(a, b);
            if (!tmpList.isEmpty()) {
                removeStack.push(shape);
                addList.append(tmpList);
            }
        }
        unSelect();

        while (!removeStack.empty()) {
            auto shape = removeStack.pop();
            delete shape;
            m_shapesList.removeAll(shape);
        }
        m_shapesList.append(addList);
        emit switchToSelection();
        updateImage();
    }
}


void QVPDocument::paintEvent(QPaintEvent *event) {
    const QRect paintRect = event->rect();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);


    painter.drawImage(paintRect, *m_mainImage, paintRect);

}

void QVPDocument::setEditorMode(QVP::editorMode em) {
    qDebug() << __FUNCTION__;
    if (!previousWasFail && ((m_currentMode == QVP::selectShape &&
                              !(em == QVP::move || em == QVP::makeOrtho ||
                                em == QVP::clipRectangle || em == QVP::crossLine ||
                                em == QVP::setUp)) ||
                             (m_currentMode == QVP::move ||
                              m_currentMode == QVP::makeOrtho ||
                              m_currentMode == QVP::clipRectangle ||
                              m_currentMode == QVP::crossLine ||
                              m_currentMode == QVP::setUp))) {
        unSelect();
    }
    m_currentMode = em;

    if (m_currentMode == QVP::selectShape && !previousWasFail) {
        emit sendMsgToUI("Выберите фигуру", false);
    }
    previousWasFail = false;

    if (m_tmpShape) {
        delete m_tmpShape;
        m_tmpShape = nullptr;
    }

    updateImage();

    if (m_currentMode == QVP::setUp) {
        if (m_selectedShapesList.empty()) {
            previousWasFail = true;
            emit switchToSelection();
        } elif (!m_shapeActions) {
            QVPShape *shape = m_selectedShapesList.last();
            if (qobject_cast<QVPDot *>(shape)) {
                auto *dot = qobject_cast<QVPDot *>(shape);
                m_shapeActions = new QVPShapeActions(nullptr, QVP::point, shape->getColor(),
                                                     shape->getWidth(), dot->getCenter());

                connect(m_shapeActions, &QVPShapeActions::finished, this, &QVPDocument::acceptParamsClose);
                connect(m_shapeActions, &QVPShapeActions::updateShape, this, &QVPDocument::receiveParams);

                m_shapeActions->show();
            } elif (qobject_cast<QVPLine *>(shape)) {
                auto *line = qobject_cast<QVPLine *>(shape);
                m_shapeActions = new QVPShapeActions(nullptr, QVP::line, shape->getColor(),
                                                     shape->getWidth(), line->getFirst(), line->getLast());

                connect(m_shapeActions, &QVPShapeActions::finished, this, &QVPDocument::acceptParamsClose);
                connect(m_shapeActions, &QVPShapeActions::updateShape, this, &QVPDocument::receiveParams);

                m_shapeActions->show();
            } elif (qobject_cast<QVPEllipse *>(shape)) {
                auto *ellipse = qobject_cast<QVPEllipse *>(shape);
                m_shapeActions = new QVPShapeActions(nullptr, QVP::ellipse, shape->getColor(),
                                                     shape->getWidth(), ellipse->getCenter(),
                                                     ellipse->getA(), ellipse->getB());

                connect(m_shapeActions, &QVPShapeActions::finished, this, &QVPDocument::acceptParamsClose);
                connect(m_shapeActions, &QVPShapeActions::updateShape, this, &QVPDocument::receiveParams);

                m_shapeActions->show();
            } elif (qobject_cast<QVPEllipticArc *>(shape)) {
                auto *arc = qobject_cast<QVPEllipticArc *>(shape);
                m_shapeActions = new QVPShapeActions(nullptr, QVP::ellipticArc, shape->getColor(),
                                                     shape->getWidth(), arc->getCenter(),
                                                     arc->getA(), arc->getB(),
                                                     arc->getAng1(), arc->getAng2());

                connect(m_shapeActions, &QVPShapeActions::finished, this, &QVPDocument::acceptParamsClose);
                connect(m_shapeActions, &QVPShapeActions::updateShape, this, &QVPDocument::receiveParams);

                m_shapeActions->show();
            }
        }
    } elif (m_shapeActions) {
        m_shapeActions->close();
        m_shapeActions->deleteLater();
        m_shapeActions = nullptr;
    }


}

void QVPDocument::unSelect() {
    for (auto shape : m_selectedShapesList) {
        shape->select(false);
    }
    m_selectedShapesList.clear();
    emit shapeSelected(false);
}

void QVPDocument::updateImage() {
    m_mainImage->fill(QVP::backgroundColor);
    for (auto shape : m_shapesList) {
        shape->getRasterized()(m_mainImage);
    }
    if (m_tmpShape) {
        m_tmpShape->getRasterized()(m_mainImage);
    }

    update(m_mainImage->rect());
}

void QVPDocument::parseString(QString inStr) {
    QStringList inList = inStr.split(';');
    qDebug() << inList;

    if (inList[0] == "D") {
        QColor color(colorFrom8BitStr(inList[3]));
        QPointF point(inList[1].toDouble(), inList[2].toDouble());
        int width = inList[4].toInt();
        m_shapesList.append(new QVPDot(this, color, point, width));
    } elif (inList[0] == "L") {
        QColor color(colorFrom8BitStr(inList[5]));
        QPointF first(inList[1].toDouble(), inList[2].toDouble());
        QPointF last(inList[3].toDouble(), inList[4].toDouble());
        int width = inList[6].toInt();
        m_shapesList.append(new QVPLine(this, color, first, last, width));
    } elif (inList[0] == "E") {
        QColor color(colorFrom8BitStr(inList[5]));
        QPointF center(inList[1].toDouble(), inList[2].toDouble());
        qreal a = inList[3].toFloat();
        qreal b = inList[4].toFloat();
        int width = inList[6].toInt();
        m_shapesList.append(new QVPEllipse(this, color, center, a, b, width));
    } elif (inList[0] == "A") {
        QColor color(colorFrom8BitStr(inList[7]));
        QPointF center(inList[1].toDouble(), inList[2].toDouble());
        qreal a = inList[3].toFloat();
        qreal b = inList[4].toFloat();
        qreal ang1 = inList[5].toFloat();
        qreal ang2 = inList[6].toFloat();
        int width = inList[8].toInt();
        m_shapesList.append(new QVPEllipticArc(this, color, center, a, b, ang1, ang2, width));
    } else {
        qWarning() << "BAD LINE";
    }
}

QColor colorFrom8BitStr(const QString &str) {
    qDebug() << str;

    auto color = quint8(str.toInt(nullptr, 8));

    quint8 red = quint8(((color & 224) >> 5) * 255 / 7);
    quint8 green = quint8(((color & 28) >> 2) * 255 / 7);
    quint8 blue = quint8((color & 3) * 255 / 3);

    qDebug() << QString("%1.%2.%3").arg(red, 0, 16).arg(green, 0, 16).arg(blue, 0, 16);
    return QColor(red, green, blue, 0xFF);
}

bool QVPDocument::loadFromFile(const QString &fileName) {
    bool status;
    QFile file(fileName);
    if ((status = file.open(QIODevice::ReadOnly))) {
        QTextStream stream(&file);

        while (!m_shapesList.empty()) {
            delete m_shapesList.takeLast();
        }
        m_shapesList.clear();

        while (!stream.atEnd()) {
            parseString(stream.readLine());
        }
        updateImage();
    }
    return status;
}

bool QVPDocument::saveToFile(const QString &fileName) {
    bool status;
    QFile file(fileName);
    if ((status = file.open(QIODevice::WriteOnly))) {
        QTextStream stream(&file);
        for (QVPShape *shape : m_shapesList) {
            stream << shape->toString();
        }
        file.close();
    }
    return status;
}

void QVPDocument::acceptParamsClose() {
    qDebug() << __FUNCTION__;
    if (m_shapeActions != nullptr) {
        qDebug() << "shapeActions deleteOnClose::" << m_shapeActions->testAttribute(Qt::WA_DeleteOnClose);
        m_shapeActions->setAttribute(Qt::WA_DeleteOnClose);
        m_shapeActions->close();
        if (m_shapeActions != nullptr) {
            m_shapeActions->deleteLater();
            m_shapeActions = nullptr;
        }
    }

    emit switchToSelection();
}

void QVPDocument::receiveParams(QColor color, int width, QPointF first, QPointF last,
                                qreal a, qreal b, qreal ang1, qreal ang2) {
    QVPShape *shape = m_selectedShapesList.last();
    shape->setColor(color);
    shape->setWidth(width);

    if (qobject_cast<QVPDot *>(shape)) {
        auto *dot = qobject_cast<QVPDot *>(shape);
        dot->setCenter(first);

    } elif (qobject_cast<QVPLine *>(shape)) {
        auto *line = qobject_cast<QVPLine *>(shape);
        line->setFirst(first);
        line->setLast(last);

    } elif (qobject_cast<QVPEllipse *>(shape)) {
        auto *ellipse = qobject_cast<QVPEllipse *>(shape);
        ellipse->setCenter(first);
        ellipse->setA(int(a));
        ellipse->setB(int(b));

    } elif (qobject_cast<QVPEllipticArc *>(shape)) {
        auto *arc = qobject_cast<QVPEllipticArc *>(shape);
        arc->setCenter(first);
        arc->setA(a);
        arc->setB(b);
        arc->setAng1(ang1);
        arc->setAng2(ang2);

    }
    acceptParamsClose();
    updateImage();
}
