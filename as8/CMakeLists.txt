cmake_minimum_required(VERSION 3.18)
project(as8 C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(raylib-cpp)
include(includeable.cmake)

add_executable(as8 src/as8.cpp src/skybox.cpp)
target_link_libraries(as8 PUBLIC raylib raylib_cpp raygui raylib::buffered)

make_includeable(assets/shaders/cubemap.fs generated/cubemap.fs)
make_includeable(assets/shaders/cubemap.vs generated/cubemap.vs)
make_includeable(assets/shaders/skybox.fs generated/skybox.fs)
make_includeable(assets/shaders/skybox.vs generated/skybox.vs)

configure_file("assets/Kenny Space Kit/rocketA.glb" meshes/rocket.glb COPYONLY)
configure_file("assets/Kenny Car Kit/sedan.glb" meshes/sedan.glb COPYONLY)
configure_file("assets/Kenny Car Kit/ambulance.glb" meshes/ambulance.glb COPYONLY)
configure_file("assets/Kenny Car Kit/firetruck.glb" meshes/firetruck.glb COPYONLY)
configure_file("assets/Kenny Car Kit/police.glb" meshes/police.glb COPYONLY)
configure_file("assets/Kenny Car Kit/taxi.glb" meshes/taxi.glb COPYONLY)
configure_file("assets/Kenny Car Kit/Textures/colormap.png" "meshes/Textures/colormap.png" COPYONLY)
configure_file(assets/textures/skybox.png textures/skybox.png COPYONLY)