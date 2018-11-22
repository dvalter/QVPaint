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

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wextra
QMAKE_CXXFLAGS_DEBUG += -fprofile-arcs
QMAKE_CXXFLAGS_DEBUG += -ftest-coverage
QMAKE_CXXFLAGS_DEBUG += -lgcov
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=address
#QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer
#QMAKE_LFLAGS_DEBUG += -fsanitize=address
QMAKE_LFLAGS_DEBUG += -ftest-coverage
QMAKE_LFLAGS_DEBUG += -fprofile-arcs


SOURCES += \
        main.cpp \
        qvpmainwindow.cpp \
    qvpdocument.cpp \
    qvpshape.cpp \
    qvpdot.cpp \
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
    qvpdot.h \
    qvpline.h \
    qvpellipse.h \
    qvpellipticarc.h \
    qvpaction.h \
    qvprasterizedshape.h \
    qvpshapeactions.h

RESOURCES += \
    resource.qrc
