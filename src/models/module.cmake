set(MODULE_DIR models)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/arpmodel.cpp
    ${MODULE_PATH}/dhcpservermodel.cpp
    ${MODULE_PATH}/routemodel.cpp
    ${MODULE_PATH}/switchmodel.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/arpmodel.h
    ${MODULE_PATH}/dhcpservermodel.h
    ${MODULE_PATH}/routemodel.h
    ${MODULE_PATH}/switchmodel.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
