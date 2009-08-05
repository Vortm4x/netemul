# -------------------------------------------------
# Project created by QtCreator 2009-08-04T23:29:44
# -------------------------------------------------
QT += testlib
QT -= gui
TARGET = ripprogramm
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
DEFINES += __TESTING__
INCLUDEPATH += ../../src \
    ../../src/programms \
    ../../src/models
SOURCES += main.cpp \
    ../../src/programms/ripprogramm.cpp \
    ../../src/programms/programmrep.cpp \
    ../../src/udppacket.cpp \
    smartdevice.cpp \
    ../../src/models/routemodel.cpp \
    ../../src/ippacket.cpp \
    ../../src/ipaddress.cpp
HEADERS += ../../src/programms/ripprogramm.h \
    ../../src/programms/programmrep.h \
    ../../src/udppacket.h \
    smartdevice.h \
    ../../src/models/routemodel.h \
    ../../src/ippacket.h \
    ../../src/ipaddress.h
