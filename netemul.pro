# #####################################################################
# Automatically generated by qmake (2.01a) Wed Jun 17 17:34:00 2009
# #####################################################################
include(src/chips/chips.pri)
include(src/programms/programms.pri)
include(src/controls/controls.pri)
include(src/devices/devices.pri)
include(src/models/models.pri)
include(src/dialogs/dialogs.pri)
include(src/other/other.pri)
include(src/graphics/graphics.pri)
include(src/forms/forms.pri)
include(src/tools/tools.pri)
TEMPLATE = app
TARGET = netemul
DEPENDPATH += . \
    src \
    ui \
    src/chips
INCLUDEPATH += . \
    src \
    src/chips \
    src/controls \
    src/programms \
    src/devices \
    src/models \
    src/dialogs \
    src/other \
    src/graphics \
    src/tools
QT += script
win32 { 
    DESTDIR = ../trunk
    message("Static link ...")
    DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG += static
}
CONFIG += warn_on
contains(QT_CONFIG, opengl) { 
    message("OpenGL connected ...")
    QT += opengl
}
debug:contains(QT_CONFIG, scripttools) { 
    message("Debugger connected ...")
    QT += scripttools
}
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = src

# Input
HEADERS += src/arppacket.h \
    src/deviceport.h \
    src/frame.h \
    src/ipaddress.h \
    src/ipedit.h \
    src/ippacket.h \
    src/macaddress.h \
    src/mainwindow.h \
    src/mycanvas.h \
    src/udppacket.h \
    src/tcppacket.h \
    src/tcpsocket.h
SOURCES += src/arppacket.cpp \
    src/deviceport.cpp \
    src/frame.cpp \
    src/ipaddress.cpp \
    src/ipedit.cpp \
    src/ippacket.cpp \
    src/macaddress.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mycanvas.cpp \
    src/udppacket.cpp \
    src/tcppacket.cpp \
    src/tcpsocket.cpp
RESOURCES += netemul.qrc
TRANSLATIONS += translation/netemul_ru.ts \
    translation/netemul_pt_BR.ts
