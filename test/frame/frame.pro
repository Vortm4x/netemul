INCLUDEPATH += ../../src/packets \
    ../../src
SOURCES += main.cpp \
    ../../src/frame.cpp \
    ../../src/macaddress.cpp \
    ../../src/packets/ippacket.cpp \
    ../../src/ipaddress.cpp \
    ../../src/packets/arppacket.cpp \
    ../../src/packets/udppacket.cpp \
    ../../src/packets/tcppacket.cpp
HEADERS += ../../src/frame.h \
    ../../src/macaddress.h \
    ../../src/packets/ippacket.h \
    ../../src/ipaddress.h \
    ../../src/packets/arppacket.h \
    ../../src/packets/udppacket.h \
    ../../src/packets/tcppacket.h
TARGET = frame
TEMPLATE = app
QT += testlib
QT -= gui
