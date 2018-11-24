#ifndef CONFIG_H
#define CONFIG_H

#include <QColor>

namespace QVP {
    // 150ppi A4 magic numbers
    const int imageHeight = 1754;
    const int imageWigth = 2480;

    // QVPaint modes
    enum editorMode : quint8 {
        selectShape,
        drawDot,
        drawLine,
        drawEllipse,
        drawEllipticCurve,
        move,
        makeOrtho,
        crossLine,
        clipRectangle,
        setUp
    };

    enum shapeType : quint8 {
        point,
        line,
        ellipse,
        ellipticArc
    };

    // default bg color
    const QColor backgroundColor(0xFF, 0xFF, 0xFF, 0xFF); // should be white

    // default pen color
    const QColor penColor(0x00, 0x0, 0x0, 0xFF); // should be black

    const int searchWidth = 6;
}

#define elif else if
#endif // CONFIG_H
