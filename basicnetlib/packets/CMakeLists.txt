set(MODULE_SOURCES
    arppacket.cpp
    dhcppacket.cpp
    ippacket.cpp
    tcppacket.cpp
    udppacket.cpp
)

set(MODULE_HEADERS
    arppacket.h
    dhcppacket.h
    ippacket.h
    tcppacket.h
    udppacket.h
)

add_library(
    ${PROJECT_NAME}-packets STATIC
    ${MODULE_SOURCES}
    ${MODULE_HEADERS}
)

target_link_libraries(
    ${PROJECT_NAME}-packets PUBLIC
    Qt5::Core
)

target_include_directories(
    ${PROJECT_NAME}-packets PRIVATE
    ${PROJECT_SOURCE_DIR}
)
