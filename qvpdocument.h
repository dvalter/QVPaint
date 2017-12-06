#ifndef QVPDOCUMENT_H
#define QVPDOCUMENT_H
#include <QtCore>


class QVPDocument : public QWidget
{
    Q_OBJECT
public:
    explicit QVPDocument(QWidget *parent = nullptr);

private:
    QList<QVPShape> shapes;

signals:

public slots:
};

#endif // QVPDOCUMENT_H
