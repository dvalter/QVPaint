#ifndef QVPLINE_H
#define QVPLINE_H

#include "qvpshape.h"

class QVPLine : public QVPShape
{
    Q_OBJECT

public:
    explicit QVPLine(QObject *parent = nullptr, bool rectangle = false);
    QVPLine(QObject *parent, QColor penColor, bool rectangle = false);
    QVPLine(QColor penColor, bool rectangle = false);
    QVPLine(QObject *parent, QColor penColor, QPointF first, QPointF last, int width=2);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual QVPRasterizedShape& getRasterized() override;
    virtual QString toString() override;
    virtual void move(QPointF vec) override;
    virtual QList<QVPShape *> cutLine(QPointF first, QPointF last) override;

    QPointF getFirst() const;
    QPointF getLast() const;

    void setFirst(const QPointF &firstPoint);
    void setLast(const QPointF &lastPoint);











protected:
    virtual void update() override;

private:

    QPointF m_firstPoint;
    QPointF m_lastPoint;
    bool m_rectangle = false;

};
//QVector<QPoint> bresenham_line(int x1, int y1, int x2, int y2);
void bresenham_line(QVector<QPoint>& line, int x1, int y1, int x2, int y2);

#endif // QVPLINE_H
