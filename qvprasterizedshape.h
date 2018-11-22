

#ifndef QVPRASTERIZEDSHAPE_H
#define QVPRASTERIZEDSHAPE_H
#include <QtWidgets>
class QVPShape;

class QVPRasterizedShape : public QObject
{
    Q_OBJECT
public:
    QVPRasterizedShape();
    QVPRasterizedShape(QVector<QPoint>* points, QColor color, int width, QVector<QPoint>* keyPoints=nullptr);
    void operator ()(QImage * image) const;

private:
    QVector<QPoint>* m_points;
    QPen m_pen;
    QVector<QPoint>* m_keyPoints;
    QPen m_keyPen;

    friend class QVPShape;
};


#endif // QVPRASTERIZEDSHAPE_H
