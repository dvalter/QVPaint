#ifndef QVPDOCUMENT_H
#define QVPDOCUMENT_H

#include <QtWidgets>
#include "qvpshape.h"
#include "config.h"
#include "qvpdot.h"
#include "qvpline.h"
#include "qvpellipse.h"
#include "qvpellipticarc.h"
#include "qvpshapeactions.h"


class QVPDocument : public QWidget {
Q_OBJECT
public:
    explicit QVPDocument(QWidget *parent = nullptr);

    bool saveToFile(const QString &fileName);

    bool loadFromFile(const QString &fileName);

    ~QVPDocument() override {
        for (auto shape : m_shapesList) {
            delete shape;
        }
        delete m_tmpShape;
        delete m_mainImage;
    }

public slots:

    void setEditorMode(QVP::editorMode em);

    void receiveParams(QColor color, int width, QPointF first, QPointF last,
                       qreal a, qreal b, qreal ang1, qreal ang2);

    void acceptParamsClose();

private:
    QList<QVPShape *> m_shapesList;
    QList<QVPShape *> m_selectedShapesList;
    QVPShape *m_tmpShape = nullptr;
    QImage *m_mainImage;
    QVPShapeActions *m_shapeActions = nullptr;
    QVP::editorMode m_currentMode = QVP::drawLine;

    bool previousWasFail = false;

    void unSelect();

    void parseString(QString inStr);

    void updateImage();

    void searchPixel(QPoint);


protected:
    void mousePressEvent(QMouseEvent *me) override;

    void mouseMoveEvent(QMouseEvent *me) override;

    void mouseReleaseEvent(QMouseEvent *me) override;

    void paintEvent(QPaintEvent *event) override;


signals:

    void updateCoord(QPoint coord);

    void shapeSelected(bool state);

    void switchToSelection();

    void sendMsgToUI(QString text, bool isError);

    void showSetupWindow(QWidget *window);

};

#endif // QVPDOCUMENT_H
