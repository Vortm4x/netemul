#-------------------------------------------------
#
# Project created by QtCreator 2009-10-15T19:50:16
#
#-------------------------------------------------

CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib
QTDIR_build:DESTDIR     = $$QT_BUILD_TREE/plugins/designer
INCLUDEPATH += "../src"
HEADERS     = ../src/ipedit.h \
              ipeditdesignerplugin.h
SOURCES     = ../src/ipedit.cpp \
              ipeditdesignerplugin.cpp
# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/customwidgetplugin
INSTALLS += target sources
