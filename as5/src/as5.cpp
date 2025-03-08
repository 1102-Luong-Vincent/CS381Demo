#include <iostream>
#include <raylib-cpp.hpp>
#include "skybox.hpp"
#include "CO.hpp"

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

struct MeshRenderComponent: cs381::Component{
    raylib::Model* model = nullptr;

    MeshRenderComponent(cs381::Entity& e, raylib::Model* model)
        : cs381::Component(e), model(model) {}

    void Tick(float dt) override{
        cs381::Component::Tick(dt);
        if(model == nullptr){
            return;
        }
        DrawModel(*model, [this](raylib::Matrix& matrix){
            auto& transform = Object().Transform();
            return matrix.Translate(transform.position).RotateY(transform.heading);
        });

        BoundingBox box = model->GetTransformedBoundingBox();
        
        if (&Object() == selectedEntity) {
            DrawBoundedModel(*model, [this](raylib::Matrix& matrix) {
                auto& transform = Object().Transform();
                return matrix.Translate(transform.position).RotateY(transform.heading);
            });  
        }    
    }
};

struct PhysicsComponent : cs381::Component{
    float lift = 0.0f;
    float angleMovement = 0.0f;
    bool isRocket = false;

    raylib::Vector3 velocitySpeed = {0, 0, 0};
    PhysicsComponent(cs381::Entity& e, bool isRocketCheck) : cs381::Component(e), isRocket(isRocketCheck) {}

    void Tick(float dt) override{ //tick ensures that velocity is applied every frame
        cs381::Component::Tick(dt);
        auto& transform = Object().Transform();

        float radians = (float)transform.heading * DEG2RAD;

        raylib::Vector3 movementDirection;

        if(isRocket){
            movementDirection = raylib::Vector3(
                sin(radians) * cos(lift), 
                sin(lift), 
                cos(radians) * cos(lift)   //rotate in the YZ plane direction, instead of XY which was cos, sin
            );
        }
        else{
            movementDirection = raylib::Vector3(
                sin(radians) * velocitySpeed.Length(), 0, cos(radians) * velocitySpeed.Length()//move only x and z directions.
            );
        }
        
        transform.position += velocitySpeed * dt;

        if (isRocket && transform.position.y < 0.0f) { //checks collision so rocket doesn't go through ground
            transform.position.y = 0.0f;  
            velocitySpeed.y = 0.0f;       
        }
    }

    void Velocity(float speed){
        float radians = (float)Object().Transform().heading * DEG2RAD;

        if(isRocket){
            velocitySpeed.y += cos(radians) * cos(lift) * speed;
        }
        else{
            velocitySpeed += raylib::Vector3(-sin(radians) * speed, 0, cos(radians) * speed);
        }
    }

    void angleSpeed(float force){
        auto& transform = Object().Transform();
        transform.heading += force;

        float radians = (float)transform.heading * DEG2RAD;
        if(isRocket){
            velocitySpeed.x = sin(radians) * cos(lift) * velocitySpeed.Length();
            velocitySpeed.z = cos(radians) * cos(lift) * velocitySpeed.Length();
            velocitySpeed.y = sin(lift) * velocitySpeed.Length();
        }
        else{
            velocitySpeed = raylib::Vector3(
                -sin(radians) * velocitySpeed.Length(),
                0,
                cos(radians) * velocitySpeed.Length()
            );
        }
    }

    void liftSpeed(float force){
        lift += force * DEG2RAD;
    }

};

struct Entity{
    raylib::Vector3 position;
    float movement = 0.0f;
    float rotation = 0.0f;
    bool isRocket = false;
};

