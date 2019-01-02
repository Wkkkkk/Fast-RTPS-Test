
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)


message(STATUS "Configuring FlowControl...")
add_library(FlowControl_lib FlowControl.cxx)
target_link_libraries(FlowControl_lib fastcdr fastrtps)

file(COPY ${PROJECT_SOURCE_DIR}/Configure.xml
        DESTINATION ${PROJECT_BINARY_DIR}
        )
message(STATUS "Copying XML...")

add_executable(FlowControl FlowControlPubSubTypes.cxx FlowControlPublisher.cxx FlowControlSubscriber.cxx FlowControlPubSubMain.cxx)
target_link_libraries(FlowControl fastcdr fastrtps
        FlowControl_lib)

