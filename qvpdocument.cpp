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

//    m_imgLbl->setBackgroundRole(QPalette::Base);
//    m_imgLbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    m_imgLbl->setScaledContents(true);
//    m_imgLbl->setPixmap(QPixmap::fromImage(*m_mainImage));
//    m_currentMode = QVP::selectShape;


//    srand((quint64)this);
}

void QVPDocument::mousePressEvent(QMouseEvent* me)
{
    qDebug() << __FUNCTION__ << me;
    if (me->button() == Qt::LeftButton){
        if (m_currentMode == QVP::drawLine){
            m_tmpShape = new QVPLine();
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        } elif (m_currentMode == QVP::drawEllipse){
            m_tmpShape = new QVPEllipse();
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        } elif (m_currentMode == QVP::drawEllipticCurve){
            if (m_tmpShape == nullptr){
                m_tmpShape = new QVPEllipticArc();
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

    if (m_currentMode == QVP::drawLine || m_currentMode == QVP::drawEllipse){
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
    if (m_currentMode == QVP::drawLine  || m_currentMode == QVP::drawEllipse){
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
    QPainter pm(m_mainImage);

    QImage background(m_mainImage->size(), m_mainImage->format());
    background.fill(QVP::backgroundColor);

    pm.setCompositionMode(QPainter::CompositionMode_Source);
    pm.fillRect(m_mainImage->rect(), Qt::transparent);

    pm.setCompositionMode(QPainter::CompositionMode_SourceOver);
    pm.drawImage(0,0, background);

    for(auto shape : m_shapesList){
        pm.drawImage(0,0, shape->getImage());

    }

    if(m_tmpShape != nullptr){
        pm.drawImage(0,0, m_tmpShape->getImage());

    }
    pm.end();

    update(m_mainImage->rect());
}
