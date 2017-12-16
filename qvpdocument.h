#ifndef QVPDOCUMENT_H
#define QVPDOCUMENT_H

#include <QtWidgets>
#include <qvpshape.h>
#include "config.h"

class QVPDocument : public QLabel
{
    Q_OBJECT
public:
    explicit QVPDocument(QWidget *parent = nullptr);

private:
    QList<QVPShape *> shapes;
    QImage* m_mainImage;
    QLabel* m_imgLbl;
    qint8 m_counter = 0;


protected:
    virtual void mousePressEvent(QMouseEvent* me) override;
    virtual void mouseMoveEvent(QMouseEvent* me) override;
    virtual void mouseReleaseEvent(QMouseEvent* me) override;
    virtual void paintEvent(QPaintEvent* event) override;

signals:

public slots:
};

#endif // QVPDOCUMENT_H
