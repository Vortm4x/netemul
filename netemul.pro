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
include(src/states/states.pri)
include(src/commands/commands.pri)
include(src/delegats/delegats.pri)
include(src/interfaces/interfaces.pri)
include(src/factories/factories.pri)
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
    src/tools \
    src/states \
    src/commands \
    src/delegats \
    src/interfaces \
    src/factories
QT += script xml

WARNING += -Wall

unix {
    QMAKE_POST_LINK += cd ipedit && qmake && make && cd ..
}

# Default translations path
TRANSLATIONS_PATH = "translation"
DOC_PATH = "doc"
SCRIPT_PATH = "scripts"
win32 { 
    DESTDIR = ../trunk
#    message("Static link ...")
#    DEFINES += QT_NO_DEBUG_OUTPUT
#    CONFIG += static
}
macx:message("You have MACX...")
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

INCLUDEPATH += basicnetlib \
    basicnetlib/packets
LIBS += -L. -lbasicnetlib

# Input
HEADERS += src/deviceport.h \
    src/ipedit.h \
    src/mainwindow.h \
    src/mycanvas.h
SOURCES += src/deviceport.cpp \
    src/ipedit.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mycanvas.cpp
RESOURCES += netemul.qrc
TRANSLATIONS += translation/netemul_ru.ts \
    translation/netemul_pt_BR.ts \
    translation/netemul_es.ts
unix { 
    CONFIG += debug
    
    # Prefix: base instalation directory
    isEmpty( PREFIX ):PREFIX = /usr/local
    DEB_BUILD = $$system(echo \$DEB_BUILD_OPTIONS)
    contains(DEB_BUILD, nostrip):QMAKE_STRIP = :
    DEFINES += PREFIX=\\\"$${PREFIX}\\\"
    target.path = $${PREFIX}/bin
    INSTALLS = target
    
    # Default path for translations in *nix
    TRANSLATIONS_PATH = $${PREFIX}/share/netemul/translation/
    
    # Translations
    translations.path = $${TRANSLATIONS_PATH}
    translations.files = ../translation/netemul_es.qm \
        ../translation/netemul_pt_BR.qm \
        ../translation/netemul_ru.qm \
        ../translation/qt_es.qm \
        ../translation/qt_pt.qm \
        ../translation/qt_ru.qm
    INSTALLS += translations
    
    # Icons
    icons.path = $${PREFIX}/share/netemul/pixmap/
    icons.files = ../images/NetEmul.png
    icons.files = ../images/NetEmul_32x32.png
    INSTALLS += icons
    
    # Desktop
    desktop.path = /usr/share/applications/
    desktop.files = ./netemul.desktop
    INSTALLS += desktop
    
    # Docs$${PREFIX}/share/netemul/
    docs.path = $${PREFIX}/share/netemul/
    docs.files = ../doc
    INSTALLS += docs
    DOC_PATH = $${PREFIX}/share/netemul/doc/
    
    # Scripts
    scripts.path = $${PREFIX}/share/netemul/
    scripts.files = ../scripts
    INSTALLS += scripts
    SCRIPT_PATH = $${PREFIX}/share/netemul/scripts/
}

# Let the variable be available for compiling
DEFINES += DOC_PATH=\\\"$${DOC_PATH}\\\"
DEFINES += TRANSLATIONS_PATH=\\\"$${TRANSLATIONS_PATH}\\\"
DEFINES += SCRIPT_PATH=\\\"$${SCRIPT_PATH}\\\"
