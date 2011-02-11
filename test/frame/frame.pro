INCLUDEPATH += ../../basicnetlib/packets \
    ../../basicnetlib
SOURCES += main.cpp

LIBS += -L../.. -lbasicnetlib

TARGET = frame
TEMPLATE = app
QT += testlib
QT -= gui
