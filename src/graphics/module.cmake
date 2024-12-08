set(MODULE_DIR graphics)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/cabledev.cpp
    ${MODULE_PATH}/device.cpp
    ${MODULE_PATH}/insertrect.cpp
    ${MODULE_PATH}/selectrect.cpp
    ${MODULE_PATH}/sendellipse.cpp
    ${MODULE_PATH}/textitem.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/cabledev.h
    ${MODULE_PATH}/device.h
    ${MODULE_PATH}/insertrect.h
    ${MODULE_PATH}/selectrect.h
    ${MODULE_PATH}/sendellipse.h
    ${MODULE_PATH}/textitem.h  
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
