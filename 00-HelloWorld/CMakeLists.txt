cmake_minimum_required(VERSION 3.5)
project(HelloWorld)

find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

message(STATUS "Configuring HelloWorld example...")
file(GLOB SOURCES_CXX "*.cxx")
file(GLOB SOURCES_CPP "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES_CXX} ${SOURCES_CPP})
target_link_libraries(${PROJECT_NAME} fastrtps fastcdr)