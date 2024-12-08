set(MODULE_DIR commands)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/addcablecommand.cpp
    ${MODULE_PATH}/addcommand.cpp
    ${MODULE_PATH}/addtextcommand.cpp
    ${MODULE_PATH}/deletecommand.cpp
    ${MODULE_PATH}/movecommand.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/addcablecommand.h
    ${MODULE_PATH}/addcommand.h
    ${MODULE_PATH}/addtextcommand.h
    ${MODULE_PATH}/deletecommand.h
    ${MODULE_PATH}/movecommand.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
