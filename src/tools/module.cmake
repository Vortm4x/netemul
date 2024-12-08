set(MODULE_DIR tools)
set(MODULE_PATH ${SOURCE_PATH}/${MODULE_DIR})

set(SOURCES ${SOURCES}
    ${MODULE_PATH}/abstractsocket.cpp
    ${MODULE_PATH}/scenexmlreader.cpp
    ${MODULE_PATH}/scenexmlwriter.cpp
    ${MODULE_PATH}/socketfactory.cpp
    ${MODULE_PATH}/statistics.cpp
    ${MODULE_PATH}/statisticsscene.cpp
    ${MODULE_PATH}/tcpsocket.cpp
    ${MODULE_PATH}/udpsocket.cpp
)

set(HEADERS ${HEADERS}
    ${MODULE_PATH}/abstractsocket.h
    ${MODULE_PATH}/scenexmlreader.h
    ${MODULE_PATH}/scenexmlwriter.h
    ${MODULE_PATH}/socketfactory.h
    ${MODULE_PATH}/statistics.h
    ${MODULE_PATH}/statisticsscene.h
    ${MODULE_PATH}/tcpsocket.h
    ${MODULE_PATH}/udpsocket.h  
)

set(INCLUDE_DEPS ${INCLUDE_DEPS}
    ${MODULE_PATH}
)
