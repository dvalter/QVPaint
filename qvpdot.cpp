#include "qvpdot.h"
#include <string.h>
#include <sstream>


QVPDot::QVPDot(QColor penColor):
    QVPShape(nullptr, penColor)
{
    m_shapePoints->reserve(1);
}

QVPDot::QVPDot(QObject * parent, QColor penColor):
    QVPShape(parent, penColor)
{
    m_shapePoints->reserve(1);
}

QVPDot::QVPDot(QObject * parent):
    QVPShape(parent, QVP::penColor)
{
    m_shapePoints->reserve(1);
}


QVPDot::QVPDot(QObject *parent, QColor penColor, QPointF point, int width):
    QVPShape(parent, penColor, width),
    m_firstPoint(point)
{
    m_shapePoints->push_back(QPoint());
    update();
}


void QVPDot::update()
{
    QColor color;
    if (m_selected){
        color = QColor(0xFF - m_penColor.red(), 0xFF - m_penColor.green(), 0xFF - m_penColor.blue(), 0xFF);
        if (color == QVP::backgroundColor){
            color = QColor(0xFF, 0x0, 0x0, 0xFF); // shoud be red
        }
        qDebug() << color;
    } else {
        color = m_penColor;
    }


    if (m_rasterized){
        delete m_rasterized;
    }
    (*m_shapePoints)[0] = QPoint(m_firstPoint.x(), m_firstPoint.y());
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, m_width);
}



void QVPDot::handleMousePressEvent(QMouseEvent * me)
{
    m_firstPoint = me->pos();
    if (m_shapePoints->size() == 0){
        m_shapePoints->push_back(me->pos());
    }
    m_mousePressed = true;
    update();
}

void QVPDot::handleMouseMoveEvent(QMouseEvent * me)
{
    m_firstPoint = me->pos();
    update();
}

void QVPDot::handleMouseReleaseEvent(QMouseEvent * me)
{
    m_firstPoint = me->pos();
    m_mousePressed = false;
    update();
}

QVPRasterizedShape& QVPDot::getRasterized()
{
    if (!m_rasterized)
        update();
    return *m_rasterized;
}

QString QVPDot::toString()
{
    std::stringstream ss;
    ss << "D;" << m_firstPoint.x() << ";" << m_firstPoint.y() << ";" <<
          QString("%1%2%3").arg(m_penColor.red() / 0x10, 0, 16)
          .arg(m_penColor.green() / 0x10, 0, 16)
          .arg(m_penColor.blue() / 0x10, 0, 16).toStdString() << ";" << m_width << "\n";
    return QString::fromStdString(std::string(ss.str()));
}

void QVPDot::move(QPointF vec)
{
    m_firstPoint.rx() += vec.x();
    m_firstPoint.ry() += vec.y();
    update();
}
