set(MODULE_DIR chips)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/abstractchip.cpp
    ${MODULE_PATH}/boxchip.cpp
    ${MODULE_PATH}/hubchip.cpp
    ${MODULE_PATH}/interface.cpp
    ${MODULE_PATH}/switchchip.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/abstractchip.h
    ${MODULE_PATH}/boxchip.h
    ${MODULE_PATH}/hubchip.h
    ${MODULE_PATH}/interface.h
    ${MODULE_PATH}/switchchip.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
