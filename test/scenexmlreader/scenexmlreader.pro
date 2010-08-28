#-------------------------------------------------
#
# Project created by QtCreator 2010-08-28T22:02:49
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = scenexmlreader
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src/tools

SOURCES += tst_scenexmlreadertest.cpp \
    ../../src/tools/scenexmlreader.cpp \
    mycanvas.cpp \
    classfactory.cpp \
    device.cpp \
    deviceimpl.cpp \
    computer.cpp \
    interface.cpp

HEADERS += \
    ../../src/tools/scenexmlreader.h \
    mycanvas.h \
    classfactory.h \
    device.h \
    deviceimpl.h \
    computer.h \
    interface.h
