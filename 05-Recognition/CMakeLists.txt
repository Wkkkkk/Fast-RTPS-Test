cmake_minimum_required(VERSION 3.5)
project(Recognition)

add_subdirectory(messages)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

include_directories(messages)
message(STATUS "Configuring Recognition...")
file(GLOB SOURCES_CXX "*.cxx")
file(GLOB SOURCES_Cpp "*.cpp")

add_executable(Recognition ${SOURCES_CXX} ${SOURCES_CPP})
target_link_libraries(Recognition fastcdr fastrtps
        AdvancedInfo_lib BasicInfo_lib)
