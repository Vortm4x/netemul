#-------------------------------------------------
#
# Project created by QtCreator 2011-02-08T00:44:43
#
#-------------------------------------------------

include(packets/packets.pri)
QT       -= gui

INCLUDEPATH += . \
    packets

TARGET = basicnetlib
TEMPLATE = lib
DESTDIR = ..

DEFINES += BASICNETLIB_LIBRARY

SOURCES += basicnetlib.cpp \
    macaddress.cpp \
    ipaddress.cpp \
    frame.cpp


HEADERS += basicnetlib.h \
    macaddress.h \
    ipaddress.h \
    frame.h \
    basicnetlib_global.h

 # install
 #target.path =
 #sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS basictools.pro
 #sources.path = $$[QT_INSTALL_EXAMPLES]/tools/plugandpaintplugins/basictools
 #INSTALLS += target sources
