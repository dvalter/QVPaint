#include "qvpshapeactions.h"
#include "qvpdocument.h"

QColor colorFrom884(quint8 eightBit)
{
    quint8 red8 = eightBit & 224;
    red8 = (red8 >> 5) * 255 / 7;
    quint8 green8 = eightBit & 28;
    green8 = (green8 >> 2) * 255 / 7;
    quint8 blue4 = eightBit & 3;
    blue4 = blue4 * 255 / 3;
    QColor color(red8, green8, blue4, 0xFF);
//    qDebug() << QString("%1").arg(eightBit, 8, 2) <<   " red=" << red8 << " green=" <<
//                green8 << " blue=" << blue4 << " " << color;
    return color;
}

inline quint8 colorTo844(QColor color){
    return (color.red() * 7 / 255) << 5 | (color.green() * 7 / 255) << 2 | (color.blue() * 3 / 255);
}

QPixmap pixmapFromColor(QColor color)
{
    QPixmap pixmap(32, 32);
    pixmap.fill(color);
    return pixmap;
}


QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF firstCoord):
    QWidget(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton(nullptr, colorTo844(color))

{
    init(width, firstCoord);
}
QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF firstCoord, QPointF lastCoord):
    QWidget(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton(nullptr, colorTo844(color))
{
    init(width, firstCoord);
}
QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF center, float paramA, float paramB):
    QWidget(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton(nullptr, colorTo844(color))
{
    init(width, center);
}
QVPShapeActions::QVPShapeActions(QWidget *parent, QVP::shapeType type, QColor color, int width,
                QPointF center, float paramA, float paramB, float ang1, float ang2):
    QWidget(parent),
    m_shapeType(type),
    m_color(color),
    m_newColor(color),
    m_colorButton(nullptr, colorTo844(color))
{
    init(width, center);
}

void QVPShapeActions::init(int width, QPointF firstCoord)
{
    m_colorLbl.setText("Color:");
    m_gridLayout.addWidget(&m_colorLbl, 0, 0);
    m_gridLayout.addWidget(&m_colorButton, 0, 1);
    connect(&m_colorButton, &QPushButton::clicked, this, &QVPShapeActions::showColorGrid);

    QGridLayout* grid = new QGridLayout(&m_colorSelectionWidget);
    for (quint8 i = 0; i < 4; i++) {
        for (quint8 j = 0; j < 8; j++) {
            for (quint8 k = 0; k < 8; k++) {

                quint8 clr844 = (k << 5) | (j << 2) | i;
                QVPColorButton* btn = new QVPColorButton(nullptr, (k << 5) | (j << 2) | i);
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
    m_colorSelectionWidget.setLayout(grid);
    setLayout(&m_gridLayout);
}

void QVPShapeActions::showColorGrid()
{
    m_colorConsistency = false;
    m_colorSelectionWidget.show();
}

void QVPShapeActions::setColor(quint8 color)
{
    m_newColor = colorFrom884(color);
}

QVPColorButton::QVPColorButton(QObject * parent, quint8 color) : m_color(color)
{
    setFlat(true);
    setIcon(QIcon(pixmapFromColor(colorFrom884(m_color))));
    connect(this, &QPushButton::toggled, this, &QVPColorButton::checkButton);
}
