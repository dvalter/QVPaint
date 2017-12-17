#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>

namespace QVP
{
    // 150ppi A4 magic numbers
    const int imageHeight = 1754;
    const int imageWigth = 2480;

    // default window params
    const int windowHeight = 400;
    const int windowWidth = 600;

    // QVPaint modes
    enum editorMode{
        drawLine,
        drawDot,
        drawEllipse,
        drawEllipticCurve,
        selectShape
    };

    // default bg color
    const QColor backgroundColor(0xFF, 0xFF, 0xFF, 0xFF); // should be white

    // default pen color
    const QColor penColor(0x00, 0x0, 0x0, 0xFF); // should be black

}
#endif // CONFIG_H
