#ifndef QVPSHAPE_H
#define QVPSHAPE_H

#include <QObject>
#include <QtWidgets>
#include "config.h"
#include "qvprasterizedshape.h"

class QVPShape : public QObject
{
    Q_OBJECT
public:
    explicit QVPShape(QObject *parent = nullptr, QColor penColor = QVP::penColor);
    const QImage& getImage();
    virtual void handleMousePressEvent(QMouseEvent * me) = 0;
    virtual void handleMouseMoveEvent(QMouseEvent * me) = 0;
    virtual void handleMouseReleaseEvent(QMouseEvent * me) = 0;
    virtual bool isReady() { return !m_mousePressed; }
    void select(bool state);
    QVector<QPoint> * getPoints() { return m_shapePoints; }
    int testPoint(QPoint point);
    virtual QVPRasterizedShape& getRasterized() = 0;
    virtual QString toString() = 0;

signals:

public slots:



protected:

    virtual void update() = 0;

    QVector<QPoint> * m_shapePoints = new QVector<QPoint>;
    QColor m_penColor;
//    QImage* m_image;
    bool m_mousePressed = false;
    bool m_selected = false;
    QVPRasterizedShape* m_rasterized = new QVPRasterizedShape;


};

#endif // QVPSHAPE_H
