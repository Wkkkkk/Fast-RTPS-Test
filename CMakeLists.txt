cmake_minimum_required(VERSION 3.5)
project(Fast-RTPS-Test)

set(CMAKE_CXX_STANDARD 11)
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "LD_LIBRARY_PATH  : ${LD_LIBRARY_PATH}")

add_subdirectory(00-HelloWorld)
add_subdirectory(01-UserDefinedData)
add_subdirectory(02-Xml)
add_subdirectory(03-BigData)
add_subdirectory(04-FlowController)
add_subdirectory(05-Recognition)
add_subdirectory(06-Communication)

find_package(Qt5Widgets QUIET)
if (Qt5Widgets_FOUND)
    add_subdirectory(07-WithQt)
endif ()