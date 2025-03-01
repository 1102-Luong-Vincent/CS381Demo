cmake_minimum_required(VERSION 3.18)
project(as4 C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(raylib-cpp)

add_executable(as4 src/as4.cpp src/obstacle.cpp src/player.cpp)
target_link_libraries(as4 PUBLIC raylib raylib_cpp raygui)