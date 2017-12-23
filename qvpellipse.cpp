#include "qvpellipse.h"
#include <string.h>



QVPEllipse::QVPEllipse(QColor penColor):
    QVPShape(nullptr, penColor)
{

}

QVPEllipse::QVPEllipse(QObject * parent, QColor penColor):
    QVPShape(parent, penColor)
{

}

QVPEllipse::QVPEllipse(QObject * parent):
    QVPShape(parent, QVP::penColor)
{

}





QVector<QPoint> bresenham_ellipse(int x, int y, int a, int b)
{
    QVector<QPoint> ellipse;
    if (!a && !b){
        ellipse.append(QPoint(x,y));
        return ellipse;
    }
//    int col,row;
//    long a_square,b_square,two_a_square,two_b_square,four_a_square,four_b_square,d;

    long b_square = b * b;
    long a_square = a * a;
    int row = b;
    int col = 0;
    long two_a_square = a_square<<1;
    long four_a_square = a_square<<2;
    long four_b_square = b_square<<2;
    long two_b_square = b_square<<1;
    long d = two_a_square * ((row - 1) * (row)) + a_square + two_b_square * (1 - a_square);
    while(a_square*(row)>b_square*(col)){
        ellipse.append(QPoint(col+x,row+y));
        ellipse.append(QPoint(col+x,y-row));
        ellipse.append(QPoint(x-col,row+y));
        ellipse.append(QPoint(x-col,y-row));
        if (d >= 0){
            row--;
            d -= four_a_square * (row);
        }
        d += two_b_square * (3 + (col << 1));
        col++;
    }
    d = two_b_square * (col + 1) * col + two_a_square *
            (row * (row - 2) + 1) + (1 - two_a_square) * b_square;
    while ((row) + 1){
        ellipse.append(QPoint(col+x,row+y));
        ellipse.append(QPoint(col+x,y-row));
        ellipse.append(QPoint(x-col,row+y));
        ellipse.append(QPoint(x-col,y-row));
        if (d <= 0){
            col++;
            d += four_b_square*col;
        }
        row--;
        d += two_a_square*(3-(row <<1));
    }


    /*row = b;
    col = 0;
    d = two_a_square * ((row - 1) * (row)) + a_square + two_b_square * (1 - a_square);
    while(a_square*(row)>b_square*(col)){
        ellipse.append(QPoint(col+x,y-row));
//        ellipse[2].append(QPoint(x-col,row+y));
//        ellipse[3].append(QPoint(x-col,y-row));
        if (d >= 0){
            row--;
            d -= four_a_square * (row);
        }
        d += two_b_square * (3 + (col << 1));
        col++;
    }
    d = two_b_square * (col + 1) * col + two_a_square *
            (row * (row - 2) + 1) + (1 - two_a_square) * b_square;
    while ((row) + 1){
        ellipse.append(QPoint(col+x,y-row));
//        ellipse[2].append(QPoint(x-col,row+y));
//        ellipse[3].append(QPoint(x-col,y-row));
        if (d <= 0){
            col++;
            d += four_b_square*col;
        }
        row--;
        d += two_a_square*(3-(row <<1));
    }*/

    return ellipse;
}



void QVPEllipse::update()
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

    drawEllipse(color);
}

void QVPEllipse::drawEllipse(QColor color)
{
    m_image->fill(QColor(0x00, 0x00, 0x00, 0x00));
    QPainter painter(m_image);
    QPen pen(color);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(Qt::NoBrush));
    QVector<QPoint> vec = bresenham_ellipse(
                m_center.x(), m_center.y(), m_a, m_b);

    painter.drawPoints(vec.data(), vec.size());


    if (m_selected){
        pen.setWidth(10);
        pen.setColor(QColor(0xFF, 0x00, 0x0, 0xFF));
        painter.setPen(pen);
        painter.drawPoint(m_center);
        //painter.end();
        pen.setColor(QColor(0xFF, 0xFF, 0x0, 0xFF));
        painter.setPen(pen);
        painter.drawPoint(QPoint(m_center.x() + m_a, m_center.y() + m_b));
        painter.drawPoint(QPoint(m_center.x() - m_a, m_center.y() - m_b));
        painter.drawPoint(QPoint(m_center.x() + m_a, m_center.y() - m_b));
        painter.drawPoint(QPoint(m_center.x() - m_a, m_center.y() + m_b));
    }
}

void QVPEllipse::handleMousePressEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_firstPoint = me->pos();
    m_lastPoint = me->pos();
    m_center = m_firstPoint;
    m_a = 0;
    m_b = 0;
    m_mousePressed = true;
    //update();
}

void QVPEllipse::handleMouseMoveEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_lastPoint = me->pos();
    initEllipseParams();
    update();
}



void QVPEllipse::handleMouseReleaseEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    m_lastPoint = me->pos();
    initEllipseParams();
    m_mousePressed = false;
    update();
}

inline void QVPEllipse::initEllipseParams(){
    m_center = QPoint(abs(m_firstPoint.x() + m_lastPoint.x()) / 2, abs(m_firstPoint.y() + m_lastPoint.y()) / 2);
    m_a = abs(m_firstPoint.x() - m_lastPoint.x()) / 2;
    m_b = abs(m_firstPoint.y() - m_lastPoint.y()) / 2;
}
