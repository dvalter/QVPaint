#include "qvpellipticarc.h"
#include <string.h>
#include <sstream>
#include <assert.h>
#include <stdio.h>

inline float angleFromSC(float sin, float cos);
inline float Q_rsqrt( float number );

QVPEllipticArc::QVPEllipticArc(QColor penColor):
    QVPShape(nullptr, penColor)
{

}

QVPEllipticArc::QVPEllipticArc(QObject * parent, QColor penColor):
    QVPShape(parent, penColor)
{

}

QVPEllipticArc::QVPEllipticArc(QObject * parent):
    QVPShape(parent, QVP::penColor)
{

}

QVPEllipticArc::QVPEllipticArc(QObject *parent, QColor penColor, QPointF center,
                               float a, float b, float a1, float a2):
    QVPShape(parent, penColor),
    m_center(center),
    m_a(a),
    m_b(b),
    m_ang1(a1),
    m_ang2(a2),
    m_state(5)
{
    update();
}

inline void QVPEllipticArc::appendPoint(QPoint point, QVector<QPoint>& vec)
{
    if (m_state < 4 || checkPoint(point)){
        //qDebug() << angleFromSC(sin(point), cos(point));
        vec.append(point);
    }
}

 void QVPEllipticArc::bresenham_elliptic_arc(QVector<QPoint>& ellipse, int x, int y, int a, int b)
{
//    qDebug() << "andgle1=" << m_ang1 << " angle2=" << m_ang2;
//    QVector<QPoint> ellipse;
    if (!a && !b){
        ellipse.append(QPoint(x,y));
        return;
    }


    QPoint point;
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

        appendPoint(QPoint(col+x,row+y), ellipse);
        appendPoint(QPoint(col+x,y-row), ellipse);
        appendPoint(QPoint(x-col,row+y), ellipse);
        appendPoint(QPoint(x-col,y-row), ellipse);
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
        appendPoint(QPoint(col+x,row+y), ellipse);
        appendPoint(QPoint(col+x,y-row), ellipse);
        appendPoint(QPoint(x-col,row+y), ellipse);
        appendPoint(QPoint(x-col,y-row), ellipse);
        if (d <= 0){
            col++;
            d += four_b_square*col;
        }
        row--;
        d += two_a_square*(3-(row <<1));
    }

    //eturn ellipse;
}



void QVPEllipticArc::update()
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

    if (m_state == 2 || m_state == 3){
        drawLine(color, m_firstPoint);
    } elif (m_state == 4){
        drawLine(color, m_firstPoint);
        drawLine(color, m_lastPoint);
    }

    if (m_rasterized != nullptr){
        delete m_rasterized;
    }
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, 2);
}

void QVPEllipticArc::drawEllipse(QColor color)
{
//    m_image->fill(QColor(0x00, 0x00, 0x00, 0x00));
//    //m_image->fill(QColor(0x00, 0x00, 0x00, 0xFF));
//    QPainter painter(m_image);
//    QPen pen(color);
//    painter.setPen(pen);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setBrush(QBrush(Qt::NoBrush));

    m_shapePoints->clear();
    bresenham_elliptic_arc(*m_shapePoints,
                m_center.x(), m_center.y(), m_a, m_b);

//    painter.drawPoints(m_shapePoints->data(), m_shapePoints->size());


//    if (m_selected){
//        pen.setWidth(10);
//        pen.setColor(QColor(0xFF, 0xFF, 0x0, 0xFF));
//        painter.setPen(pen);
//        painter.drawPoint(m_center);
//        pen.setColor(QColor(0xFF, 0xFF, 0x0, 0xFF));
//        painter.setPen(pen);
//        painter.drawPoint(QPoint(m_center.x() + m_a, m_center.y() + m_b));
//        painter.drawPoint(QPoint(m_center.x() - m_a, m_center.y() - m_b));
//        painter.drawPoint(QPoint(m_center.x() + m_a, m_center.y() - m_b));
//        painter.drawPoint(QPoint(m_center.x() - m_a, m_center.y() + m_b));
//    }
}



void QVPEllipticArc::drawLine(QColor color, QPointF point){
//    QPainter painter(m_image);
//    QPen pen(color);
//    pen.setWidth(1);
//    painter.setPen(pen);
//    painter.setRenderHint(QPainter::Antialiasing, true);

    bresenham_line(*m_shapePoints,
                m_center.x(), m_center.y(),
                point.x(), point.y());


//    painter.drawPoints(m_shapePoints->data(), m_shapePoints->size());
}





