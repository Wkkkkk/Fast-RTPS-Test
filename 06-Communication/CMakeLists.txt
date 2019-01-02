cmake_minimum_required(VERSION 3.5)
project(Communication)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread")
add_subdirectory(messages)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

include_directories(messages)
message(STATUS "Configuring Recognition...")
file(GLOB SOURCES_CXX "*.cxx")
file(GLOB SOURCES_Cpp "*.cpp")

file(COPY ${PROJECT_SOURCE_DIR}/Configure.xml
        DESTINATION ${PROJECT_BINARY_DIR}
        )
add_executable(Communication ${SOURCES_CXX} ${SOURCES_CPP})
target_link_libraries(Communication
        fastcdr fastrtps
        Info_lib
        )
