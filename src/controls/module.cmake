set(MODULE_DIR controls)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/scenecontrol.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/scenecontrol.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
