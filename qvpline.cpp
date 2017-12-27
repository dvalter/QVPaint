#include "qvpline.h"
#include <string.h>
#include <sstream>
#include <cmath>

static const double Epsilon = 1e-10;

bool IsZero(double d)
{
    return abs(d) < Epsilon;
}



class Vector
{
public:
    double X;
    double Y;

    // Constructors.
    Vector(double x, double y) : X(x), Y(y) {}
    Vector() : X(NAN), Y(NAN) {}
    Vector(QPointF point) : X(point.x()), Y(point.y()) {}

    QPointF toQPointF(){
        return QPointF(X, Y);
    }

    double Cross(Vector v)
    {
        return X * v.Y - Y * v.X;
    }

    bool Equals(Vector obj)
    {
        return IsZero(X - obj.X) && IsZero(Y - obj.Y);
    }
};



Vector operator -(Vector v, Vector w)
{
    return Vector(v.X - w.X, v.Y - w.Y);
}

Vector operator +(Vector v, Vector w)
{
    return Vector(v.X + w.X, v.Y + w.Y);
}

double operator *(Vector v, Vector w)
{
    return v.X * w.X + v.Y * w.Y;
}

Vector operator *(Vector v, double mult)
{
    return Vector(v.X * mult, v.Y * mult);
}

Vector operator *(double mult, Vector v)
{
    return Vector(v.X * mult, v.Y * mult);
}



static bool lineSegementsIntersect(Vector p, Vector p2, Vector q, Vector q2,
    Vector& intersection, bool considerCollinearOverlapAsIntersect = false)
{
    intersection = Vector();

    Vector r = p2 - p;
    Vector s = q2 - q;
    double rxs = r.Cross(s);
    double qpxr = (q - p).Cross(r);

    // If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
    if (IsZero(rxs) && IsZero(qpxr))
    {
        // 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
        // then the two lines are overlapping,
        if (considerCollinearOverlapAsIntersect)
            if ((0 <= (q - p)*r && (q - p)*r <= r*r) || (0 <= (p - q)*s && (p - q)*s <= s*s))
                return true;

        // 2. If neither 0 <= (q - p) * r = r * r nor 0 <= (p - q) * s <= s * s
        // then the two lines are collinear but disjoint.
        // No need to implement this expression, as it follows from the expression above.
        return false;
    }

    // 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
    if (IsZero(rxs) && !IsZero(qpxr))
        return false;

    // t = (q - p) x s / (r x s)
    auto t = (q - p).Cross(s)/rxs;

    // u = (q - p) x r / (r x s)

    auto u = (q - p).Cross(r)/rxs;

    // 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
    // the two line segments meet at the point p + t r = q + u s.
    if (!IsZero(rxs) && (0 <= t && t <= 1) && (0 <= u && u <= 1))
    {
        // We can calculate the intersection point using either t or u.
        intersection = p + t*r;

        // An intersection was found.
        return true;
    }

    // 5. Otherwise, the two line segments are not parallel but do not intersect.
    return false;
}

QVPLine::QVPLine(QColor penColor, bool rectangle):
    QVPShape(nullptr, penColor),
    m_rectangle(rectangle)
{

}

QVPLine::QVPLine(QObject * parent, QColor penColor, bool rectangle):
    QVPShape(parent, penColor),
    m_rectangle(rectangle)
{

}

QVPLine::QVPLine(QObject * parent, bool rectangle):
    QVPShape(parent, QVP::penColor),
    m_rectangle(rectangle)
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
//    qDebug() << "(" << x1 << "," << y1 << ")->(" << x2 << "," << y2 << ")";
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
    if (!m_rectangle){
        bresenham_line(*m_shapePoints,
                m_firstPoint.x(), m_firstPoint.y(),
                m_lastPoint.x(), m_lastPoint.y());
    } else {
        bresenham_line(*m_shapePoints,
                m_firstPoint.x(), m_firstPoint.y(),
                m_lastPoint.x(), m_firstPoint.y());
        bresenham_line(*m_shapePoints,
                m_lastPoint.x(), m_firstPoint.y(),
                m_lastPoint.x(), m_lastPoint.y());
        bresenham_line(*m_shapePoints,
                m_lastPoint.x(), m_lastPoint.y(),
                m_firstPoint.x(), m_lastPoint.y());
        bresenham_line(*m_shapePoints,
                m_firstPoint.x(), m_lastPoint.y(),
                m_firstPoint.x(), m_firstPoint.y());
    }

    if (m_rasterized){
        delete m_rasterized;
    }
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, m_width);
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
          QString("%1").arg(((m_penColor.red() * 7 / 255) << 5 |
                              m_penColor.green() * 7 / 255 << 2 |
                              m_penColor.blue() * 3 / 255), 0, 8
                            ).toStdString() << ";" << m_width << "\n";
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

QList<QVPShape *> QVPLine::cutLine(QPointF first, QPointF last)
{
    Vector result;
    QList<QVPShape *> newShapes;
    if (lineSegementsIntersect(first, last, m_firstPoint, m_lastPoint, result)){
        QPointF res = result.toQPointF();
        qDebug() << first << "->" << last << " and " << m_firstPoint
                 << "->" << m_lastPoint << " X => " << res;
        newShapes.append(new QVPLine(parent(), m_penColor, m_firstPoint, res, m_width));
        newShapes.append(new QVPLine(parent(), m_penColor, res, m_lastPoint, m_width));
        m_lastPoint = res;
    } else {
        newShapes.append(new QVPLine(parent(), m_penColor, m_firstPoint, m_lastPoint, m_width));
    }
    return newShapes;
}
