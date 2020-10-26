cmake_minimum_required(VERSION 3.17)

project(teal)
set(CMAKE_CXX_STANDARD 14)
add_executable(${PROJECT_NAME} main.cpp ../grep/main.cpp)