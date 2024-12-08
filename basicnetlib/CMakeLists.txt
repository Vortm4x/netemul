cmake_minimum_required(VERSION 3.19)

project(basicnetlib LANGUAGES CXX)

find_package(
    Qt5 REQUIRED COMPONENTS
    Core
)

set(SOURCES
    basicnetlib.cpp
    frame.cpp
    ipaddress.cpp
    macaddress.cpp
)

set(HEADERS
    basicnetlib_global.h
    basicnetlib.h
    frame.h
    ipaddress.h
)

add_subdirectory(packets)


add_library(
    ${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    Qt5::Core
    ${PROJECT_NAME}-packets
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -Wall
)

target_include_directories(
    ${PROJECT_NAME}-packets PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/packets
)
