#ifndef QVPEllipticArc_H
#define QVPEllipticArc_H

#include "qvpshape.h"
#include "qvpline.h"
#include "qvpellipse.h"

class QVPEllipticArc : public QVPShape
{
    Q_OBJECT

public:
    explicit QVPEllipticArc(QObject *parent = nullptr);
    QVPEllipticArc(QObject *parent, QColor penColor);
    QVPEllipticArc(QColor penColor);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual bool isReady() override { return m_state > 4; }

protected:
    virtual void update() override;

private:
    void drawEllipse(QColor color);
    void drawLine(QColor color);
    void drawLine(QColor color, QPoint point);
    inline bool checkPoint(QPoint point);

    inline float sin(QPoint point);
    inline float cos(QPoint point);
    inline void appendPoint(QPoint point, QVector<QPoint>& vec);
    QVector<QPoint> bresenham_elliptic_arc(int x, int y, int a, int b);
    inline void initEllipseParams();

    QPoint m_center;
    int m_a;
    int m_b;
    QPoint m_firstPoint;
    QPoint m_lastPoint;
    int m_state = 0;
    float m_ang1;
    float m_ang2;
};

#endif // QVPEllipticArc_H
