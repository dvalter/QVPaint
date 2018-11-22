#include "qvpshapeactions.h"
#include "qvpdocument.h"
#include <math.h>

QColor colorFrom884(quint8 eightBit)
{
    quint8 red8 = eightBit & 224;
    red8 = (red8 >> 5) * 255 / 7;
    quint8 green8 = eightBit & 28;
    green8 = (green8 >> 2) * 255 / 7;
    quint8 blue4 = eightBit & 3;
    blue4 = blue4 * 255 / 3;
    QColor color(red8, green8, blue4, 0xFF);
    return color;
}

inline quint8 colorTo844(QColor color){
    return quint8((color.red() * 7 / 255) << 5 | (color.green() * 7 / 255) << 2 | (color.blue() * 3 / 255));
}

QPixmap pixmapFromColor(QColor color)
{
    QPixmap pixmap(32, 32);
    pixmap.fill(color);
    return pixmap;
}


QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF firstCoord):
    QMainWindow(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton( new QVPColorButton(colorTo844(color)))

{
    init(width, firstCoord);
    m_firstCoordYLbl->setText("Point: Y");
    m_firstCoordXLbl->setText("Point: X");
}
QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF firstCoord, QPointF lastCoord):
    QMainWindow(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton( new QVPColorButton(colorTo844(color)))
{
    init(width, firstCoord);

    m_firstCoordXLbl->setText("First point: X");
    m_firstCoordYLbl->setText("First point: Y");


    m_lastCoordXLbl = new QLabel(this);
    m_lastCoordXLbl->setText("Last point: X");
    m_gridLayout->addWidget(m_lastCoordXLbl, 2, 1);

    m_lastCoordXSb = new QDoubleSpinBox(this);
    m_lastCoordXSb->setRange(0, 20000.0);
    m_lastCoordXSb->setValue(lastCoord.x());
    m_gridLayout->addWidget(m_lastCoordXSb, 2, 2);

    m_lastCoordYLbl = new QLabel(this);
    m_lastCoordYLbl->setText("Last point: Y");
    m_gridLayout->addWidget(m_lastCoordYLbl, 2, 3);

    m_lastCoordYSb = new QDoubleSpinBox(this);
    m_lastCoordYSb->setRange(0, 20000.0);
    m_lastCoordYSb->setValue(lastCoord.y());
    m_gridLayout->addWidget(m_lastCoordYSb, 2, 4);

}
QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF center, qreal paramA, qreal paramB):
    QMainWindow(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton( new QVPColorButton(colorTo844(color)))
{
    init(width, center);

    m_firstCoordXLbl->setText("Center: X");
    m_firstCoordYLbl->setText("Center: Y");

    m_paramALbl = new QLabel(this);
    m_paramALbl->setText("Semi-major axis (a)");
    m_gridLayout->addWidget(m_paramALbl, 2, 1);

    m_paramASb = new QDoubleSpinBox(this);
    m_paramASb->setRange(0, 20000.0);
    m_paramASb->setValue(paramA);
    m_gridLayout->addWidget(m_paramASb, 2, 2);

    m_paramBLbl = new QLabel(this);
    m_paramBLbl->setText("Semi-minor axis (b)");
    m_gridLayout->addWidget(m_paramBLbl, 2, 3);

    m_paramBSb = new QDoubleSpinBox(this);
    m_paramBSb->setRange(0, 20000.0);
    m_paramBSb->setValue(paramB);
    m_gridLayout->addWidget(m_paramBSb, 2, 4);
}


QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF center, qreal paramA, qreal paramB, qreal ang1, qreal ang2):
    QMainWindow(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton( new QVPColorButton(colorTo844(color)))
{
    init(width, center);

    m_firstCoordXLbl->setText("Center: X");
    m_firstCoordYLbl->setText("Center: Y");

    m_paramALbl = new QLabel(this);
    m_paramALbl->setText("Semi-major axis (a)");
    m_gridLayout->addWidget(m_paramALbl, 2, 1);

    m_paramASb = new QDoubleSpinBox(this);
    m_paramASb->setRange(0, 20000.0);
    m_paramASb->setValue(paramA);
    m_gridLayout->addWidget(m_paramASb, 2, 2);

    m_paramBLbl = new QLabel(this);
    m_paramBLbl->setText("Semi-minor axis (b)");
    m_gridLayout->addWidget(m_paramBLbl, 2, 3);

    m_paramBSb = new QDoubleSpinBox(this);
    m_paramBSb->setRange(0, 20000.0);
    m_paramBSb->setValue(paramB);
    m_gridLayout->addWidget(m_paramBSb, 2, 4);

    m_paramAng1Lbl = new QLabel(this);
    m_paramAng1Lbl->setText("First angle");
    m_gridLayout->addWidget(m_paramAng1Lbl, 3, 1);

    m_paramAng1Sb = new QDoubleSpinBox(this);
    m_paramAng1Sb->setRange(-M_PI, M_PI);
    m_paramAng1Sb->setValue(ang1);
    m_gridLayout->addWidget(m_paramAng1Sb, 3, 2);

    m_paramAng2Lbl = new QLabel(this);
    m_paramAng2Lbl->setText("Last angle");
    m_gridLayout->addWidget(m_paramAng2Lbl, 3, 3);

    m_paramAng2Sb = new QDoubleSpinBox(this);
    m_paramAng2Sb->setRange(-M_PI, M_PI);
    m_paramAng2Sb->setValue(ang2);
    m_gridLayout->addWidget(m_paramAng2Sb, 3, 4);
}

void QVPShapeActions::init(int width, QPointF firstCoord)
{
    m_cancelButton->setText("&Cancel");
    m_confirmButton->setText("&Confirm");
    m_gridLayout->addWidget(m_confirmButton, 100, 3);
    m_gridLayout->addWidget(m_cancelButton, 100, 4);
    qDebug() << firstCoord;

    m_colorLbl->setText("Color:");
    m_gridLayout->addWidget(m_colorLbl, 50, 1);
    m_gridLayout->addWidget(m_colorButton, 50, 2);
    connect(m_colorButton, &QPushButton::clicked, this, &QVPShapeActions::showColorGrid);
    connect(m_colorSelectionWidget, &QVPColorGridWidget::closed, this, &QVPShapeActions::hideColorGrid);

    QGridLayout* grid = new QGridLayout(m_colorSelectionWidget);
    for (quint8 i = 0; i < 4; i++) {
        for (quint8 j = 0; j < 8; j++) {
            for (quint8 k = 0; k < 8; k++) {

                auto clr844 = quint8((k << 5) | (j << 2) | i);
                QVPColorButton* btn = new QVPColorButton(quint8((k << 5) | (j << 2) | i));
                btn->setCheckable(true);
                if (colorTo844(m_color) == clr844){
                    btn->setChecked(true);
                } else {
                    btn->setChecked(false);
                }
                int y = (j*(1 - 2*(i >> 1))%8 + 16 *  (i >> 1));
                int x = (k*(1 - 2*(i &  1))%8 + 16 * (i & 1));
                grid->addWidget(btn, x, y);
                m_colorGridButtons << btn;
                connect(btn, &QVPColorButton::changeColor, this, &QVPShapeActions::setColor);
            }
        }
    }

    m_colorCancelButton->setText("&Cancel");
    m_colorConfirmButton->setText("&Confirm");
    grid->addWidget(m_colorConfirmButton, 17, 11, 1, 3);
    grid->addWidget(m_colorCancelButton, 17, 14, 1, 3);
    connect(m_colorConfirmButton, &QPushButton::clicked, this, &QVPShapeActions::saveColorGrid);
    connect(m_colorCancelButton, &QPushButton::clicked, this, &QVPShapeActions::hideColorGrid);

    m_colorSelectionWidget->setLayout(grid);
    m_colorSelectionWidget->setWindowTitle("Colors");

    m_mainWgt->setLayout(m_gridLayout);
    setCentralWidget(m_mainWgt);
    setWindowTitle("Shape preferences");

    m_firstCoordXLbl->setText("Default: X");
    m_gridLayout->addWidget(m_firstCoordXLbl, 1, 1);

    m_firstCoordXSb->setRange(0, 20000.0);
    m_firstCoordXSb->setValue(firstCoord.x());
    m_gridLayout->addWidget(m_firstCoordXSb, 1, 2);

    m_firstCoordYLbl->setText("Default: Y");
    m_gridLayout->addWidget(m_firstCoordYLbl, 1, 3);

    m_firstCoordYSb->setRange(0, 20000.0);
    m_gridLayout->addWidget(m_firstCoordYSb, 1, 4);
    m_firstCoordYSb->setValue(firstCoord.y());

    m_widthLbl->setText("Width");
    m_gridLayout->addWidget(m_widthLbl, 50, 3);

    m_widthSb->setRange(1, 20);
    m_widthSb->setValue(width);
    m_gridLayout->addWidget(m_widthSb, 50, 4);

    connect(m_cancelButton, &QPushButton::clicked, this, &QVPShapeActions::cancel);
    connect(m_confirmButton, &QPushButton::clicked, this, &QVPShapeActions::save);

    qDebug() << "x=" << m_firstCoordXSb->value() << " y=" << m_firstCoordYSb->value();

}

void QVPShapeActions::showColorGrid()
{
    m_colorConsistency = false;
    m_colorSelectionWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    m_colorSelectionWidget->show();
}

void QVPShapeActions::hideColorGrid()
{
    qDebug() << "hiding collor grid";
    m_colorConsistency = true;
    m_colorSelectionWidget->hide();
    m_newColor = m_color;
    quint8 color = colorTo844(m_color);

    for (auto btn : m_colorGridButtons){
        if (btn->getColor() != color){
            btn->setChecked(false);
        } else {
            btn->setChecked(true);
        }
    }
}

void QVPShapeActions::saveColorGrid()
{
    qDebug() << "saving collor grid";
    m_color = m_newColor;
    m_colorConsistency = true;
    m_colorSelectionWidget->hide();
    m_colorButton->setIcon(pixmapFromColor(m_color));
}

void QVPShapeActions::setColor(quint8 color)
{
    m_newColor = colorFrom884(color);
    for (auto btn : m_colorGridButtons){
        if (btn->getColor() != color){
            btn->setChecked(false);
        }
    }
}

QVPColorButton::QVPColorButton(quint8 color) : m_color(color)
{
    setFlat(true);
    setIcon(QIcon(pixmapFromColor(colorFrom884(m_color))));
    connect(this, &QPushButton::toggled, this, &QVPColorButton::checkButton);
}

void QVPShapeActions::save()
{
    QPointF first(m_firstCoordXSb->value(), m_firstCoordYSb->value());
    QPointF last;
    if(m_lastCoordYSb && m_lastCoordXSb){
        last = QPointF(double(m_lastCoordXSb->value()), double(m_lastCoordYSb->value()));
    }
    hide();
    emit updateShape(m_color, m_widthSb->value(), first, last,
                     m_paramASb ? m_paramASb->value() : 1.0,
                     m_paramBSb ? m_paramBSb->value() : 1.0,
                     m_paramAng1Sb ? m_paramAng1Sb->value() : 1.0,
                     m_paramAng2Sb ? m_paramAng2Sb->value() : 1.0);
}

void QVPShapeActions::cancel()
{
    hide();
    emit finished();
}
