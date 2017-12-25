#include "qvpellipse.h"
#include <string.h>
#include <sstream>
#include "qvpellipticarc.h"

static const double Epsilon = 2;


inline float Q_rsqrt( float number );

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
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, m_width);
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

inline float QVPEllipse::sin(QPointF point)
{
    volatile float x = point.x() - m_center.x();
    volatile float y = point.y() - m_center.y();
//    return y*Q_rsqrt(x*x + y*y);
    return y / sqrt(x*x + y*y);
}


inline float QVPEllipse::cos(QPointF point)
{
    volatile float x = point.x() - m_center.x();
    volatile float y = point.y() - m_center.y();
//    return x*Q_rsqrt(x*x + y*y);
    return x / sqrt(x*x + y*y);
}




inline float angleFromSC(float sin, float cos){
    volatile float asin = asinf(sin);
    volatile float acos = acosf(cos);
    if (acos < 0.7){
        return -asin;
    } elif (acos > 2.4){
        return asin < 0 ? M_PI + asin : - M_PI + asin;
    } else {
        return asin < 0 ? acos :  -acos;
    }
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

inline void swap(volatile float& a, volatile float& b){
    volatile float tmp = a;
    a = b;
    b = tmp;
}

inline void swap(float& a, float& b){
    volatile float tmp = a;
    a = b;
    b = tmp;
}

static bool ellipseIntersectLine(volatile float a, volatile float b, volatile float h, volatile float k,
volatile float x1 , volatile float y1 , volatile float x2 , volatile float y2,
float &xi1 , float &xi2 , float &yi1 , float &yi2)
{

    bool inverted = false;
    if (abs(x1 - x2) < Epsilon) {
        swap(a, b);
        swap(h, k);
        swap(x1, y1);
        swap(x2, y2);
        inverted = true;
        qDebug() << "inverted";
    }
    volatile float aa,bb,cc,m;
    //
    if ( x1 != x2) {
        m = (y2-y1)/(x2-x1);
        float c = y1 - m*x1;
        //
        aa = b*b + a*a*m*m;
        bb = 2*a*a*c*m - 2*a*a*k*m - 2*h*b*b;
        cc = b*b*h*h + a*a*c*c - 2*a*a*k*c + a*a*k*k - a*a*b*b;
    } else {
        //
        // vertical line case
        //
        aa = a*a;
        bb = -2.0*k*a*a;
        cc = -a*a*b*b + b*b*(x1-h)*(x1-h);
    }

    float d = bb*bb-4*aa*cc;
    //
    // intersection points : (xi1,yi1) and (xi2,yi2)
    //
    if (d > 0.0) {
        if (x1 != x2) {
            xi1 = (-bb + sqrt(d)) / (2*aa);
            xi2 = (-bb - sqrt(d)) / (2*aa);
            yi1 = y1 + m * (xi1 - x1);
            yi2 = y1 + m * (xi2 - x1);
        } else {
            yi1 = (-bb + sqrt(d)) / (2*aa);
            yi2 = (-bb - sqrt(d)) / (2*aa);
            xi1 = x1;
            xi2 = x1;
        }
    } else {
        return false;	// no intersections
    }
    if (inverted) {
        swap(xi1, yi1);
        swap(xi2, yi2);
    }
    return true;
}

inline float min(const float& a, const float& b)
{
    return a<b?a:b;
}

inline float max(const float& a, const float& b)
{
    return a>b?a:b;
}

inline bool between(const QPointF& a, const QPointF& first, const QPointF& last)
{
    bool m = a.x() <= max(first.x(), last.x());
    bool n = a.y() <= max(first.y(), last.y());
    bool o = a.x() >= min(first.x(), last.x());
    bool p = a.y() >= min(first.y(), last.y());
    return m && n && o && p;

//    return a.x() < max(first.x(), last.x()) && a.y() < max(first.y(), last.y()) &&
//            a.x() > min(first.x(), last.x()) && a.y() > min(first.y(), last.y());
}

QList<QVPShape *> QVPEllipse::cutLine(QPointF first, QPointF last)
{

    float x1, x2, y1, y2;
    QList<QVPShape *> newShapes;
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), first.x(), first.y(),
                             last.x(), last.y(), x1, x2, y1, y2/*int1.rx(), int2.rx(), int1.ry(),int2.ry()*/)){
//        int1(QPointF(x1, y1));
//        int2(QPointF(x2, y2));
        QPointF int1(x1, y1), int2(x2, y2);
        qDebug() << int1 << " " << between(int1, first, last) << " " << int2 << " " << between(int2, first, last)/* << int1==int2*/;
        if (between(int1, first, last) && between(int2, first, last)){
            volatile float ang1 = angleFromSC(sin(int1), cos(int1));
            //qDebug() << ang1;
            volatile float ang2 = angleFromSC(sin(int2), cos(int2));
            //qDebug() << ang2;
            newShapes.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, ang1, ang2, m_width));
            newShapes.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, ang2, ang1, m_width));
        }
    }
    return newShapes;
}
