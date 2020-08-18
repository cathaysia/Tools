cmake_minimum_required(VERSION 3.17)
project(head_and_teal)

set(CMAKE_CXX_STANDARD 14)

add_executable(tail tail.cpp common.h)
add_executable(head head.cpp common.h)