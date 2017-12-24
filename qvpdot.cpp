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



void QVPDot::update()
{
    QColor color;
    if (m_selected){
        color = QColor(0xFF - m_penColor.red(), 0xFF - m_penColor.green(), 0xFF - m_penColor.blue(), 0xFF);
        if (color == QVP::backgroundColor){
            color = QColor(0xFF, 0x0, 0x0, 0xFF); // shoud be red
        }
    } else {
        color = m_penColor;
    }

    drawDot(color);

    if (m_rasterized){
        delete m_rasterized;
    }
    (*m_shapePoints)[0] = QPoint(m_firstPoint.x(), m_firstPoint.y());
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, 2);
}

void QVPDot::drawDot(QColor color)
{
//    m_image->fill(QColor(0x00, 0x00, 0x00, 0x00));
//    QPainter painter(m_image);
//    QPen pen(color);
//    pen.setWidth(1);
//    painter.setPen(pen);
//    painter.setRenderHint(QPainter::Antialiasing, true);

//    if (m_selected){
//        pen.setWidth(10);
//        pen.setColor(QColor(0xFF, 0xFF, 0x0, 0xFF));
//        painter.setPen(pen);
//    }
//    painter.drawPoint(m_firstPoint);
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
    ss << "D;" << m_firstPoint.x() << ";" << m_firstPoint.y() << ";\n";
    return QString::fromStdString(std::string(ss.str()));
}
