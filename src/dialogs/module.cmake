set(MODULE_DIR dialogs)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/aboutwindow.cpp
    ${MODULE_PATH}/adapterproperty.cpp
    ${MODULE_PATH}/computerproperty.cpp
    ${MODULE_PATH}/connectdialog.cpp
    ${MODULE_PATH}/designerdialog.cpp
    ${MODULE_PATH}/devicenotedialog.cpp
    ${MODULE_PATH}/dhcpclientproperty.cpp
    ${MODULE_PATH}/dhcpserverproperty.cpp
    ${MODULE_PATH}/dialogtemplate.cpp
    ${MODULE_PATH}/hubproperty.cpp
    ${MODULE_PATH}/installdialog.cpp
    ${MODULE_PATH}/logdialog.cpp
    ${MODULE_PATH}/programdialog.cpp
    ${MODULE_PATH}/ripproperty.cpp
    ${MODULE_PATH}/routeeditor.cpp
    ${MODULE_PATH}/routerproperty.cpp
    ${MODULE_PATH}/senddialog.cpp
    ${MODULE_PATH}/settingdialog.cpp
    ${MODULE_PATH}/spoofingproperty.cpp
    ${MODULE_PATH}/staticsdialog.cpp
    ${MODULE_PATH}/switchproperty.cpp
    ${MODULE_PATH}/tablearp.cpp
    ${MODULE_PATH}/tableswitch.cpp
    ${MODULE_PATH}/testdialog.cpp
    ${MODULE_PATH}/virtualnetworkdialog.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/aboutwindow.h
    ${MODULE_PATH}/adapterproperty.h
    ${MODULE_PATH}/computerproperty.h
    ${MODULE_PATH}/connectdialog.h
    ${MODULE_PATH}/designerdialog.h
    ${MODULE_PATH}/devicenotedialog.h
    ${MODULE_PATH}/dhcpclientproperty.h
    ${MODULE_PATH}/dhcpserverproperty.h
    ${MODULE_PATH}/dialogtemplate.h
    ${MODULE_PATH}/hubproperty.h
    ${MODULE_PATH}/installdialog.h
    ${MODULE_PATH}/logdialog.h
    ${MODULE_PATH}/programdialog.h
    ${MODULE_PATH}/ripproperty.h
    ${MODULE_PATH}/routeeditor.h
    ${MODULE_PATH}/routerproperty.h
    ${MODULE_PATH}/senddialog.h
    ${MODULE_PATH}/settingdialog.h
    ${MODULE_PATH}/spoofingproperty.h
    ${MODULE_PATH}/staticsdialog.h
    ${MODULE_PATH}/switchproperty.h
    ${MODULE_PATH}/tablearp.h
    ${MODULE_PATH}/tableswitch.h
    ${MODULE_PATH}/testdialog.h
    ${MODULE_PATH}/virtualnetworkdialog.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
