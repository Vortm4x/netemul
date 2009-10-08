# -------------------------------------------------
# Project created by QtCreator 2009-07-18T22:25:27
# -------------------------------------------------
QT += testlib
QT -= gui
TARGET = boxchip
CONFIG += console
INCLUDEPATH += ../../src \
    ../../src/tools
TEMPLATE = app
SOURCES += main.cpp \
    ../../src/chips/boxchip.cpp \
    ../../src/chips/hubchip.cpp \
    ../../src/tools/statistics.cpp \
    ../../src/chips/abstractchip.cpp \
    deviceport.cpp \
    ../../src/macaddress.cpp \
    ../../src/ipaddress.cpp
HEADERS += ../../src/chips/boxchip.h \
    ../../src/chips/hubchip.h \
    ../../src/tools/statistics.h \
    ../../src/chips/abstractchip.h \
    deviceport.h \
    ../../src/macaddress.h \
    ../../src/ipaddress.h
DEFINES += __TESTING__
