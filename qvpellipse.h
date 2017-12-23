#ifndef QVPEllipse_H
#define QVPEllipse_H

#include "qvpshape.h"

class QVPEllipse : public QVPShape
{
    Q_OBJECT

public:
    explicit QVPEllipse(QObject *parent = nullptr);
    QVPEllipse(QObject *parent, QColor penColor);
    QVPEllipse(QColor penColor);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual QVPRasterizedShape& getRasterized() {return *(new QVPRasterizedShape());}

protected:
    virtual void update() override;

private:
    void drawEllipse(QColor color);
    inline void initEllipseParams();

    QPointF m_center;
    int m_a;
    int m_b;
    QPointF m_firstPoint;
    QPointF m_lastPoint;

    //QLabel* testWGT;
};

QVector<QPoint> bresenham_ellipse(int x, int y, int a, int b);

#endif // QVPEllipse_H
