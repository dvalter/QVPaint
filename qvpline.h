#ifndef QVPLINE_H
#define QVPLINE_H

#include "qvpshape.h"

class QVPLine : public QVPShape {
Q_OBJECT

public:
    explicit QVPLine(QObject *parent = nullptr, bool rectangle = false);

    QVPLine(QObject *parent, QColor penColor, bool rectangle = false);

    explicit QVPLine(QColor penColor, bool rectangle = false);

    QVPLine(QObject *parent, QColor penColor, QPointF first, QPointF last, int width = 2);

    void handleMouseMoveEvent(QMouseEvent *me) override;

    void handleMousePressEvent(QMouseEvent *me) override;

    void handleMouseReleaseEvent(QMouseEvent *me) override;

    QVPRasterizedShape &getRasterized() override;

    QString toString() override;

    void move(QPointF vec) override;

    QList<QVPShape *> cutLine(QPointF first, QPointF last) override;

    QPointF getFirst() const;

    QPointF getLast() const;

    void setFirst(const QPointF &firstPoint);

    void setLast(const QPointF &lastPoint);

protected:
    void update() override;

private:

    QPointF m_firstPoint;
    QPointF m_lastPoint;
    bool m_rectangle = false;

};

void bresenham_line(QVector<QPoint> &line, int x1, int y1, int x2, int y2);

#endif // QVPLINE_H
