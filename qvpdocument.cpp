#include "qvpdocument.h"
#include <QPainter>
#include <QDebug>
#include <stdlib.h>




QVPDocument::QVPDocument(QWidget* parent) :
    QLabel(parent),
    m_mainImage(new QImage(QVP::imageWigth, QVP::imageHeight, QImage::Format_ARGB32_Premultiplied)),
    m_imgLbl(new QLabel)
{
    qDebug() << __FUNCTION__;
    m_mainImage->fill(QVP::backgroundColor);
    resize(QVP::imageWigth, QVP::imageHeight);

    setMouseTracking(true);

    m_shapesList.append(new QVPEllipticArc(this, QVP::penColor, QPointF(100, 100), 50.0, 50.0, -M_PI_2, M_PI_2));
    update();
}

void QVPDocument::mousePressEvent(QMouseEvent* me)
{
    qDebug() << __FUNCTION__ << me;
    if (me->button() == Qt::LeftButton){
        if (m_currentMode == QVP::drawDot){
            m_tmpShape = new QVPDot(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        } elif (m_currentMode == QVP::drawLine){
            m_tmpShape = new QVPLine(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        } elif (m_currentMode == QVP::drawEllipse){
            m_tmpShape = new QVPEllipse(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        } elif (m_currentMode == QVP::drawEllipticCurve){
            if (m_tmpShape == nullptr){
                m_tmpShape = new QVPEllipticArc(this);
            }
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        } elif (m_currentMode == QVP::selectShape){
            searchPixel(me->pos());
            updateImage();
        }
    }
}

void QVPDocument::searchPixel(QPoint point)
{
    if (-1 != checkPixel(point))
        return;
    int x = point.x();
    int y = point.y();
    for(int i = 0; i < QVP::searchWidth; i++){
        for(int j = 0; j < i + 2; j++){
            int xDelta = j;
            int yDelta = i - j + 1;
            if (-1 != checkPixel(QPoint(x + xDelta, y + yDelta)))
                return;
            if (-1 != checkPixel(QPoint(x - xDelta, y + yDelta)))
                return;
            if (-1 != checkPixel(QPoint(x + xDelta, y - yDelta)))
                return;
            if (-1 != checkPixel(QPoint(x - xDelta, y - yDelta)))
                return;
        }
    }
}

int QVPDocument::checkPixel(QPoint point){

//    m_mainImage->setPixelColor(point, Qt::blue);
//    update(m_mainImage->rect());

    int counter = 0;
    for(QVPShape* shape : m_shapesList){
        if (qAlpha(shape->getImage().pixel(point.x(), point.y())) != 0x00){
            shape->select(true);
            qDebug()  << shape->metaObject()->className();
            return counter;
        } else {
            shape->select(false);
        }
        counter++;
    }
    if (counter >= m_shapesList.size()){
        return -1;
    } else {
        return -2;
    }
}


void QVPDocument::mouseMoveEvent(QMouseEvent *me)
{
    //Debug() << __FUNCTION__ << me;

    emit updateCoord(me->pos());

    if (m_currentMode == QVP::drawLine || m_currentMode == QVP::drawEllipse
            || m_currentMode == QVP::drawDot){
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
//    qDebug() << __FUNCTION__ << me;
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
    }
}

void QVPDocument::paintEvent(QPaintEvent *event)
{
//    qDebug() << event;


//    qDebug() << m_mainImage;
    const QRect paintRect = event->rect();

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);




    painter.drawImage(paintRect, *m_mainImage, paintRect);

}

void QVPDocument::setEditorMode(QVP::editorMode em)
{
    qDebug() << __FUNCTION__;
    m_currentMode = em;
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

//void QVPDocument::updateImageOld()
//{
//    QPainter pm(m_mainImage);

//    QImage background(m_mainImage->size(), m_mainImage->format());
//    background.fill(QVP::backgroundColor);

//    pm.setCompositionMode(QPainter::CompositionMode_Source);
//    pm.fillRect(m_mainImage->rect(), Qt::transparent);

//    pm.setCompositionMode(QPainter::CompositionMode_SourceOver);
//    pm.drawImage(0,0, background);

//    for(auto shape : m_shapesList){
//        pm.drawImage(0,0, shape->getImage());

//    }

//    if (m_tmpShape != nullptr){
//        pm.drawImage(0,0, m_tmpShape->getImage());

//    }
//    pm.end();

//    update(m_mainImage->rect());
//}

bool QVPDocument::saveToFile(QString fileName)
{
    bool status;
    QFile file(fileName);
    if ((status = file.open(QIODevice::ReadWrite))) {
        QTextStream stream(&file);
        for (QVPShape* shape : m_shapesList){
               stream << shape->toString();
        }
        file.close();
    }
    return status;
}
