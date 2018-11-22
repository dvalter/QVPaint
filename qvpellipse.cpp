#include "qvpellipse.h"
#include <sstream>
#include "qvpellipticarc.h"

static const double Epsilon = 2;


QVPEllipse::QVPEllipse(QColor penColor) :
        QVPShape(nullptr, std::move(penColor)) {

}

QVPEllipse::QVPEllipse(QObject *parent, QColor penColor) :
        QVPShape(parent, std::move(penColor)) {

}

QVPEllipse::QVPEllipse(QObject *parent) :
        QVPShape(parent, QVP::penColor) {

}

QVPEllipse::QVPEllipse(QObject *parent, QColor penColor, QPointF center,
                       qreal a, qreal b, int width) :
        QVPShape(parent, std::move(penColor), width),
        m_center(center),
        m_a(int(a)),
        m_b(int(b)) {
    update();
}


void bresenham_ellipse(QVector<QPoint> &ellipse, int x, int y, int a, int b) {
    if (!a && !b) {
        ellipse.append(QPoint(x, y));
        return;
    }

    long b_square = b * b;
    long a_square = a * a;
    int row = b;
    int col = 0;
    long two_a_square = a_square * 2;
    long four_a_square = a_square * 4;
    long four_b_square = b_square * 4;
    long two_b_square = b_square * 2;
    long d = two_a_square * ((row - 1) * (row)) + a_square + two_b_square * (1 - a_square);
    while (a_square * (row) > b_square * (col)) {
        ellipse.append(QPoint(col + x, row + y));
        ellipse.append(QPoint(col + x, y - row));
        ellipse.append(QPoint(x - col, row + y));
        ellipse.append(QPoint(x - col, y - row));
        if (d >= 0) {
            row--;
            d -= four_a_square * (row);
        }
        d += two_b_square * (3 + (col * 2));
        col++;
    }
    d = two_b_square * (col + 1) * col + two_a_square *
                                         (row * (row - 2) + 1) + (1 - two_a_square) * b_square;
    while ((row) + 1) {
        ellipse.append(QPoint(col + x, row + y));
        ellipse.append(QPoint(col + x, y - row));
        ellipse.append(QPoint(x - col, row + y));
        ellipse.append(QPoint(x - col, y - row));
        if (d <= 0) {
            col++;
            d += four_b_square * col;
        }
        row--;
        d += two_a_square * (3 - (row * 2));
    }

}


void QVPEllipse::update() {
    QColor color;
    if (m_selected) {
        color = QColor(0xFF - m_penColor.red(), 0xFF - m_penColor.green(), 0xFF - m_penColor.blue(), 0xFF);
        if (color == QVP::backgroundColor) {
            color = QColor(0xFF, 0x0, 0x0, 0xFF); // should be red
        }
        qDebug() << color;
    } else {
        color = m_penColor;
    }

    m_shapePoints->clear();
    bresenham_ellipse(*m_shapePoints,
                      int(m_center.x()), int(m_center.y()), m_a, m_b);

    delete m_rasterized;
    m_rasterized = new QVPRasterizedShape(m_shapePoints, color, m_width);
}


void QVPEllipse::handleMousePressEvent(QMouseEvent *me) {
    m_firstPoint = me->pos();
    m_lastPoint = me->pos();
    m_center = m_firstPoint;
    m_a = 0;
    m_b = 0;
    m_mousePressed = true;
    update();
}

void QVPEllipse::handleMouseMoveEvent(QMouseEvent *me) {
    m_lastPoint = me->pos();
    initEllipseParams();
    update();
}


void QVPEllipse::handleMouseReleaseEvent(QMouseEvent *me) {
    m_lastPoint = me->pos();
    initEllipseParams();
    m_mousePressed = false;
    update();
}

