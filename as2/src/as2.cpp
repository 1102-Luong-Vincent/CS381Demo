#include <iostream>
#include <raylib-cpp.hpp>
#include "skybox.hpp"

template<typename T>
concept Transformer = requires(T t, raylib::Matrix m){
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};

void DrawBoundedModel(raylib::Model& model, auto transformer){
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});

    BoundingBox box = model.GetTransformedBoundingBox();
    DrawBoundingBox(box, raylib::Color::White());

    model.transform = backup;
}

auto fn() -> int {

}

int main(){

    float spin = 0.0f; //keeps track of the spinning angle.
    
    raylib::Window window(800, 600, "CS381 - Assignment 2");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    raylib::Model cube = raylib::Mesh::Cube(30, 30, 30).LoadModelFrom();
    auto camera = raylib::Camera(
        {0, 120, 500}, 
        {0, 0, 0}, 
        {0, 1 ,0},
        45
    );

    auto rocket = raylib::Model("meshes/rocket.glb");
    rocket.transform = raylib::Matrix::Identity().Scale(30);

    auto car = raylib::Model("meshes/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(30);

    auto wheel = raylib::Model("meshes/wheel-default.glb");
    wheel.transform = raylib::Matrix::Identity().Scale(50);

    cs381::SkyBox sky("textures/skybox.png");
    Texture2D carTexture = LoadTexture("Textures/colormap.png");

    while(!window.ShouldClose()){
        window.BeginDrawing();
        camera.BeginMode();
            window.ClearBackground(raylib::Color::Black());
            sky.Draw();

            auto rocketDefaultPosition = [&sky, &spin](raylib::Matrix& transform) -> raylib::Matrix {
                return transform
                .Translate(0, 0, 0)
                .RotateY(spin); //spins counter-clockwise
            };

            auto rocketTransformer = [&sky, &spin](raylib::Matrix& transform) -> raylib::Matrix {
                return transform
                    .Translate(-100, 100, 0)
                    .Scale(1.0f, -1.0f, 1.0f)
                    .RotateY(180 * DEG2RAD)
                    .RotateY(-spin); //spins clockwise
            };

            auto carTransformer = [&sky, &spin](raylib::Matrix& transform) -> raylib::Matrix {
                return transform
                .Translate(-200, 0, 0)
                .RotateY(spin); //spin counter-clockwise
            };

            auto carTransformer2 = [&sky, &spin](raylib::Matrix& transform) -> raylib::Matrix {
                return transform 
                    .Translate(200, 0, 0)
                    .RotateY(90 * DEG2RAD)
                    .RotateY(spin); //spins counter-clockwise
            };

            auto carTransformer3 = [&sky, &spin](raylib::Matrix& transform) -> raylib::Matrix {
                return transform
                    .Translate(100, 100, 0)
                    .Scale(1.0f, 2.0f, 1.0f)
                    .RotateY(270 * DEG2RAD)
                    .RotateY(-spin); //spins clockwise
            };

            spin += 3.0f * GetFrameTime(); //keeps track of the speed of the spin and keeps the spin consistent.

            DrawBoundedModel(rocket, rocketDefaultPosition);
            DrawBoundedModel(rocket, rocketTransformer);
            DrawBoundedModel(car, carTransformer);
            DrawBoundedModel(car, carTransformer2);
            DrawBoundedModel(car, carTransformer3);

            camera.EndMode();
            window.EndDrawing();    
    }
}