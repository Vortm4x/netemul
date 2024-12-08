set(MODULE_DIR states)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/abstractstate.cpp
    ${MODULE_PATH}/cablestate.cpp
    ${MODULE_PATH}/emptystate.cpp
    ${MODULE_PATH}/insertstate.cpp
    ${MODULE_PATH}/movestate.cpp
    ${MODULE_PATH}/sendstate.cpp
    ${MODULE_PATH}/textstate.cpp    
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/abstractstate.h
    ${MODULE_PATH}/cablestate.h
    ${MODULE_PATH}/emptystate.h
    ${MODULE_PATH}/insertstate.h
    ${MODULE_PATH}/movestate.h
    ${MODULE_PATH}/sendstate.h
    ${MODULE_PATH}/textstate.h
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