int main(){

    raylib::Window window(800, 600, "CS 381 - Assignment 5");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    auto camera = raylib::Camera(
        {0, 80, -200}, 
        {0, 20, 0},  
        {0, 1 ,0},
        45
    );

    std::vector<cs381::Entity> entities;
    size_t selected_entity = 0;

    auto rocket = raylib::Model("meshes/rocket.glb");
    rocket.transform = raylib::Matrix::Identity().Scale(15);

    auto& rocketEntity = entities.emplace_back();
    rocketEntity.AddComponent<MeshRenderComponent>(&rocket);
    rocketEntity.AddComponent<PhysicsComponent>(true);
    rocketEntity.Transform().position = raylib::Vector3(0, 0, 0); 

    auto car = raylib::Model("meshes/sedan.glb");
    car.transform = raylib::Matrix::Identity().Scale(15);

    auto& carEntity = entities.emplace_back();
    carEntity.AddComponent<MeshRenderComponent>(&car);
    carEntity.AddComponent<PhysicsComponent>(false);
    carEntity.Transform().position = raylib::Vector3(-30, 0, 0); //coordinates for car position

    auto& carEntity2 = entities.emplace_back();
    carEntity2.AddComponent<MeshRenderComponent>(&car);
    carEntity2.AddComponent<PhysicsComponent>(false);
    carEntity2.Transform().position = raylib::Vector3(30, 0, 0); //coordinates for second car position

    cs381::SkyBox sky("textures/skybox.png");

    Texture2D carTexture = LoadTexture("Textures/colormap.png");
    raylib::Model grass = raylib::Mesh::Plane(500, 500, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    while(!window.ShouldClose()){

        if(!entities.empty()){
            selectedEntity = &entities[selected_entity];
        }

        // auto pos = raylib::Mouse::GetPosition();
        // std::cout << pos.x << "," << pos.y << std::endl;
        
        bool isRocket = (&entities[selected_entity] == &rocketEntity);

        if(isRocket){
            if (raylib::Keyboard::IsKeyDown(KEY_W)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();
                
                if(movement){
                     movement->get().liftSpeed(5.0f);
                }
            }
            if (raylib::Keyboard::IsKeyDown(KEY_S)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();
     
                if(movement){
                     movement->get().liftSpeed(-5.0f);
                }
            }
            if (raylib::Keyboard::IsKeyDown(KEY_A)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();
     
                if(movement){ //allows turning left
                     movement->get().angleSpeed(5.0f);
                } 
            }
            if (raylib::Keyboard::IsKeyDown(KEY_D)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();
     
                if(movement){ //allows turning right
                     movement->get().angleSpeed(-5.0f);
                } 
            }
        }
        else{
            if (raylib::Keyboard::IsKeyPressed(KEY_W)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();

                if(movement){
                     movement->get().Velocity(5.0f);
                }
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();

                if(movement){
                     movement->get().Velocity(-5.0f);
                }
            }
            if (raylib::Keyboard::IsKeyPressed(KEY_A)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();
                if (movement) { //allows turning left
                    movement->get().angleSpeed(7.0f); 
                }
            }    
            if (raylib::Keyboard::IsKeyPressed(KEY_D)) {
                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();
                if (movement) { //allows turning right
                    movement->get().angleSpeed(-7.0f);  
                } 
            }
            if(raylib::Keyboard::IsKeyPressed(KEY_SPACE)){
                //car only

                auto movement = entities[selected_entity].GetComponent<PhysicsComponent>();

                if(movement){
                    movement->get().velocitySpeed = raylib::Vector3(0, 0, 0);
                }
            }
        }

        if(raylib::Keyboard::IsKeyPressed(KEY_TAB)){
            selected_entity = (selected_entity + 1) % entities.size();
            selectedEntity = &entities[selected_entity];
        }
    
        window.BeginDrawing();
        camera.BeginMode();
        window.ClearBackground(raylib::Color::Black());

        sky.Draw();
        grass.Draw({});

        auto dt = window.GetFrameTime();
        for(auto& e: entities){
            e.Tick(dt);
        }
        camera.EndMode();
        window.EndDrawing();
    }
}