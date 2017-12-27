#ifndef QVPDOCUMENT_H
#define QVPDOCUMENT_H

#include <QtWidgets>
#include <qvpshape.h>
#include "config.h"
#include "qvpdot.h"
#include "qvpline.h"
#include "qvpellipse.h"
#include "qvpellipticarc.h"
#include "qvpshapeactions.h"


class QVPDocument : public QWidget
{
    Q_OBJECT
public:
    explicit QVPDocument(QWidget* parent = nullptr);
    bool saveToFile(const QString& fileName);
    bool loadFromFile(const QString& fileName);

    ~QVPDocument(){
        for (auto shape : m_shapesList){
            delete shape;
        }
        if (m_tmpShape != nullptr){
            delete m_tmpShape;
        }
        delete m_mainImage;
//        if (m_shapeActions != nullptr){
//            delete m_shapeActions;
//        }
    }

public slots:
    void setEditorMode(QVP::editorMode em);
    void receiveParams(QColor color, int width, QPointF first, QPointF last,
                     float a, float b, float ang1, float ang2);
    void acceptParamsClose();

private:
    QList<QVPShape*> m_shapesList;
    QList<QVPShape*> m_selectedShapesList;
    QVPShape* m_tmpShape = nullptr;
    QImage* m_mainImage;
//    QLabel* m_imgLbl;
    qint8 m_counter = 0;
    QVP::editorMode m_currentMode = QVP::drawLine;
    QVPShapeActions* m_shapeActions = nullptr;

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
    void showSetupWindow(QWidget* window);

};

#endif // QVPDOCUMENT_H
