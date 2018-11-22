#ifndef QVPSHAPEACTIONS_H
#define QVPSHAPEACTIONS_H

#include <QtWidgets>
#include <config.h>
#include "qvpshape.h"
#include <limits>

class QVPColorGridWidget : public QWidget {
Q_OBJECT
public:
    explicit QVPColorGridWidget(QWidget *parent = nullptr) : QWidget(parent) {
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        emit closed();
        event->ignore();
    }

signals:

    void closed();
};


class QVPColorButton : public QPushButton {
Q_OBJECT

public:
    explicit QVPColorButton(quint8 color);

    quint8 getColor() {
        return m_color;
    }

    ~QVPColorButton() override = default;

signals:

    void changeColor(quint8 color);

private slots:

    void checkButton(bool state) {
        if (state) {
            emit changeColor(m_color);
        }
    }

private:
    quint8 m_color;

};

class QVPShapeActions : public QMainWindow {
Q_OBJECT
public:
    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF firstCoord);

    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF firstCoord, QPointF lastCoord);

    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF center, qreal paramA, qreal paramB);

    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF center, qreal paramA, qreal paramB, qreal ang1, qreal ang2);


    ~QVPShapeActions() override {
        delete m_colorButton;
        qDebug() << __FUNCTION__;
    }

signals:

    void finished();

    void updateShape(QColor color, int width, QPointF first, QPointF last,
                     qreal a, qreal b, qreal ang1, qreal ang2);

public slots:

    void showColorGrid();

    void setColor(quint8 color);

    void hideColorGrid();

    void saveColorGrid();

    void cancel();

    void save();

protected:
    void closeEvent(QCloseEvent *event) override {
        event->ignore();
        cancel();
    }

private:


    void init(int width,
              QPointF firstCoord);

    QWidget *m_mainWgt = new QWidget(this);

    QVP::shapeType m_shapeType;
    QGridLayout *m_gridLayout = new QGridLayout(this);

    QLabel *m_firstCoordXLbl = new QLabel(this);
    QDoubleSpinBox *m_firstCoordXSb = new QDoubleSpinBox(this);
    QLabel *m_firstCoordYLbl = new QLabel(this);
    QDoubleSpinBox *m_firstCoordYSb = new QDoubleSpinBox(this);

    QLabel *m_lastCoordXLbl = nullptr;
    QDoubleSpinBox *m_lastCoordXSb = nullptr;
    QLabel *m_lastCoordYLbl = nullptr;
    QDoubleSpinBox *m_lastCoordYSb = nullptr;

    QLabel *m_paramALbl = nullptr;
    QDoubleSpinBox *m_paramASb = nullptr;
    QLabel *m_paramBLbl = nullptr;
    QDoubleSpinBox *m_paramBSb = nullptr;

    QLabel *m_paramAng1Lbl = nullptr;
    QDoubleSpinBox *m_paramAng1Sb = nullptr;
    QLabel *m_paramAng2Lbl = nullptr;
    QDoubleSpinBox *m_paramAng2Sb = nullptr;

    QLabel *m_widthLbl = new QLabel(this);
    QSpinBox *m_widthSb = new QSpinBox(this);

    QLabel *m_colorLbl = new QLabel(this);
    QColor m_color;
    QColor m_newColor;

    QVPColorButton *m_colorButton;
    bool m_colorConsistency = true;
    QList<QVPColorButton *> m_colorGridButtons;
    QVPColorGridWidget *m_colorSelectionWidget = new QVPColorGridWidget(this);

    QPushButton *m_confirmButton = new QPushButton(this);
    QPushButton *m_cancelButton = new QPushButton(this);

    QPushButton *m_colorConfirmButton = new QPushButton(this);
    QPushButton *m_colorCancelButton = new QPushButton(this);

};


#endif // QVPSHAPEACTIONS_H
