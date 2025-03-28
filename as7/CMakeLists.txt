cmake_minimum_required(VERSION 3.18)
project(as7 C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(raylib-cpp)

add_executable(as7 src/as7.cpp)
target_link_libraries(as7 PUBLIC raylib raylib_cpp raygui)