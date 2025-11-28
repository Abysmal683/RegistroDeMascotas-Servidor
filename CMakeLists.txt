cmake_minimum_required(VERSION 3.19)
project(RegistroDeMascotas-Servidor VERSION 0.1 LANGUAGES CXX)
# Estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Widgets
    LinguistTools
    Sql
#   Gui
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#Paths o archivos
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HDR_DIR ${CMAKE_SOURCE_DIR}/include)
set(UI_DIR ${CMAKE_SOURCE_DIR}/ui)
set(RES_DIR ${CMAKE_SOURCE_DIR}/resources)
set(TS_DIR ${CMAKE_SOURCE_DIR}/translations)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${UI_DIR}
)
#qt creator usara esto para agrupar archivos por carpetas
file(GLOB_RECURSE SRC_FILES
${SRC_DIR}/*.cpp
)
file(GLOB_RECURSE HDR_FILES
${HDR_DIR}/*.h
#${HDR_DIR}/*.inl
)
file(GLOB UI_FILES
${UI_DIR}/*.ui
)
file(GLOB RES_FILES
${RES_DIR}/*.qrc
)

file(GLOB TS_FILES
${TS_DIR}/*.ts
)
#recursos
qt_add_resources(resources ${RES_FILES})

#target u objectivo principal
qt_add_executable(${PROJECT_NAME}
${SRC_FILES}
${HDR_FILES}
${UI_FILES}
${RES_FILES}
)
target_sources(${PROJECT_NAME} PRIVATE ${resources})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${HDR_DIR}
    #${HDR_DIR}/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Sql
#        Qt::Gui
)
#salidas de directorios
set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
# traducciones
qt_add_translations(${PROJECT_NAME} TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES)
set(QM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/translations)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QM_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILES} ${QM_OUTPUT_DIR}
)
message(STATUS "QM files: ${QM_FILES}")
