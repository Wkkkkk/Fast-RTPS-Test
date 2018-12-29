
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)


message(STATUS "Configuring XML...")
add_library(XML_lib XML.cxx)
target_link_libraries(XML_lib fastcdr fastrtps)

file(COPY ${PROJECT_SOURCE_DIR}/XMLProfiles.xml
        DESTINATION ${PROJECT_BINARY_DIR}
        )

add_executable(XML XMLPubSubTypes.cxx XMLPublisher.cxx XMLSubscriber.cxx XMLPubSubMain.cxx)
target_link_libraries(XML fastcdr fastrtps
        XML_lib)

