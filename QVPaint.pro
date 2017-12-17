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
    qvpaction.cpp

HEADERS += \
        qvpmainwindow.h \
    qvpdocument.h \
    qvpshape.h \
    config.h \
    qvpline.h \
    qvpaction.h

RESOURCES += \
    resource.qrc
