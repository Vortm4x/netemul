set(MODULE_DIR factories)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/classfactory.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/classfactory.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
