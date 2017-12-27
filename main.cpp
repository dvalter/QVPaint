#include "qvpmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    try { /* just to catch any exception in code below */

        QApplication::setDesktopSettingsAware(true);
        QApplication a(argc, argv);
        QVPMainWindow w;
        w.show();

        return a.exec();

    } catch (std::exception e){
        return 123;
    }
}
