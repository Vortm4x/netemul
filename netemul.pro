# #####################################################################
# Automatically generated by qmake (2.01a) Wed Jun 17 17:34:00 2009
# #####################################################################
TEMPLATE = app
TARGET = netemul
DEPENDPATH += . \
    src \
    ui
INCLUDEPATH += . \
    src
QT += script
win32 { 
    message("Static link ...")
    CONFIG += static \
        console
}
CONFIG += warn_on
contains(QT_CONFIG, opengl) { 
    message("OpenGL connected ...")
    QT += opengl
}
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = src

# Input
HEADERS += src/abstractchip.h \
    src/adapterproperty.h \
    src/arppacket.h \
    src/boxdevice.h \
    src/cabledev.h \
    src/computer.h \
    src/computerproperty.h \
    src/connectdialog.h \
    src/device.h \
    src/deviceport.h \
    src/dialogtemplate.h \
    src/frame.h \
    src/hubdevice.h \
    src/hubproperty.h \
    src/interface.h \
    src/interfacedialog.h \
    src/ipaddress.h \
    src/ipedit.h \
    src/ippacket.h \
    src/macaddress.h \
    src/mainwindow.h \
    src/mycanvas.h \
    src/routeeditor.h \
    src/routerdevice.h \
    src/routerproperty.h \
    src/senddialog.h \
    src/settingdialog.h \
    src/sharebus.h \
    src/smartdevice.h \
    src/switchdevice.h \
    src/switchproperty.h \
    src/tableswitch.h \
    src/testdialog.h \
    src/tablearp.h
FORMS += ui/testdialog.ui \
    ui/connectdialog.ui \
    ui/tableswitch.ui
SOURCES += src/abstractchip.cpp \
    src/adapterproperty.cpp \
    src/arppacket.cpp \
    src/boxdevice.cpp \
    src/cabledev.cpp \
    src/computer.cpp \
    src/computerproperty.cpp \
    src/connectdialog.cpp \
    src/device.cpp \
    src/deviceport.cpp \
    src/dialogtemplate.cpp \
    src/frame.cpp \
    src/hubdevice.cpp \
    src/hubproperty.cpp \
    src/interface.cpp \
    src/interfacedialog.cpp \
    src/ipaddress.cpp \
    src/ipedit.cpp \
    src/ippacket.cpp \
    src/macaddress.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mycanvas.cpp \
    src/routeeditor.cpp \
    src/routerdevice.cpp \
    src/routerproperty.cpp \
    src/senddialog.cpp \
    src/settingdialog.cpp \
    src/sharebus.cpp \
    src/smartdevice.cpp \
    src/switchdevice.cpp \
    src/switchproperty.cpp \
    src/tableswitch.cpp \
    src/testdialog.cpp \
    src/tablearp.cpp
RESOURCES += netemul.qrc
OTHER_FILES += test/README.txt
