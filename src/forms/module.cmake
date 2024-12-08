set(MODULE_DIR forms)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(UIS
    ${MODULE_PATH}/aboutwindow.ui
    ${MODULE_PATH}/connectdialog.ui
    ${MODULE_PATH}/devicenotedialog.ui
    ${MODULE_PATH}/dhcpclientproperty.ui
    ${MODULE_PATH}/dhcpserverproperty.ui
    ${MODULE_PATH}/installdialog.ui
    ${MODULE_PATH}/logdialog.ui
    ${MODULE_PATH}/programdialog.ui
    ${MODULE_PATH}/ripproperty.ui
    ${MODULE_PATH}/settingdialog.ui
    ${MODULE_PATH}/spoofingproperty.ui
    ${MODULE_PATH}/staticsdialog.ui
    ${MODULE_PATH}/tableswitch.ui
    ${MODULE_PATH}/testdialog.ui
    ${MODULE_PATH}/virtualnetworkdialog.ui
)

qt5_wrap_ui(FORMS ${UIS})

set(HEADERS ${HEADERS}
    ${FORMS}
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${CMAKE_BINARY_DIR}
)
