#include "qvpellipticarc.h"
#include <string.h>
#include <sstream>
#include <assert.h>
#include <stdio.h>

static const double Epsilon = 2;

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
                               float a, float b, float a1, float a2, int width):
    QVPShape(parent, penColor, width),
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
        vec.append(point);
    }
}

 void QVPEllipticArc::bresenham_elliptic_arc(QVector<QPoint>& ellipse, int x, int y, int a, int b)
{

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

}



void QVPEllipticArc::update()
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
    bresenham_elliptic_arc(*m_shapePoints,
                m_center.x(), m_center.y(), m_a, m_b);

    if (m_state == 2 || m_state == 3){
        bresenham_line(*m_shapePoints,
                    m_center.x(), m_center.y(),
                    m_firstPoint.x(), m_firstPoint.y());
    } elif (m_state == 4){
        bresenham_line(*m_shapePoints,
                    m_center.x(), m_center.y(),
                    m_firstPoint.x(), m_firstPoint.y());
        bresenham_line(*m_shapePoints,
                    m_center.x(), m_center.y(),
                    m_lastPoint.x(), m_lastPoint.y());
    }

    if (m_rasterized != nullptr){
        delete m_rasterized;
    }
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, m_width);
}


void QVPEllipticArc::handleMousePressEvent(QMouseEvent * me)
{


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
    if (m_state < 2){
        m_lastPoint = me->pos();
        initEllipseParams();
    } elif (m_state < 4) {
        m_firstPoint = me->pos();
        m_ang1 = angleFromSC(sin(m_firstPoint), cos(m_firstPoint));
        qDebug() << m_ang1;
    } else {
        m_lastPoint = me->pos();
        m_ang2 = angleFromSC(sin(m_lastPoint), cos(m_lastPoint));
        qDebug() << m_ang2;
    }
    update();
}



void QVPEllipticArc::handleMouseReleaseEvent(QMouseEvent * me)
{

    if (m_state < 2){
        m_lastPoint = me->pos();
        initEllipseParams();
    } elif (m_state < 4) {
        m_firstPoint = me->pos();
        m_ang1 = angleFromSC(sin(m_firstPoint), cos(m_firstPoint));
    } else {
        m_lastPoint = me->pos();
    }
    m_mousePressed = false;
    m_state++;
    update();
}

inline float QVPEllipticArc::sin(QPointF point)
{
    volatile float x = point.x() - m_center.x();
    volatile float y = point.y() - m_center.y();
    return y / sqrt(x*x + y*y);
}


inline float QVPEllipticArc::cos(QPointF point)
{
    volatile float x = point.x() - m_center.x();
    volatile float y = point.y() - m_center.y();
    return x / sqrt(x*x + y*y);
}


inline void QVPEllipticArc::initEllipseParams(){
    m_center = QPoint(abs(m_firstPoint.x() + m_lastPoint.x()) / 2, abs(m_firstPoint.y() + m_lastPoint.y()) / 2);
    m_a = abs(m_firstPoint.x() - m_lastPoint.x()) / 2;
    m_b = abs(m_firstPoint.y() - m_lastPoint.y()) / 2;
}

float QVPEllipticArc::getAng2() const
{
    return m_ang2;
}

void QVPEllipticArc::setAng2(float ang2)
{
    m_ang2 = ang2;
    update();
}

float QVPEllipticArc::getAng1() const
{
    return m_ang1;
}

void QVPEllipticArc::setAng1(float ang1)
{
    m_ang1 = ang1;
    update();
}

float QVPEllipticArc::getB() const
{
    return m_b;
}

void QVPEllipticArc::setB(float b)
{
    m_b = b;
    update();
}

float QVPEllipticArc::getA() const
{
    return m_a;
}

void QVPEllipticArc::setA(float a)
{
    m_a = a;
    update();
}

QPointF QVPEllipticArc::getCenter() const
{
    return m_center;
}

void QVPEllipticArc::setCenter(const QPointF &center)
{
    m_center = center;
    update();
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

inline bool QVPEllipticArc::checkPoint(QPointF point){
    float angle = angleFromSC(sin(point), cos(point));

    if (m_ang1 < m_ang2){
        return angle > m_ang1 && angle < m_ang2;
    } elif (m_ang1 > m_ang2) {
        return angle > m_ang1 || angle < m_ang2;
    } else {
        return true;
    }
}

QString QVPEllipticArc::toString()
{
    std::stringstream ss;
    ss << "A;" << m_center.x() << ";" << m_center.y() << ";" <<
          m_a << ";" << m_b << ";" << m_ang1 << ";" << m_ang2 << ";" <<
          QString("%1").arg(((m_penColor.red() * 7 / 255) << 5 |
                              m_penColor.green() * 7 / 255 << 2 |
                              m_penColor.blue() * 3 / 255), 0, 8
                            ).toStdString() << ";" << m_width << "\n";
    return QString::fromStdString(std::string(ss.str()));
}

void QVPEllipticArc::move(QPointF vec)
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

}

QList<QVPShape *> QVPEllipticArc::cutLine(QPointF first, QPointF last)
{

    float x1, x2, y1, y2;
    QList<QVPShape *> newShapes;
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), first.x(), first.y(),
                             last.x(), last.y(), x1, x2, y1, y2)){
        QPointF int1(x1, y1), int2(x2, y2);
        QList<float> angles;

        float ang1 = angleFromSC(sin(int1), cos(int1));
        float ang2 = angleFromSC(sin(int2), cos(int2));

        if (between(int1, first, last) && checkPoint(int1)){

            angles.append(ang1);

        }

        if (between(int2, first, last) && checkPoint(int2)){

            if (angles.empty() || ang2 > ang1 || ((ang1 - ang2) > 2*M_PI)){
                angles.append(ang2);
            } else {
                angles.prepend(ang2);
            }
        }
        angles.prepend(m_ang1);
        angles.append(m_ang2);
        for (int i = 0; i < angles.size() - 1; i++){
            newShapes.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, angles[i], angles[(i + 1) % angles.size()], m_width));
        }

    } else {
        newShapes.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, m_ang1, m_ang2, m_width));
    }

    Q_ASSERT(!newShapes.empty());
    return newShapes;
}
