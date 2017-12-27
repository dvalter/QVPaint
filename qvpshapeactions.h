#ifndef QVPSHAPEACTIONS_H
#define QVPSHAPEACTIONS_H

#include <QtWidgets>
#include <config.h>
#include "qvpshape.h"
#include <limits>

class QVPColorGridWidget: public QWidget
{
    Q_OBJECT
public:
    explicit QVPColorGridWidget(QWidget* parent = nullptr): QWidget(parent) {}

protected:
    void closeEvent(QCloseEvent *event) {
        emit closed();
        event->ignore();
    }

signals:
    void closed();
};


class QVPColorButton: public QPushButton
{
    Q_OBJECT

public:
    QVPColorButton(quint8 color);
    quint8 getColor() { return m_color; }

    virtual ~QVPColorButton(){
        qDebug() << __FUNCTION__;
    }

signals:
    void changeColor(quint8 color);

private slots:
    void checkButton(bool state)
    {
        if(state) {
            emit changeColor(m_color);
        }
    }

private:
    quint8 m_color;

};

class QVPShapeActions : public QMainWindow/*QWidget*/
{
    Q_OBJECT
public:
    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF firstCoord);
    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF firstCoord, QPointF lastCoord);
    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF center, float paramA, float paramB);
    QVPShapeActions(QWidget *parent, QVP::shapeType, QColor color, int width,
                    QPointF center, float paramA, float paramB, float ang1, float ang2);



    virtual ~QVPShapeActions(){
        qDebug() << __FUNCTION__;
    }

signals:

public slots:
    void showColorGrid();
    void setColor(quint8 color);
    void hideColorGrid();
    void saveColorGrid();
private:

    void init(int width,
              QPointF firstCoord);

    QWidget m_mainWgt;

    QVP::shapeType m_shapeType;
    QGridLayout m_gridLayout;

    QLabel m_firstCoordXLbl;
    QDoubleSpinBox m_firstCoordXSb;
    QLabel m_firstCoordYLbl;
    QDoubleSpinBox m_firstCoordYSb;

    QLabel m_lastCoordXLbl;
    QDoubleSpinBox m_lastCoordXSb;
    QLabel m_lastCoordYLbl;
    QDoubleSpinBox m_lastCoordYSb;

    QLabel m_paramALbl;
    QDoubleSpinBox m_paramASb;
    QLabel m_paramBLbl;
    QDoubleSpinBox m_paramBSb;

    QLabel m_paramAng1Lbl;
    QDoubleSpinBox m_paramAng1Sb;
    QLabel m_paramAng2Lbl;
    QDoubleSpinBox m_paramAng2Sb;

    QLabel m_widthLbl;
    QSpinBox m_widthSb;

//    QHBoxLayout m_colorBox;
    QLabel m_colorLbl;
    QColor m_color;
    QColor m_newColor;

    QVPColorButton m_colorButton;
    bool m_colorConsistency = true;
    QList<QVPColorButton *> m_colorGridButtons;
    QVPColorGridWidget m_colorSelectionWidget;

    QPushButton m_confirmButton;
    QPushButton m_cancelButton;

    QPushButton m_colorConfirmButton;
    QPushButton m_colorCancelButton;



};




#endif // QVPSHAPEACTIONS_H
