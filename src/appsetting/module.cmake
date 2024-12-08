set(MODULE_DIR appsetting)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/appsetting.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/appsetting.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
