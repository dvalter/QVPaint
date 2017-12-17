#ifndef QVPLINE_H
#define QVPLINE_H

#include "qvpshape.h"

class QVPLine : public QVPShape
{
public:
    explicit QVPLine(QObject *parent = nullptr);
    QVPLine(QObject *parent, QColor penColor);
    QVPLine(QColor penColor);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;

protected:
    virtual void update() override;

private:
    void drawLine(QColor color);

    QPoint m_firstPoint;
    QPoint m_lastPoint;

    QLabel* testWGT;
};

#endif // QVPLINE_H
