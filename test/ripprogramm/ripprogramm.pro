# -------------------------------------------------
# Project created by QtCreator 2009-08-04T23:29:44
# -------------------------------------------------
include(../testing.pri)
QT += testlib \
    script
TARGET = ripprogramm
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
DEFINES += __TESTING__
SOURCES += main.cpp
