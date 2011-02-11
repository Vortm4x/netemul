# -------------------------------------------------
# Project created by QtCreator 2009-06-28T01:50:47
# -------------------------------------------------
QT += testlib
TARGET = ippacket
CONFIG += console
TEMPLATE = app

LIBS += -L../.. -lbasicnetlib

INCLUDEPATH += ../../basicnetlib/packets \
    ../../basicnetlib
SOURCES += main.cpp


