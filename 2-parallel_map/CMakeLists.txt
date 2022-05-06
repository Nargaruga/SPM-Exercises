cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

project(ParallelMap)

set(SOURCES 
    main.cpp
    parallel_map.cpp
    parallel_map.h
    ../timer.h)

add_executable(ParallelMap ${SOURCES})