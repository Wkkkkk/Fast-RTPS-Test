cmake_minimum_required(VERSION 3.5)
project(WithQt)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread")

message(STATUS "Configuring WithQt...")
add_subdirectory(message)

find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(OpenSceneGraph 3.2 REQUIRED COMPONENTS osgGA osgViewer osgDB osgUtil osgText osg osgQt osgFX)

include_directories(
        ${OPENSCENEGRAPH_INCLUDE_DIRS})

file(GLOB SOURCES_CXX "*.cxx")
file(GLOB SOURCES_CPP "*.cpp")
file(COPY ${PROJECT_SOURCE_DIR}/Configure.xml
        DESTINATION ${PROJECT_BINARY_DIR}
        )
message(STATUS "Copying XML...")

add_executable(WithQt ${SOURCES_CXX} ${SOURCES_CPP})
target_link_libraries(WithQt
        fastcdr fastrtps
        QInfo_lib  # message
        Qt5::Gui
        Qt5::Widgets
        Qt5::Core
        Qt5::Network
        ${OPENSCENEGRAPH_LIBRARIES}
        )
