cmake_minimum_required(VERSION 3.15)

project(aTools VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

include(UseSphinxDoc)
add_sphinx_doc(
        ${PROJECT_SOURCE_DIR}/doc
        ${PROJECT_SOURCE_DIR}/doc/_build
        ${PROJECT_SOURCE_DIR}/doc/build/_doctrees
        ${PROJECT_SOURCE_DIR}/doc/_build/html
        ${PROJECT_SOURCE_DIR}/doc/conf.py.in
        docs
        "HTML Documentation"
)