void QVPEllipticArc::handleMousePressEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    qDebug() << __FILE__ << ":" << __FUNCTION__ << "\tstate=" << m_state
             << "\nf=" << m_firstPoint << " l=" << m_lastPoint
             << "\nc=" << m_center << " a=" << m_a << " b=" << m_b
             << "\na1=" << m_ang1 << " a2=" << m_ang2;

    if (m_state < 2){
        m_firstPoint = me->pos();
        m_lastPoint = me->pos();
        m_center = m_firstPoint;
        m_a = 0;
        m_b = 0;
    } elif (m_state < 4) {
        m_firstPoint = me->pos();
    } else {
        m_lastPoint = me->pos();
    }
    m_mousePressed = true;
    m_state++;
}

void QVPEllipticArc::handleMouseMoveEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    qDebug() << __FILE__ << ":" << __FUNCTION__ << "\tstate=" << m_state
             << "\nf=" << m_firstPoint << " l=" << m_lastPoint
             << "\nc=" << m_center << " a=" << m_a << " b=" << m_b
             << "\na1=" << m_ang1 << " a2=" << m_ang2;
    if (m_state < 2){
        m_lastPoint = me->pos();
        initEllipseParams();
    } elif (m_state < 4) {
        m_firstPoint = me->pos();
        m_ang1 = angleFromSC(sin(m_firstPoint), cos(m_firstPoint));
    } else {
        m_lastPoint = me->pos();
        m_ang2 = angleFromSC(sin(m_lastPoint), cos(m_lastPoint));
    }
    update();
}



void QVPEllipticArc::handleMouseReleaseEvent(QMouseEvent * me)
{
    //qDebug() << __FILE__ << ":" << __FUNCTION__;
    qDebug() << __FILE__ << ":" << __FUNCTION__ << "\tstate=" << m_state
             << "\nf=" << m_firstPoint << " l=" << m_lastPoint
             << "\nc=" << m_center << " a=" << m_a << " b=" << m_b
             << "\na1=" << m_ang1 << " a2=" << m_ang2;
    if (m_state < 2){
        m_lastPoint = me->pos();
        initEllipseParams();
    } elif (m_state < 4) {
        m_firstPoint = me->pos();
        m_ang1 = angleFromSC(sin(m_firstPoint), cos(m_firstPoint));
    } else {
        m_lastPoint = me->pos();
//        m_ang1 = angleFromSC(sin(m_firstPoint), cos(m_firstPoint));
//        m_ang2 = angleFromSC(sin(m_lastPoint), cos(m_lastPoint));
    }
    m_mousePressed = false;
    m_state++;
    update();
}

inline float QVPEllipticArc::sin(QPointF point)
{
    volatile float x = point.x() - m_center.x();
    volatile float y = point.y() - m_center.y();
    return y*Q_rsqrt(x*x + y*y);
}


inline float QVPEllipticArc::cos(QPointF point)
{
    volatile float x = point.x() - m_center.x();
    volatile float y = point.y() - m_center.y();
    return x*Q_rsqrt(x*x + y*y);
}

inline float Q_rsqrt( float number )
{
    volatile long i;
    volatile float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the hell?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

inline void QVPEllipticArc::initEllipseParams(){
    m_center = QPoint(abs(m_firstPoint.x() + m_lastPoint.x()) / 2, abs(m_firstPoint.y() + m_lastPoint.y()) / 2);
    m_a = abs(m_firstPoint.x() - m_lastPoint.x()) / 2;
    m_b = abs(m_firstPoint.y() - m_lastPoint.y()) / 2;
}

inline float angleFromSC(float sin, float cos){
    float asin = asinf(sin);
    float acos = acosf(cos);
    if (acos < 0.7){
        return -asin;
    } elif (acos > 2.4){
        return asin < 0 ? M_PI + asin : - M_PI + asin;
    } else {
        return asin < 0 ? acos :  -acos;
    }
}

inline bool QVPEllipticArc::checkPoint(QPoint point){
    float angle = angleFromSC(sin(point), cos(point));

    if (m_ang1 < m_ang2){
        return angle > m_ang1 && angle < m_ang2;
    } elif (m_ang1 > m_ang2) {
        return angle > m_ang1 || angle < m_ang2;
    } else {
        return true;
    }

//    if (m_ang1 - m_ang2 > M_PI){
////        qDebug() << "case A";
//        result =  angle > m_ang1  || angle < m_ang2;
//    } elif (m_ang2 - m_ang1 > M_PI){
////        qDebug() << "case B";
//        result =  angle > m_ang2  || angle < m_ang1;
//    } elif (m_ang1 < m_ang2){
////        qDebug() << "case 1";
//        result =  angle > m_ang1  && angle < m_ang2;
//    } else {
////        qDebug() << "case 2";
//        result =  angle < m_ang1  && angle > m_ang2;
//    }

//    return result;

}

QString QVPEllipticArc::toString()
{
    std::stringstream ss;
    ss << "A;" << m_center.x() << ";" << m_center.y() << ";" <<
              m_a << ";" << m_b << ";" << m_ang1 << ";" << m_ang2 << ";\n";
    return QString::fromStdString(std::string(ss.str()));
}
