set(MODULE_DIR delegates)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/ipaddressdelegate.cpp
    ${MODULE_PATH}/macaddressdelegate.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/ipaddressdelegate.h
    ${MODULE_PATH}/macaddressdelegate.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
