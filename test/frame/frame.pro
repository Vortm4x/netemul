SOURCES += main.cpp \
    ../../src/frame.cpp \
    ../../src/macaddress.cpp \
    ../../src/ippacket.cpp \
    ../../src/ipaddress.cpp \
    ../../src/arppacket.cpp
HEADERS += ../../src/frame.h \
    ../../src/macaddress.h \
    ../../src/ippacket.h \
    ../../src/ipaddress.h \
    ../../src/arppacket.h
TARGET = frame
TEMPLATE = app
QT += testlib
