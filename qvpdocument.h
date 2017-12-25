#ifndef QVPDOCUMENT_H
#define QVPDOCUMENT_H

#include <QtWidgets>
#include <qvpshape.h>
#include "config.h"
#include "qvpdot.h"
#include "qvpline.h"
#include "qvpellipse.h"
#include "qvpellipticarc.h"


class QVPDocument : public QLabel
{
    Q_OBJECT
public:
    explicit QVPDocument(QWidget* parent = nullptr);
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName);

public slots:
    void setEditorMode(QVP::editorMode em);

private:
    QList<QVPShape*> m_shapesList;
    QList<QVPShape*> m_selectedShapesList;
    QVPShape* m_tmpShape = nullptr;
    QImage* m_mainImage;
    QLabel* m_imgLbl;
    qint8 m_counter = 0;
    QVP::editorMode m_currentMode = QVP::drawLine;

    bool previousWasFail = false;

    void unSelect();
    void parseString(QString inStr);
    void updateImage();
    void searchPixel(QPoint);
//    int checkPixel(QPoint);


protected:
    virtual void mousePressEvent(QMouseEvent* me) override;
    virtual void mouseMoveEvent(QMouseEvent* me) override;
    virtual void mouseReleaseEvent(QMouseEvent* me) override;
    virtual void paintEvent(QPaintEvent* event) override;

signals:
    void updateCoord(QPoint coord);
    void shapeSelected(bool state);
    void switchToSelection();
    void sendMsgToUI(QString text, bool isError);

};

#endif // QVPDOCUMENT_H
