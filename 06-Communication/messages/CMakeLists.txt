
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

message(STATUS "Configuring Info...")
add_library(Info_lib Info.cxx InfoPubSubTypes.cxx)
target_link_libraries(Info_lib fastcdr fastrtps)

