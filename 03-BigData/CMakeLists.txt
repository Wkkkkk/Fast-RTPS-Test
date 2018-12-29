
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)


message(STATUS "Configuring BigData...")
add_library(BigData_lib BigData.cxx)
target_link_libraries(BigData_lib fastcdr fastrtps)

add_executable(BigData BigDataPubSubTypes.cxx BigDataPublisher.cxx BigDataSubscriber.cxx BigDataPubSubMain.cxx)
target_link_libraries(BigData fastcdr fastrtps
        BigData_lib)

