cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

project(Farm)

set(SOURCES main.cpp)
add_executable(Farm ${SOURCES})

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Farm PUBLIC OpenMP::OpenMP_CXX)
endif()