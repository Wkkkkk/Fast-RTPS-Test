cmake_minimum_required(VERSION 3.5)
project(ChaseGame)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/icon.qrc)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread")

message(STATUS "Configuring WithOSG...")
add_subdirectory(message)

find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)
find_package(Qt5Widgets 5.11 CONFIG REQUIRED)
find_package(Qt5Core 5.11 CONFIG REQUIRED)
find_package(Qt5Gui 5.11 CONFIG REQUIRED)
find_package(Qt5Network 5.11 CONFIG REQUIRED)
find_package(Qt5OpenGL 5.11 CONFIG REQUIRED)
qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
find_package(OpenSceneGraph 3.2 REQUIRED COMPONENTS osgGA osgViewer osgDB osgUtil osgText osg osgQt osgFX)

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

file(GLOB SOURCES_CXX "*.cxx")
file(GLOB SOURCES_CPP "*.cpp")
file(GLOB CONFING_XML "*.xml")
file(COPY ${CONFING_XML}
        DESTINATION ${PROJECT_BINARY_DIR}
        )
message(STATUS "Copying XML...")

add_executable(${PROJECT_NAME} ${SOURCES_CXX} ${SOURCES_CPP} ${QRC_FILES})
target_link_libraries(${PROJECT_NAME}
        fastcdr fastrtps
        message_lib  # message
        Qt5::Gui
        Qt5::Widgets
        Qt5::Core
        Qt5::Network
        Qt5::OpenGL
        ${OPENSCENEGRAPH_LIBRARIES}
        )