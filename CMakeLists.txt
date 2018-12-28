cmake_minimum_required(VERSION 3.5)
project(Fast-RTPS-Test)

set(CMAKE_CXX_STANDARD 11)
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "LD_LIBRARY_PATH  : ${LD_LIBRARY_PATH}")

add_subdirectory(00-HelloWorld)
add_subdirectory(01-UserDefinedData)