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
    QVPEllipse(QObject *parent, QColor penColor, QPointF center, float a, float b, int width=1);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual QVPRasterizedShape& getRasterized() { return *m_rasterized; }
    virtual QString toString() override;
    virtual void move(QPointF vec) override;
    virtual QList<QVPShape *> cutLine(QPointF first, QPointF last) override;
    virtual QList<QVPShape *> cutRect(QPointF first, QPointF last) override { return QList<QVPShape *>();}

protected:
    virtual void update() override;

private:
    inline void initEllipseParams();
    inline bool checkPoint(QPoint point);

    inline float sin(QPointF point);
    inline float cos(QPointF point);

    QPointF m_center;
    int m_a;
    int m_b;
    QPointF m_firstPoint;
    QPointF m_lastPoint;

    //QLabel* testWGT;
};

void bresenham_ellipse(QVector<QPoint> &ellipse, int x, int y, int a, int b);

#endif // QVPEllipse_H
