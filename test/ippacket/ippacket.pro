# -------------------------------------------------
# Project created by QtCreator 2009-06-28T01:50:47
# -------------------------------------------------
QT += testlib
TARGET = ippacket
CONFIG += console
TEMPLATE = app
INCLUDEPATH += ../../src/packets \
    ../../src
SOURCES += main.cpp \
    ../../src/packets/ippacket.cpp \
    ../../src/ipaddress.cpp \
    ../../src/packets/udppacket.cpp \
    ../../src/packets/tcppacket.cpp
HEADERS += ../../src/packets/ippacket.h \
    ../../src/ipaddress.h \
   ../../src/packets/udppacket.h \
  ../../src/packets/tcppacket.h

