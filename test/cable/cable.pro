#-------------------------------------------------
#
# Project created by QtCreator 2010-08-29T20:51:32
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_cabletest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_cabletest.cpp \
    ../../src/graphics/cabledev.cpp \
    device.cpp \
    deviceport.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../src/graphics/cabledev.h \
    device.h \
    deviceport.h
