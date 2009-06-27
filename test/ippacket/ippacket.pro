#-------------------------------------------------
#
# Project created by QtCreator 2009-06-28T01:50:47
#
#-------------------------------------------------

QT += testlib

QT -= gui

TARGET = ippacket
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
SOURCES += main.cpp \
            ../../src/ippacket.cpp \
            ../../src/ipaddress.cpp
HEADERS += ../../src/ippacket.h \
            ../../src/ipaddress.h
INCLUDEPATH += ../../src

