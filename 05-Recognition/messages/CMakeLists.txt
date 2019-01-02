cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

message(STATUS "Configuring BasicInfo...")
add_library(BasicInfo_lib BasicInfo.cxx)
target_link_libraries(BasicInfo_lib fastcdr fastrtps)

message(STATUS "Configuring AdvancedInfo...")
add_library(AdvancedInfo_lib AdvancedInfo.cxx)
target_link_libraries(AdvancedInfo_lib fastcdr fastrtps)
