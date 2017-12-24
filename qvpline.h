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
    QVPLine(QObject *parent, QColor penColor, QPointF first, QPointF last, int width=1);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual QVPRasterizedShape& getRasterized() override;
    virtual QString toString() override;
    virtual void move(QPointF vec) override;

    const QPointF& getFirst() { return m_firstPoint; }
    const QPointF& getLast() { return m_lastPoint; }

protected:
    virtual void update() override;

private:

    QPointF m_firstPoint;
    QPointF m_lastPoint;

};
//QVector<QPoint> bresenham_line(int x1, int y1, int x2, int y2);
void bresenham_line(QVector<QPoint>& line, int x1, int y1, int x2, int y2);

#endif // QVPLINE_H
