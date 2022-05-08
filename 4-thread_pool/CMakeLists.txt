cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

project(ThreadPool)

set(SOURCES main.cpp threadPool.h)
add_executable(ThreadPool ${SOURCES})