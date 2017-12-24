#include "qvpline.h"
#include <string.h>
#include <sstream>




QVPLine::QVPLine(QColor penColor):
    QVPShape(nullptr, penColor)
{

}

QVPLine::QVPLine(QObject * parent, QColor penColor):
    QVPShape(parent, penColor)
{

}

QVPLine::QVPLine(QObject * parent):
    QVPShape(parent, QVP::penColor)
{

}

QVPLine::QVPLine(QObject *parent, QColor penColor, QPointF first, QPointF last, int width):
    QVPShape(parent, penColor, width),
    m_firstPoint(first),
    m_lastPoint(last)
{
    update();
}

void bresenham_line(QVector<QPoint>& line, int x1, int y1, int x2, int y2)
{
//    QVector<QPoint> line;
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    line.append(QPoint(x2, y2));
    while(x1 != x2 || y1 != y2)
    {
        line.append(QPoint(x1, y1));

        const int error2 = error * 2;

        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
    //return line;
}

void QVPLine::update()
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

    m_shapePoints->clear();
    bresenham_line(*m_shapePoints,
                m_firstPoint.x(), m_firstPoint.y(),
                m_lastPoint.x(), m_lastPoint.y());

    if (m_rasterized){
        delete m_rasterized;
    }
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, 2);
}



void QVPLine::handleMousePressEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_firstPoint = me->pos();
    m_lastPoint = me->pos();
    m_mousePressed = true;
    update();
}

void QVPLine::handleMouseMoveEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_lastPoint = me->pos();
    update();
}

void QVPLine::handleMouseReleaseEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_lastPoint = me->pos();
    m_mousePressed = false;
    update();
}

QVPRasterizedShape& QVPLine::getRasterized()
{
    if (!m_rasterized)
        update();
    return *m_rasterized;
}

QString QVPLine::toString()
{
    std::stringstream ss;
    ss << "L;" << m_firstPoint.x() << ";" << m_firstPoint.y() << ";" <<
          m_lastPoint.x() << ";" << m_lastPoint.y() << ";" <<
          QString("%1%2%3").arg(m_penColor.red() / 0x10, 0, 16)
          .arg(m_penColor.green() / 0x10, 0, 16)
          .arg(m_penColor.blue() / 0x10, 0, 16).toStdString() << ";" << m_width << "\n";
    return QString::fromStdString(std::string(ss.str()));
}

void QVPLine::move(QPointF vec)
{
    m_firstPoint.rx() += vec.x();
    m_firstPoint.ry() += vec.y();
    m_lastPoint.rx() += vec.x();
    m_lastPoint.ry() += vec.y();
    update();
}
