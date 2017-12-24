#ifndef QVPEllipticArc_H
#define QVPEllipticArc_H

#include "qvpshape.h"
#include "qvpline.h"
#include "qvpellipse.h"
#include <math.h>

class QVPEllipticArc : public QVPShape
{
    Q_OBJECT

public:
    explicit QVPEllipticArc(QObject *parent = nullptr);
    QVPEllipticArc(QObject *parent, QColor penColor);
    QVPEllipticArc(QColor penColor);
    QVPEllipticArc(QObject *parent, QColor penColor, QPointF center, float a, float b, float a1, float a2);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual bool isReady() override { return m_state > 4; }
    virtual QVPRasterizedShape& getRasterized() {return *(new QVPRasterizedShape());}
    virtual QString toString() override;

protected:
    virtual void update() override;

private:
    void drawEllipse(QColor color);
    void drawLine(QColor color);
    void drawLine(QColor color, QPointF point);
    inline bool checkPoint(QPoint point);

    inline float sin(QPointF point);
    inline float cos(QPointF point);
    inline void appendPoint(QPoint point, QVector<QPoint>& vec);
    QVector<QPoint> bresenham_elliptic_arc(int x, int y, int a, int b);
    inline void initEllipseParams();

    QPointF m_center;
    float m_a;
    float m_b;
    QPointF m_firstPoint;
    QPointF m_lastPoint;
    float m_ang1;
    float m_ang2;
    int m_state = 0;
};

#endif // QVPEllipticArc_H
