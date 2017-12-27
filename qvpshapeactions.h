#ifndef QVPSHAPEACTIONS_H
#define QVPSHAPEACTIONS_H

#include <QtWidgets>
#include <config.h>
#include "qvpshape.h"
#include <limits>

class QVPColorButton: public QPushButton
{
    Q_OBJECT

public:
    QVPColorButton(QObject * parent, quint8 color);
    quint8 getColor() { return m_color; }

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

class QVPShapeActions : public QWidget
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


signals:

public slots:
    void showColorGrid();
    void setColor(quint8 color);
private:

    void init(int width,
              QPointF firstCoord);

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
    QList<QPushButton *> m_colorGridButtons;
    QWidget m_colorSelectionWidget;

};




#endif // QVPSHAPEACTIONS_H
