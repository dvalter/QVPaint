#ifndef QVPSHAPE_H
#define QVPSHAPE_H

#include <QObject>
#include <QtWidgets>
#include "config.h"

class QVPShape : public QObject
{
    Q_OBJECT
public:
    explicit QVPShape(QObject *parent = nullptr);
    const QImage& getImage();
    virtual void handleMousePressEvent(QMouseEvent * me) = 0;
    virtual void handleMouseMoveEvent(QMouseEvent * me) = 0;

signals:

public slots:



protected:

    QImage* m_image;
    bool m_mouserPressed = false;



};

#endif // QVPSHAPE_H
