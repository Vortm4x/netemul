set(MODULE_DIR devices)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/boxdevice.cpp
    ${MODULE_PATH}/computer.cpp
    ${MODULE_PATH}/deviceimpl.cpp
    ${MODULE_PATH}/hubdevice.cpp
    ${MODULE_PATH}/routerdevice.cpp
    ${MODULE_PATH}/sharebus.cpp
    ${MODULE_PATH}/smartdevice.cpp
    ${MODULE_PATH}/switchdevice.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/boxdevice.h
    ${MODULE_PATH}/computer.h
    ${MODULE_PATH}/deviceimpl.h
    ${MODULE_PATH}/hubdevice.h
    ${MODULE_PATH}/routerdevice.h
    ${MODULE_PATH}/sharebus.h
    ${MODULE_PATH}/smartdevice.h
    ${MODULE_PATH}/switchdevice.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
