cmake_minimum_required(VERSION 3.15)

project(aTools VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

# 设置二进制输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
# 设置 CMake 模块路径
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# 配置文件生成
set(LOCALEDIR ${CMAKE_BINARY_DIR}/bin)
set(PACKAGE aTools)
configure_file(${PROJECT_SOURCE_DIR}/include/Config.h.in
        ${PROJECT_BINARY_DIR}/include/Config.h)
include_directories(${PROJECT_BINARY_DIR}/include)
# 公有头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
# 子模块
add_subdirectory(src)

# 文档构建
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

if(MSVC)
    add_compile_options(/source-charset:utf-8)
    string(FIND ${VCPKG_TARGET_TRIPLET} "static" vcpkg_static_build)
    if(${vcpkg_static_build})
        add_compile_options(/MT)
    endif()
endif()


