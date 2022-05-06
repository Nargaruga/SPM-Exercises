cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

project(OverheadTest)

set(SOURCES
    overhead_test.cpp)
    
add_executable(OverheadTest ${SOURCES})