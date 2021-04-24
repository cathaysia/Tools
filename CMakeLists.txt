cmake_minimum_required(VERSION 3.15)

project(aTools VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(LOCALEDIR ${CMAKE_BINARY_DIR}/bin)
set(PACKAGE aTools)
configure_file(${PROJECT_SOURCE_DIR}/include/Config.h.in
        ${PROJECT_BINARY_DIR}/include/Config.h)
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

set(DOC_LANGUAGE zh_CN)
include(UseBreatheDoc)
add_breathe_doc(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_BINARY_DIR}/doc
        ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in
        ${PROJECT_SOURCE_DIR}/doc/conf.py.in
        html
        "HTML Documentation"
)

