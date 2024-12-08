set(MODULE_DIR programs)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/dhcpclientprogram.cpp
    ${MODULE_PATH}/dhcpdaemon.cpp
    ${MODULE_PATH}/dhcpserverprogram.cpp
    ${MODULE_PATH}/program.cpp
    ${MODULE_PATH}/ripprogram.cpp
    ${MODULE_PATH}/spoofingprogram.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/dhcpclientprogram.h
    ${MODULE_PATH}/dhcpdaemon.h
    ${MODULE_PATH}/dhcpserverprogram.h
    ${MODULE_PATH}/program.h
    ${MODULE_PATH}/ripprogram.h
    ${MODULE_PATH}/spoofingprogram.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
