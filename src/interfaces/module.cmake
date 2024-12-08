set(MODULE_DIR interfaces)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/visualizable.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/visualizable.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
