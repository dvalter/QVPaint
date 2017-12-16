#include "qvpdocument.h"
#include <QPainter>

QVPDocument::QVPDocument(QWidget *parent) :
    QLabel(parent),
    m_mainImage(new QImage(":/sample.png")),
    m_imgLbl(new QLabel)
{
    qDebug() << __FUNCTION__;
    m_imgLbl->setBackgroundRole(QPalette::Base);
    m_imgLbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_imgLbl->setScaledContents(true);
    m_imgLbl->setPixmap(QPixmap::fromImage(*m_mainImage));
    resize(500,500);
}

void QVPDocument::mousePressEvent(QMouseEvent* me)
{
    qDebug() << __FUNCTION__ << me;
    m_counter++;
    if (m_counter > 10){
        qDebug() << "Upd2; couner=" << m_counter;
        *m_mainImage = QImage(":/sample.png");
        m_counter = 0;
        m_imgLbl->setPixmap(QPixmap::fromImage(*m_mainImage));
    } else if (m_counter > 5){
        qDebug() << "Upd1; couner=" << m_counter;
        *m_mainImage = QImage(":/sample.jpg");
        m_imgLbl->setPixmap(QPixmap::fromImage(*m_mainImage));
    }
}

void QVPDocument::mouseMoveEvent(QMouseEvent *me)
{
    qDebug() << __FUNCTION__ << me;
}

void QVPDocument::mouseReleaseEvent(QMouseEvent *me)
{
    qDebug() << __FUNCTION__ << me;
}

void QVPDocument::paintEvent(QPaintEvent *event)
{
    qDebug() << event;
    const QRect paintRect = event->rect();
    qDebug() << paintRect;
    QPainter painter(this);
    painter.drawImage(paintRect, *m_mainImage, paintRect);
}
