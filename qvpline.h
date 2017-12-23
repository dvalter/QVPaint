#ifndef QVPLINE_H
#define QVPLINE_H

#include "qvpshape.h"

class QVPLine : public QVPShape
{
    Q_OBJECT

public:
    explicit QVPLine(QObject *parent = nullptr);
    QVPLine(QObject *parent, QColor penColor);
    QVPLine(QColor penColor);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual QVPRasterizedShape& getRasterized() override;

protected:
    virtual void update() override;

private:
    void drawLine(QColor color);

    QPointF m_firstPoint;
    QPointF m_lastPoint;

    //QLabel* testWGT;
};
QVector<QPoint> bresenham_line(int x1, int y1, int x2, int y2);

#endif // QVPLINE_H
