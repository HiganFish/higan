cmake_minimum_required(VERSION 2.8.12)
project(higan)

SET(CMAKE_CXX_FLAGS "-g -std=c++17 -lstdc++ -Wall"
)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(higan)

add_subdirectory(example)