#include <iostream>
#include <raylib-cpp.hpp>
#include "skybox.hpp"
#include "CO.hpp"
#include <vector>

cs381::Entity* selectedEntity = nullptr;

void DrawBoundedModel(raylib::Model& model, auto transformer){
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.GetTransformedBoundingBox().Draw();
    model.transform = backup;
}

void DrawModel(raylib::Model& model, auto transformer){
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.transform = backup;
}

struct Entity{
    raylib::Vector3 position;
    float movement;
    float rotation;
    float lift;
    bool isRocket;

    Entity(raylib::Vector3 position, bool rocket = false)
        : position(position), rotation(0.0f), lift(0.0f), movement(0.0f), isRocket(rocket) {}

        void Tick(float dt) {
            float radians = rotation * DEG2RAD;
            
            if (isRocket) { //movement for rocket
                position.x += -sin(radians) * movement * dt;
                position.y += cos(radians) * movement * dt;
                position.z += 0;
    
                if (position.y < 0.0f) { //checks for position, so no objects will go below ground
                    position.y = 0.0f;
                    movement = 0.0f;
                }
            } 
            else { //movement for car
                position.x += sin(radians) * movement * dt;
                position.z += cos(radians) * movement * dt;
            }
        }
};

void HandleMouseSelection(raylib::Camera& camera, std::vector<Entity>& entities, size_t& selected_entity, raylib::Model& rocket, raylib::Model& car) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Get the mouse position
        raylib::Vector2 mousePosition = GetMousePosition();

        // Cast a ray from the mouse position into the 3D world
        raylib::Ray ray = camera.GetMouseRay(mousePosition);

        // Check for collisions with each entity
        for (size_t i = 0; i < entities.size(); i++) {
            auto& e = entities[i];

            auto transform = raylib::Matrix::Identity()
                .Translate(e.position)
                .RotateY(e.rotation * DEG2RAD);
                
                // Transform the bounding box
                raylib::BoundingBox boundBox;
                if(e.isRocket){
                    boundBox = rocket.GetTransformedBoundingBox();
                }
                else{
                    boundBox = car.GetTransformedBoundingBox();
                }

                //apply the transformation to the bounding boxes
                boundBox.min = Vector3Transform(boundBox.min, transform);
                boundBox.max = Vector3Transform(boundBox.max, transform);

                // Check for collision between the ray and the bounding box
                raylib::RayCollision collision = ray.GetCollision(boundBox);

                if (collision.hit) {
                    selected_entity = i;
                    break;
                }
        }
    }
}

int main(){

    raylib::Window window(800, 600, "CS 381 - Assignment 5");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    auto camera = raylib::Camera(
        {0, 80, -200}, 
        {0, 20, 0},  
        {0, 1 ,0},
        45
    );

    std::vector<Entity> entities;
    size_t selected_entity = 0;

    entities.emplace_back(raylib::Vector3(0, 0, 0), true);
    entities.emplace_back(raylib::Vector3(30, 0, 0), false);
    entities.emplace_back(raylib::Vector3(-30, 0, 0), false);

    auto rocket = raylib::Model("meshes/rocket.glb");
    rocket.transform = raylib::Matrix::Identity().Scale(15);

    auto car = raylib::Model("meshes/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(15);

    cs381::SkyBox sky("textures/skybox.png");

    Texture2D carTexture = LoadTexture("Textures/colormap.png");
    raylib::Model grass = raylib::Mesh::Plane(500, 500, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    while(!window.ShouldClose()){

        auto dt = window.GetFrameTime();
        Entity& selectedEntity = entities[selected_entity];

        if(selectedEntity.isRocket){
            if (raylib::Keyboard::IsKeyPressed(KEY_W)) {
                selectedEntity.movement += 5.0f;
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
                selectedEntity.movement -= 5.0f;
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_A)) {
                selectedEntity.rotation += 7.0f;
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_D)) {
                selectedEntity.rotation -= 7.0f;
            }
        }
        else{
            if (raylib::Keyboard::IsKeyPressed(KEY_W)) {
                selectedEntity.movement += 5.0f;
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
                selectedEntity.movement -= 5.0f;
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_A)) {
                selectedEntity.rotation += 7.0f;
            }    
            if (raylib::Keyboard::IsKeyPressed(KEY_D)) {
                selectedEntity.rotation -= 7.0f;
            }
            if(raylib::Keyboard::IsKeyPressed(KEY_SPACE)){
                //car only
                selectedEntity.movement = 0.0f;
            }
        }

        HandleMouseSelection(camera, entities, selected_entity, rocket, car);

        if(raylib::Keyboard::IsKeyPressed(KEY_TAB)){
            selected_entity = (selected_entity + 1) % entities.size();
        }

        for(auto& e : entities){
            e.Tick(dt);
        }
        window.BeginDrawing();
        camera.BeginMode();
        window.ClearBackground(raylib::Color::Black());

        sky.Draw();
        grass.Draw({});

        for(size_t i = 0; i < entities.size(); ++i){
            const auto& e = entities[i];

            auto transformer = [&e](raylib::Matrix& transform) {
                if (e.isRocket) {
                    return transform
                    .Translate(e.position)
                    .RotateZ(e.rotation * DEG2RAD);
                } 
                else {
                    return transform
                    .Translate(e.position)
                    .RotateY(e.rotation * DEG2RAD);
                }
            };
            if(i == selected_entity){
                if (e.isRocket) { 
                    DrawBoundedModel(rocket, transformer);
                } 
                else {
                    DrawBoundedModel(car, transformer);
                }
            }
            else{
                if (e.isRocket) { 
                    DrawModel(rocket, transformer);
                } 
                else {
                    DrawModel(car, transformer);
                }
            }
        }

        camera.EndMode();
        window.EndDrawing();
    }
}