# -------------------------------------------------
# Project created by QtCreator 2009-07-18T22:25:27
# -------------------------------------------------
include(../testing.pri)
QT += testlib
TARGET = boxchip
CONFIG += console
INCLUDEPATH += ../../src
TEMPLATE = app
SOURCES += main.cpp \
    ../../src/chips/boxchip.cpp \
    ../../src/chips/hubchip.cpp \
    ../../src/chips/abstractchip.cpp \
    deviceport.cpp
HEADERS += ../../src/chips/boxchip.h \
    ../../src/chips/hubchip.h \
    ../../src/chips/abstractchip.h \
    deviceport.h
DEFINES += __TESTING__
