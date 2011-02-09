# -------------------------------------------------
# Project created by QtCreator 2009-06-28T00:44:43
# -------------------------------------------------
QT += testlib
QT -= gui
TARGET = mac
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

LIBS += -L../.. -lbasicnetlib

INCLUDEPATH += ../../basicnetlib

SOURCES += main.cpp 
