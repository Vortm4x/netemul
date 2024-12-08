cmake_minimum_required(VERSION 3.19)

project(netemul VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets
    LinguistTools
    Xml
    Script
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_DIR          src)

set(SOURCE_PATH         ${PROJECT_SOURCE_DIR}/src)
set(TRANSLATIONS_PATH   ${PROJECT_SOURCE_DIR}/translations)
set(DOC_PATH            ${PROJECT_SOURCE_DIR}/doc)
set(SCRIPT_PATH         ${PROJECT_SOURCE_DIR}/scripts)


set(SOURCES
    ${SOURCE_PATH}/deviceport.cpp
    ${SOURCE_PATH}/ipedit.cpp
    ${SOURCE_PATH}/mainwindow.cpp
    ${SOURCE_PATH}/mycanvas.cpp
    ${SOURCE_PATH}/main.cpp
)

set(HEADERS
    ${SOURCE_PATH}/deviceport.h
    ${SOURCE_PATH}/ipedit.h
    ${SOURCE_PATH}/mainwindow.h
    ${SOURCE_PATH}/mycanvas.h
)

set(RESOURCES
    netemul.qrc
)

set(MODULES
    chips
    commands
    controls
    delegates
    devices
    dialogs
    factories
    forms
    graphics
    interfaces
    models
    appsetting
    programs
    states
    tools
)

foreach(MODULE_DIR ${MODULES})
    include(${SOURCE_PATH}/${MODULE_DIR}/module.cmake)
endforeach()

qt5_add_translation(TRANSLATIONS
    ${TRANSLATIONS_PATH}/netemul_ru.ts
    ${TRANSLATIONS_PATH}/netemul_pt_BR.ts
    ${TRANSLATIONS_PATH}/netemul_es.ts
)

add_subdirectory(basicnetlib)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
    ${TRANSLATIONS}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Xml
    Qt5::Script
    basicnetlib
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${SOURCE_PATH}
    ${INCLUDE_DEPS}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
)

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    TRANSLATIONS_PATH="${TRANSLATIONS_PATH}"
    DOC_PATH="${DOC_PATH}"
    SCRIPT_PATH="${SCRIPT_PATH}"
)
