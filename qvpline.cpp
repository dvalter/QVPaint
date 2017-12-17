#include "qvpline.h"
#include <string.h>


QVPLine::QVPLine(QColor penColor):
    QVPShape(nullptr, penColor)
{
    testWGT = new QLabel();

    testWGT->setPixmap(QPixmap::fromImage(*m_image));
    //testWGT->show();

}

QVPLine::QVPLine(QObject * parent, QColor penColor):
    QVPShape(parent, penColor)
{
    testWGT = new QLabel();

    testWGT->setPixmap(QPixmap::fromImage(*m_image));
    //testWGT->show();
}

QVPLine::QVPLine(QObject * parent):
    QVPShape(parent, QVP::penColor)
{
    testWGT = new QLabel();

    testWGT->setPixmap(QPixmap::fromImage(*m_image));
    //testWGT->show();
}


static QVector<QPoint> bresenham_line(int x1, int y1, int x2, int y2)
{
    QVector<QPoint> line;
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

        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
    return line;
}

void QVPLine::update()
{
    QColor color;
    if (m_selected){
        color = QColor(0xFF - m_penColor.red(), 0xFF - m_penColor.green(), 0xFF - m_penColor.blue(), 0xFF);
        if(color == QVP::backgroundColor){
            color = QColor(0xFF, 0x0, 0x0, 0xFF); // shoud be red
        }
    } else {
        color = m_penColor;
    }

    drawLine(color);
}

void QVPLine::drawLine(QColor color)
{
    m_image->fill(QColor(0x00, 0x00, 0x00, 0x00));
    QPainter painter(m_image);
    QPen pen(color);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QVector<QPoint> vec = bresenham_line(
                m_firstPoint.x(), m_firstPoint.y(),
                m_lastPoint.x(), m_lastPoint.y());

    painter.drawPolyline(vec.data(), vec.size());
    testWGT->setPixmap(QPixmap::fromImage(*m_image));

    if(m_selected){
        pen.setWidth(10);
        pen.setColor(QColor(0xFF, 0xFF, 0x0, 0xFF));
        painter.setPen(pen);
        painter.drawPoint(m_firstPoint);
        painter.drawPoint(m_lastPoint);
    }
}

void QVPLine::handleMousePressEvent(QMouseEvent * me)
{
    qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_firstPoint = me->pos();
    m_lastPoint = me->pos();
    m_mousePressed = true;
    update();
}

void QVPLine::handleMouseMoveEvent(QMouseEvent * me)
{
    qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_lastPoint = me->pos();
    update();
}

void QVPLine::handleMouseReleaseEvent(QMouseEvent * me)
{
    qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_lastPoint = me->pos();
    m_mousePressed = false;
    update();
}