inline void QVPEllipse::initEllipseParams() {
    m_center = QPoint(int(abs(m_firstPoint.x() + m_lastPoint.x()) / 2),
                      int(abs(m_firstPoint.y() + m_lastPoint.y()) / 2));
    m_a = int(abs(m_firstPoint.x() - m_lastPoint.x()) / 2);
    m_b = int(abs(m_firstPoint.y() - m_lastPoint.y()) / 2);
}

inline qreal QVPEllipse::sin(QPointF point) {
    volatile qreal x = point.x() - m_center.x();
    volatile qreal y = point.y() - m_center.y();
    return y / sqrt(x * x + y * y);
}


inline qreal QVPEllipse::cos(QPointF point) {
    volatile qreal x = point.x() - m_center.x();
    volatile qreal y = point.y() - m_center.y();
    return x / sqrt(x * x + y * y);
}

int QVPEllipse::getB() const {
    return m_b;
}

void QVPEllipse::setB(int b) {
    m_b = b;
    update();
}

int QVPEllipse::getA() const {
    return m_a;
}

void QVPEllipse::setA(int a) {
    m_a = a;
    update();
}

QPointF QVPEllipse::getCenter() const {
    return m_center;
}

void QVPEllipse::setCenter(const QPointF &center) {
    m_center = center;
    update();
}


QString QVPEllipse::toString() {
    std::stringstream ss;
    ss << "E;" << m_center.x() << ";" << m_center.y() << ";" <<
       m_a << ";" << m_b << ";" <<
       QString("%1").arg(((m_penColor.red() * 7u / 255) << 5 |
                          m_penColor.green() * 7u / 255 << 2 |
                          m_penColor.blue() * 3u / 255), 0, 8
       ).toStdString() << ";" << m_width << "\n";
    return QString::fromStdString(std::string(ss.str()));
}

void QVPEllipse::move(QPointF vec) {
    m_center.rx() += vec.x();
    m_center.ry() += vec.y();
    update();
}

inline void swap(volatile qreal &a, volatile qreal &b) {
    volatile qreal tmp = a;
    a = b;
    b = tmp;
}

inline void swap(qreal &a, qreal &b) {
    volatile qreal tmp = a;
    a = b;
    b = tmp;
}

static bool ellipseIntersectLine(volatile qreal a, volatile qreal b, volatile qreal h, volatile qreal k,
                                 volatile qreal x1, volatile qreal y1, volatile qreal x2, volatile qreal y2,
                                 qreal &xi1, qreal &xi2, qreal &yi1, qreal &yi2) {

    bool inverted = false;
    if (abs(x1 - x2) < Epsilon) {
        swap(a, b);
        swap(h, k);
        swap(x1, y1);
        swap(x2, y2);
        inverted = true;
        qDebug() << "inverted";
    }
    volatile qreal aa, bb, cc, m = 0;
    //
    if (x1 != x2) {
        m = (y2 - y1) / (x2 - x1);
        qreal c = y1 - m * x1;
        //
        aa = b * b + a * a * m * m;
        bb = 2 * a * a * c * m - 2 * a * a * k * m - 2 * h * b * b;
        cc = b * b * h * h + a * a * c * c - 2 * a * a * k * c + a * a * k * k - a * a * b * b;
    } else {
        //
        // vertical line case
        //
        aa = a * a;
        bb = -2.0 * k * a * a;
        cc = -a * a * b * b + b * b * (x1 - h) * (x1 - h);
    }

    qreal d = bb * bb - 4 * aa * cc;
    //
    // intersection points : (xi1,yi1) and (xi2,yi2)
    //
    if (d > 0.0) {
        if (x1 != x2) {
            xi1 = (-bb + sqrt(d)) / (2 * aa);
            xi2 = (-bb - sqrt(d)) / (2 * aa);
            yi1 = y1 + m * (xi1 - x1);
            yi2 = y1 + m * (xi2 - x1);
        } else {
            yi1 = (-bb + sqrt(d)) / (2 * aa);
            yi2 = (-bb - sqrt(d)) / (2 * aa);
            xi1 = x1;
            xi2 = x1;
        }
    } else {
        return false;    // no intersections
    }
    if (inverted) {
        swap(xi1, yi1);
        swap(xi2, yi2);
    }
    return true;
}

