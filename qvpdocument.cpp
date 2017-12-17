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
//    m_counter++;
//    if (m_counter > 10){
//        qDebug() << "Upd2; couner=" << m_counter;
//        *m_mainImage = QImage(":/sample.png");
//        m_counter = 0;
//        m_imgLbl->setPixmap(QPixmap::fromImage(*m_mainImage));
//    } else if (m_counter > 5){
//        qDebug() << "Upd1; couner=" << m_counter;
//        *m_mainImage = QImage(":/sample.jpg");
//        m_imgLbl->setPixmap(QPixmap::fromImage(*m_mainImage));
//    }
    if (m_currentMode == QVP::drawLine){
        if (me->button() == Qt::LeftButton){
            m_tmpShape = new QVPLine();
            m_tmpShape->handleMousePressEvent(me);
        }
        updateImage();
    }
}

void QVPDocument::mouseMoveEvent(QMouseEvent *me)
{
    qDebug() << __FUNCTION__ << me;

    emit updateCoord(me->pos());

    if (m_currentMode == QVP::drawLine){
        if (me->buttons() & Qt::LeftButton){
            m_tmpShape->handleMouseMoveEvent(me);
        }
        updateImage();
    }
}

void QVPDocument::mouseReleaseEvent(QMouseEvent *me)
{
    qDebug() << __FUNCTION__ << me;
    if (m_currentMode == QVP::drawLine){
        if (me->button() == Qt::LeftButton){
            m_tmpShape->handleMouseReleaseEvent(me);
            m_shapesList.append(m_tmpShape);
            m_tmpShape = nullptr;
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

    //QImage background(":/sample.png");
    QImage background(m_mainImage->size(), m_mainImage->format());
    background.fill(QVP::backgroundColor);
    qDebug() << background;

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
