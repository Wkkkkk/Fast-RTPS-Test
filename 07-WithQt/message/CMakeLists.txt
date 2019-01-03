cmake_minimum_required(VERSION 2.8.12)

project(message)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

message(STATUS "Configuring Info...")
add_library(QInfo_lib Info.cxx InfoPubSubTypes.cxx)
target_link_libraries(QInfo_lib fastcdr fastrtps)
