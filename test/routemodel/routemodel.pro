# -------------------------------------------------
# Project created by QtCreator 2009-08-04T16:11:22
# -------------------------------------------------
QT -= gui
QT += testlib
TARGET = routemodel
CONFIG += console
INCLUDEPATH += ../../src/models \
                ../../src
TEMPLATE = app
SOURCES += main.cpp \
    ../../src/models/routemodel.cpp \
    ../../src/ipaddress.cpp
HEADERS += ../../src/models/routemodel.h \
    ../../src/ipaddress.h
