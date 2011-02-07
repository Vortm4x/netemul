#-------------------------------------------------
#
# Project created by QtCreator 2010-12-05T22:29:47
#
#-------------------------------------------------

QT       += script testlib xml

TARGET = tst_mycanvastest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src \
../../src/tools

SOURCES += tst_mycanvastest.cpp \
    ../../src/mycanvas.cpp \
    textitem.cpp \
    cabledev.cpp \
    device.cpp \
    appsetting.cpp \
    abstractstate.cpp \
    deletecommand.cpp \
    addcablecommand.cpp \
    ../../src/tools/scenexmlwriter.cpp \
    ../../src/tools/scenexmlreader.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../src/mycanvas.h \
    textitem.h \
    cabledev.h \
    device.h \
    appsetting.h \
    abstractstate.h \
    deletecommand.h \
    addcablecommand.h \
    ../../src/tools/scenexmlwriter.h \
    ../../src/tools/scenexmlreader.h
