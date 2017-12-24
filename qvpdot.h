#ifndef QVPDOT_H
#define QVPDOT_H

#include "qvpshape.h"

class QVPDot : public QVPShape
{
    Q_OBJECT

public:
    explicit QVPDot(QObject *parent = nullptr);
    QVPDot(QObject *parent, QColor penColor);
    QVPDot(QColor penColor);
    QVPDot(QObject *parent, QColor penColor, QPointF point, int width=1);
    virtual void handleMouseMoveEvent(QMouseEvent * me) override;
    virtual void handleMousePressEvent(QMouseEvent * me) override;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) override;
    virtual QVPRasterizedShape& getRasterized() override;
    virtual QString toString() override;
    virtual void move(QPointF vec) override;

protected:
    virtual void update() override;

private:

    QPointF m_firstPoint;

};


#endif // QVPDOT_H
