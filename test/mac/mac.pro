# -------------------------------------------------
# Project created by QtCreator 2009-06-28T00:44:43
# -------------------------------------------------
include(../testing.pri)
QT += testlib
QT -= gui
TARGET = mac
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

# HEADERS += ../../src/macaddress.h
SOURCES += main.cpp \
    ../../src/macaddress.cpp
HEADERS += ../../src/macaddress.h
