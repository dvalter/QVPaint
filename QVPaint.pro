#-------------------------------------------------
#
# Project created by QtCreator 2017-12-06T13:21:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QVPaint
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14
QMAKE_CXXFLAGS += -std=gnu++14


SOURCES += \
        main.cpp \
        qvpmainwindow.cpp \
    qvpdocument.cpp \
    qvpshape.cpp \
    qvpline.cpp \
    qvpellipse.cpp \
    qvpellipticarc.cpp \
    qvpaction.cpp \
    qvprasterizedshape.cpp \
    qvpshapeactions.cpp

HEADERS += \
        qvpmainwindow.h \
    qvpdocument.h \
    qvpshape.h \
    config.h \
    qvpline.h \
    qvpellipse.h \
    qvpellipticarc.h \
    qvpaction.h \
    qvprasterizedshape.h \
    qvpshapeactions.h

RESOURCES += \
    resource.qrc
