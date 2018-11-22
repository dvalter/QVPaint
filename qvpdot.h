#ifndef QVPDOT_H
#define QVPDOT_H

#include "qvpshape.h"

class QVPDot : public QVPShape {
Q_OBJECT

public:
    explicit QVPDot(QObject *parent = nullptr);

    QVPDot(QObject *parent, QColor penColor);

    explicit QVPDot(QColor penColor);

    QVPDot(QObject *parent, QColor penColor, QPointF point, int width = 2);

    void handleMouseMoveEvent(QMouseEvent *me) override;

    void handleMousePressEvent(QMouseEvent *me) override;

    void handleMouseReleaseEvent(QMouseEvent *me) override;

    QVPRasterizedShape &getRasterized() override;

    QString toString() override;

    void move(QPointF vec) override;

    QList<QVPShape *> cutLine(QPointF first, QPointF last) override;

    QPointF getCenter() {
        return m_firstPoint;
    }

    QPointF getCenter() const;

    void setCenter(const QPointF &firstPoint);

protected:
    void update() override;

private:

    QPointF m_firstPoint;

};


#endif // QVPDOT_H
