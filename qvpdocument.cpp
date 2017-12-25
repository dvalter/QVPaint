#include "qvpdocument.h"
#include <QPainter>
#include <QDebug>
#include <stdlib.h>
#include <math.h>
QColor colorFrom8BitStr(QString str);



QVPDocument::QVPDocument(QWidget* parent) :
    QLabel(parent),
    m_mainImage(new QImage(QVP::imageWigth, QVP::imageHeight, QImage::Format_ARGB32_Premultiplied)),
    m_imgLbl(new QLabel)
{
    qDebug() << __FUNCTION__;
    m_mainImage->fill(QVP::backgroundColor);
    resize(QVP::imageWigth, QVP::imageHeight);

    setMouseTracking(true);
    emit shapeSelected(false);
//    m_shapesList.append(new QVPEllipticArc(this, QVP::penColor, QPointF(100, 100), 50.0, 50.0, -M_PI_2, M_PI_2));
//    update();
}


void QVPDocument::searchPixel(QPoint point)
{
    int minDist = 2 * QVP::searchWidth * QVP::searchWidth;
    QVPShape * selectedShape = nullptr;
    for (auto shape : m_shapesList){
        int dist = shape->testPoint(point);
        if (dist < minDist){
            selectedShape = shape;
            minDist = dist;
        } /*else {
            shape->select(false);
        }*/
    }
    if (selectedShape){
        selectedShape->select(true);
        m_selectedShapesList.append(selectedShape);
        emit shapeSelected(true);
    }
    update();
}

