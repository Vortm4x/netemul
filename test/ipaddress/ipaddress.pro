#-------------------------------------------------
#
# Project created by QtCreator 2009-06-28T11:29:53
#
#-------------------------------------------------

QT += testlib
QT -= gui
TARGET = ipaddress
CONFIG  += console
CONFIG  -= app_bundle
TEMPLATE = app

INCLUDEPATH += ../../basicnetlib

LIBS += -L../.. -lbasicnetlib

SOURCES += main.cpp
