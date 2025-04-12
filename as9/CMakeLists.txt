cmake_minimum_required(VERSION 3.18)
project(as9 C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(raylib-cpp)

add_executable(as9 src/as9.cpp)
target_link_libraries(as9 PUBLIC raylib raylib_cpp raygui)