#include "qvpellipse.h"
#include <string.h>
#include <sstream>


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

QVPEllipse::QVPEllipse(QObject *parent, QColor penColor, QPointF center,
                               float a, float b, int width):
    QVPShape(parent, penColor, width),
    m_center(center),
    m_a(a),
    m_b(b)
{
    update();
}



void bresenham_ellipse(QVector<QPoint>& ellipse, int x, int y, int a, int b)
{
//    QVector<QPoint> ellipse;
    if (!a && !b){
        ellipse.append(QPoint(x,y));
        return;
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

//    return ellipse;
}



void QVPEllipse::update()
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
    bresenham_ellipse(*m_shapePoints,
                m_center.x(), m_center.y(), m_a, m_b);

    if (m_rasterized != nullptr){
        delete m_rasterized;
    }
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, 2);
}



void QVPEllipse::handleMousePressEvent(QMouseEvent * me)
{
    m_firstPoint = me->pos();
    m_lastPoint = me->pos();
    m_center = m_firstPoint;
    m_a = 0;
    m_b = 0;
    m_mousePressed = true;
    update();
}

void QVPEllipse::handleMouseMoveEvent(QMouseEvent * me)
{
    m_lastPoint = me->pos();
    initEllipseParams();
    update();
}



void QVPEllipse::handleMouseReleaseEvent(QMouseEvent * me)
{
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

QString QVPEllipse::toString()
{
    std::stringstream ss;
    ss << "E;" << m_center.x() << ";" << m_center.y() << ";" <<
          m_a << ";" << m_b << ";" <<
          QString("%1%2%3").arg(m_penColor.red() / 0x10, 0, 16)
          .arg(m_penColor.green() / 0x10, 0, 16)
          .arg(m_penColor.blue() / 0x10, 0, 16).toStdString() << ";" << m_width << "\n";
    return QString::fromStdString(std::string(ss.str()));
}

void QVPEllipse::move(QPointF vec)
{
    m_center.rx() += vec.x();
    m_center.ry() += vec.y();
    update();
}
