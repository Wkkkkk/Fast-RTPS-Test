
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)


message(STATUS "Configuring Base...")
add_library(Base_lib Base.cxx)
target_link_libraries(Base_lib fastcdr fastrtps)

add_executable(Base BasePubSubTypes.cxx BasePublisher.cxx BaseSubscriber.cxx BasePubSubMain.cxx)
target_link_libraries(Base fastcdr fastrtps
        Base_lib)


message(STATUS "Configuring Vec...")
add_library(Vec_lib Vec.cxx)
target_link_libraries(Vec_lib fastcdr fastrtps)

add_executable(Vec VecPubSubTypes.cxx VecPublisher.cxx VecSubscriber.cxx VecPubSubMain.cxx)
target_link_libraries(Vec fastcdr fastrtps
        Vec_lib Base_lib)

