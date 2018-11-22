#ifndef QVPEllipticArc_H
#define QVPEllipticArc_H

#include "qvpshape.h"
#include "qvpline.h"
#include "qvpellipse.h"
#include <cmath>

class QVPEllipticArc : public QVPShape {
Q_OBJECT

public:
    explicit QVPEllipticArc(QObject *parent = nullptr);

    QVPEllipticArc(QObject *parent, QColor penColor);

    explicit QVPEllipticArc(QColor penColor);

    QVPEllipticArc(QObject *parent, QColor penColor, QPointF center, qreal a, qreal b, qreal a1, qreal a2,
                   int width = 2);

    void handleMouseMoveEvent(QMouseEvent *me) override;

    void handleMousePressEvent(QMouseEvent *me) override;

    void handleMouseReleaseEvent(QMouseEvent *me) override;

    bool isReady() override {
        return m_state > 4;
    }

    QVPRasterizedShape &getRasterized() override {
        return *m_rasterized;
    }

    QString toString() override;

    void move(QPointF vec) override;

    QList<QVPShape *> cutLine(QPointF first, QPointF last) override;


    QPointF getCenter() const;

    void setCenter(const QPointF &center);

    qreal getA() const;

    void setA(qreal a);

    qreal getB() const;

    void setB(qreal b);

    qreal getAng1() const;

    void setAng1(qreal ang1);

    qreal getAng2() const;

    void setAng2(qreal ang2);

protected:
    void update() override;

private:
    inline bool checkPoint(QPointF point);

    inline qreal sin(QPointF point);

    inline qreal cos(QPointF point);

    inline void appendPoint(QPoint point, QVector<QPoint> &vec);

    void bresenham_elliptic_arc(QVector<QPoint> &ellipse, int x, int y, int a, int b);

    inline void initEllipseParams();

    QPointF m_center;
    qreal m_a;
    qreal m_b;
    QPointF m_firstPoint;
    QPointF m_lastPoint;
    qreal m_ang1 = 0.0;
    qreal m_ang2 = 2 * M_PI;
    int m_state = 0;
};

qreal angleFromSC(qreal sin, qreal cos);

#endif // QVPEllipticArc_H
