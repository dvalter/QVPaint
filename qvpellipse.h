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

protected:
    virtual void update() override;

private:
    void drawEllipse(QColor color);
    inline void initEllipseParams();

    QPoint m_center;
    int m_a;
    int m_b;
    QPoint m_firstPoint;
    QPoint m_lastPoint;

    //QLabel* testWGT;
};

QVector<QPoint> bresenham_ellipse(int x, int y, int a, int b);

#endif // QVPEllipse_H