void QVPDocument::mousePressEvent(QMouseEvent* me)
{
//    qDebug() << __FUNCTION__ << me;
    if (me->button() == Qt::LeftButton){

        if (m_currentMode == QVP::drawDot){
            emit sendMsgToUI("Нарисуйте точку", false);

            m_tmpShape = new QVPDot(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawLine || m_currentMode == QVP::move ||
                m_currentMode == QVP::crossLine){
            emit sendMsgToUI("Нарисуйте линию", false);

            m_tmpShape = new QVPLine(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawEllipse){
            emit sendMsgToUI("Нарисуйте эллипс", false);

            m_tmpShape = new QVPEllipse(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawEllipticCurve){
            emit sendMsgToUI("Нарисуйте эллипс и линиями ограничьте дугу", false);

            if (m_tmpShape == nullptr){
                m_tmpShape = new QVPEllipticArc(this);
            }
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::selectShape){
            searchPixel(me->pos());
            updateImage();
            if (!m_selectedShapesList.empty()){
                emit sendMsgToUI("Выберите ещё фигуры", false);
            }
        } elif (m_currentMode == QVP::makeOrtho){
            if (m_selectedShapesList.isEmpty()){
                previousWasFail = true;
                emit sendMsgToUI("Ничего не выбрано!", true);
                emit switchToSelection();
            }
            QVPLine * line = qobject_cast<QVPLine *>(m_selectedShapesList.last());
            if (line){
                emit sendMsgToUI("Нарисуйте линию ожидаемой длины", false);
                m_tmpShape = new QVPLine(this);
                m_tmpShape->handleMousePressEvent(me);
                updateImage();
            } else {
                previousWasFail = true;
                emit sendMsgToUI("Выбрана не линия", true);
                emit switchToSelection();

            }
        }
    }
    qDebug() << "Shapes: " << m_shapesList.size();
}



void QVPDocument::mouseMoveEvent(QMouseEvent *me)
{

    emit updateCoord(me->pos());

    if (m_currentMode == QVP::drawLine || m_currentMode == QVP::drawEllipse
            || m_currentMode == QVP::drawDot || m_currentMode == QVP::move
            || m_currentMode == QVP::makeOrtho || m_currentMode == QVP::crossLine){
        if (me->buttons() & Qt::LeftButton){
            m_tmpShape->handleMouseMoveEvent(me);
        }
        updateImage();
    } elif (m_currentMode == QVP::drawEllipticCurve && m_tmpShape != nullptr){
        qDebug() << "mme=" << me;
        m_tmpShape->handleMouseMoveEvent(me);
        updateImage();
    }
}

void QVPDocument::mouseReleaseEvent(QMouseEvent *me)
{
    if (m_currentMode == QVP::drawLine  || m_currentMode == QVP::drawEllipse
            || m_currentMode == QVP::drawDot){
        if (me->button() == Qt::LeftButton){
            m_tmpShape->handleMouseReleaseEvent(me);
            m_shapesList.append(m_tmpShape);
            m_tmpShape = nullptr;
        }
        updateImage();
    } elif (m_currentMode == QVP::drawEllipticCurve){
        if (me->button() == Qt::LeftButton){
            m_tmpShape->handleMouseReleaseEvent(me);
            if (m_tmpShape->isReady()){
                m_shapesList.append(m_tmpShape);
                m_tmpShape = nullptr;
            }
        }
        updateImage();
    } elif (m_currentMode == QVP::move){
        QVPLine* linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        QPointF vec(b.x() - a.x(), b.y() - a.y());
        for (auto shape : m_selectedShapesList){
            shape->move(vec);
        }
        delete m_tmpShape;
        m_tmpShape = nullptr;
        updateImage();
    } elif (m_currentMode == QVP::makeOrtho){
        if (m_selectedShapesList.isEmpty()){
            previousWasFail = true;
            emit sendMsgToUI("Ничего не выбрано!", true);
            emit switchToSelection();
        }
        QVPLine* linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        double targetLenght = sqrt((b.x() - a.x())*(b.x() - a.x()) +
                             (b.y() - a.y())*(b.y() - a.y()));
        QVPLine* selectedLinePtr = qobject_cast<QVPLine *>(m_selectedShapesList.last());
        QPointF sa = selectedLinePtr->getFirst();
        QPointF sb = selectedLinePtr->getLast();
        float x = sb.x() - sa.x();
        float y = sb.y() - sa.y();
        double lenght = sqrt(x*x + y*y);
        float scale = targetLenght/lenght;
        b.setX(a.x() + y * scale);
        b.setY(a.y() - x * scale);
        m_shapesList.append(new QVPLine(this, QVP::penColor, a, b));
        delete m_tmpShape;
        m_tmpShape = nullptr;
        updateImage();
        emit switchToSelection();
    } elif (m_currentMode == QVP::crossLine){
        QVPLine* linePtr = qobject_cast<QVPLine *>(m_tmpShape);
        QPointF a = linePtr->getFirst();
        QPointF b = linePtr->getLast();
        for (auto shape : m_selectedShapesList){
            auto tmpList = shape->cutLine(a, b);
            m_shapesList.append(tmpList);
        }
        updateImage();
        emit switchToSelection();
    }
}

void QVPDocument::paintEvent(QPaintEvent *event)
{
    const QRect paintRect = event->rect();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);




    painter.drawImage(paintRect, *m_mainImage, paintRect);

}

void QVPDocument::setEditorMode(QVP::editorMode em)
{
    qDebug() << __FUNCTION__;
    if (!previousWasFail && ((m_currentMode == QVP::selectShape &&
            !(em == QVP::move || em == QVP::makeOrtho ||
             em == QVP::clipRectangle || em == QVP::crossLine ||
             em == QVP::setUp)) ||
            (m_currentMode == QVP::move ||
             m_currentMode == QVP::makeOrtho ||
             m_currentMode == QVP::clipRectangle ||
             m_currentMode == QVP::crossLine ||
             m_currentMode == QVP::setUp))){
        unSelect();
    }
    m_currentMode = em;

    if (m_currentMode == QVP::selectShape && !previousWasFail){
        emit sendMsgToUI("Выберите фигуру", false);
    }
    previousWasFail = false;

    if (m_tmpShape) {
        delete m_tmpShape;
        m_tmpShape = nullptr;
    }
    updateImage();


}

void QVPDocument::unSelect()
{
    for (auto shape : m_selectedShapesList){
        shape->select(false);
    }
    m_selectedShapesList.clear();
    emit shapeSelected(false);
}

void QVPDocument::updateImage()
{
    m_mainImage->fill(QVP::backgroundColor);
    for (auto shape : m_shapesList){
        shape->getRasterized()(m_mainImage);
    }
    if (m_tmpShape){
        m_tmpShape->getRasterized()(m_mainImage);
    }

    update(m_mainImage->rect());
}

void QVPDocument::parseString(QString inStr)
{
    QStringList inList = inStr.split(';');
    qDebug() << inList;

    if (inList[0] == "D"){
        QColor color(colorFrom8BitStr(inList[3]));
        QPointF point(inList[1].toFloat(), inList[2].toFloat());
        int width  = inList[4].toInt();
        m_shapesList.append(new QVPDot(this, color, point, width));
    } elif (inList[0] == "L"){
        QColor color(colorFrom8BitStr(inList[5]));
        QPointF first(inList[1].toFloat(), inList[2].toFloat());
        QPointF last(inList[3].toFloat(), inList[4].toFloat());
        int width  = inList[6].toInt();
        m_shapesList.append(new QVPLine(this, color, first, last, width));
    } elif (inList[0] == "E"){
        QColor color(colorFrom8BitStr(inList[5]));
        QPointF center(inList[1].toFloat(), inList[2].toFloat());
        float a = inList[3].toFloat();
        float b = inList[4].toFloat();
        int width  = inList[6].toInt();
        m_shapesList.append(new QVPEllipse(this, color, center, a, b, width));
    } elif (inList[0] == "A"){
        QColor color(colorFrom8BitStr(inList[7]));
        QPointF center(inList[1].toFloat(), inList[2].toFloat());
        float a = inList[3].toFloat();
        float b = inList[4].toFloat();
        float ang1 = inList[5].toFloat();
        float ang2 = inList[6].toFloat();
        int width  = inList[8].toInt();
        m_shapesList.append(new QVPEllipticArc(this, color, center, a, b, ang1, ang2, width));
    } else {
        qWarning() << "BAD LINE";
    }
}

QColor colorFrom8BitStr(QString str)
{
    qDebug() << str;

    if (str.length() > 3){
        throw std::runtime_error("bad color");
    }

    std::string sstr = str.toLower().toStdString();

    quint8 red =    str[0] != '0' ? ((sstr[0] > '9' ? sstr[0] - 'a' + 0xA : sstr[0] - '0') << 4) + 0xF  : 0x00;
    quint8 green =  str[1] != '0' ? ((sstr[1] > '9' ? sstr[1] - 'a' + 0xA : sstr[1] - '0') << 4) + 0xF  : 0x00;
    quint8 blue =   str[2] != '0' ? ((sstr[2] > '9' ? sstr[2] - 'a' + 0xA : sstr[2] - '0') << 4) + 0xF  : 0x00;

    qDebug() << QString("%1.%2.%3").arg(red , 0, 16).arg(green , 0, 16).arg(blue , 0, 16);
    return QColor(red, green, blue, 0xFF);
}

bool QVPDocument::loadFromFile(const QString& fileName)
{
    bool status;
    QFile file(fileName);
    if ((status = file.open(QIODevice::ReadOnly))) {
        QTextStream stream(&file);

        while(!m_shapesList.empty()){
            delete m_shapesList.takeLast();
        }
        m_shapesList.clear();

        while(!stream.atEnd()){
            parseString(stream.readLine());
        }
        updateImage();
    }
    return status;
}

bool QVPDocument::saveToFile(const QString& fileName)
{
    bool status;
    QFile file(fileName);
    if ((status = file.open(QIODevice::WriteOnly))) {
        QTextStream stream(&file);
        for (QVPShape* shape : m_shapesList){
               stream << shape->toString();
        }
        file.close();
    }
    return status;
}