inline qreal min(const qreal &a, const qreal &b) {
    return a < b ? a : b;
}

inline qreal max(const qreal &a, const qreal &b) {
    return a > b ? a : b;
}

inline bool between(const QPointF &a, const QPointF &first, const QPointF &last) {
    bool m = a.x() <= max(first.x(), last.x());
    bool n = a.y() <= max(first.y(), last.y());
    bool o = a.x() >= min(first.x(), last.x());
    bool p = a.y() >= min(first.y(), last.y());
    return m && n && o && p;
}

QList<QVPShape *> QVPEllipse::cutLine(QPointF first, QPointF last) {

    qreal x1, x2, y1, y2;
    QList<QVPShape *> newShapes;
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), first.x(), first.y(),
                             last.x(), last.y(), x1, x2, y1, y2)) {
        QPointF int1(x1, y1), int2(x2, y2);
        qDebug() << int1 << " " << between(int1, first, last) << " " << int2 << " "
                 << between(int2, first, last)/* << int1==int2*/;
        if (between(int1, first, last) && between(int2, first, last)) {
            volatile qreal ang1 = angleFromSC(sin(int1), cos(int1));
            volatile qreal ang2 = angleFromSC(sin(int2), cos(int2));
            newShapes.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, ang1, ang2, m_width));
            newShapes.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, ang2, ang1, m_width));
        }
    }
    return newShapes;
}

QList<QVPShape *> QVPEllipse::cutRect(QPointF first, QPointF last) {
    QList<QVPShape *> result;
    QVector<qreal> angles;

    qreal x1, x2, y1, y2;
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), first.x(), first.y(),
                             last.x(), first.y(), x1, x2, y1, y2)) {
        QPointF int1(x1, y1), int2(x2, y2);
        if (between(int1, first, last)) {
            qreal ang = angleFromSC(sin(int1), cos(int1));
            angles.append(ang);
        }
        if (between(int2, first, last)) {
            qreal ang = angleFromSC(sin(int2), cos(int2));
            angles.append(ang);
        }
    }
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), last.x(), first.y(),
                             last.x(), last.y(), x1, x2, y1, y2)) {
        QPointF int1(x1, y1), int2(x2, y2);
        if (between(int1, first, last)) {
            qreal ang = angleFromSC(sin(int1), cos(int1));
            angles.append(ang);
        }
        if (between(int2, first, last)) {
            qreal ang = angleFromSC(sin(int2), cos(int2));
            angles.append(ang);
        }
    }
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), last.x(), last.y(),
                             first.x(), last.y(), x1, x2, y1, y2)) {
        QPointF int1(x1, y1), int2(x2, y2);
        if (between(int1, first, last)) {
            qreal ang = angleFromSC(sin(int1), cos(int1));
            angles.append(ang);
        }
        if (between(int2, first, last)) {
            qreal ang = angleFromSC(sin(int2), cos(int2));
            angles.append(ang);
        }
    }
    if (ellipseIntersectLine(m_a, m_b, m_center.x(), m_center.y(), first.x(), last.y(),
                             first.x(), first.y(), x1, x2, y1, y2)) {
        QPointF int1(x1, y1), int2(x2, y2);
        if (between(int1, first, last)) {
            qreal ang = angleFromSC(sin(int1), cos(int1));
            angles.append(ang);
        }
        if (between(int2, first, last)) {
            qreal ang = angleFromSC(sin(int2), cos(int2));
            angles.append(ang);
        }
    }
    if (angles.size() > 1) {
        std::sort(angles.begin(), angles.end());
        for (int i = 0; i < angles.length(); i++) {
            result.append(new QVPEllipticArc(parent(), m_penColor, m_center, m_a, m_b, angles[i],
                                             angles[(i + 1) % angles.size()], m_width));
        }
    }

    return result;
}
