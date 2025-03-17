#include <iostream>
#include <vector>
#include <algorithm>
#include "CO.hpp"
#include "raylib-cpp.hpp"
#include "skybox.hpp"

enum class EntityType { sedan, Firetruck, ambulance, Police, Taxi, Rocket };

template<typename T>
concept Transformer = requires(T t, raylib::Matrix m) {
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};

void DrawBoundedModel(raylib::Model& model, Transformer auto transformer){
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.GetTransformedBoundingBox().Draw();
    model.transform = backup;
}

void DrawModel(raylib::Model& model, Transformer auto transformer){
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.transform = backup;
}

struct TransformComponent {
    raylib::Vector3 position;
    raylib::Quaternion rotation;
    raylib::Vector3 scale;

    TransformComponent(
        raylib::Vector3 pos = {0, 0, 0},
        raylib::Quaternion rot = raylib::Quaternion::Identity(),
        raylib::Vector3 sc = {30, 30, 30}
    ) : position(pos), rotation(rot), scale(sc) {}

    raylib::Matrix GetMatrix() const {
        return raylib::Matrix::Identity()
            .Scale(scale.x, scale.y, scale.z)
            .Rotate(rotation)
            .Translate(position);
    }
};

struct PhysicsComponent {
    raylib::Vector3 velocity;

    PhysicsComponent(raylib::Vector3 vel = {0, 0, 0}) : velocity(vel) {}

    virtual void Update(TransformComponent& transform, float dt) {
        transform.position += velocity * dt;
    }

    virtual ~PhysicsComponent() {}
};

struct OrientedPhysicsComponent : public PhysicsComponent {
    float heading;

    OrientedPhysicsComponent(raylib::Vector3 vel, float head)
        : PhysicsComponent(vel), heading(head) {}

    void Update(TransformComponent& transform, float dt) override {
        float radians = heading * DEG2RAD;

        raylib::Vector3 direction{-std::sin(radians), 0, std::cos(radians)};

        velocity = direction * velocity.Length();
        transform.position += velocity * dt;
    }
};

struct RocketPhysicsComponent : public PhysicsComponent{
    float fly = 0.0f;
    bool isFlying = false;
    bool descend;

    RocketPhysicsComponent(raylib::Vector3 vel = {0, 0, 0}, float speed = 20.0f, float descendSpeed = 5.0f)
        : PhysicsComponent(vel), fly(speed), descend(descendSpeed) {}

        void Update(TransformComponent& transform, float dt) override {
            if(raylib::Keyboard::IsKeyDown(KEY_W)){
                velocity.y = fly; //allows the rocket to ascend vertically
            }
            else if(raylib::Keyboard::IsKeyDown(KEY_S)){
                velocity.y -= fly; //allows the rocket to descend 
            }
            else{ 
                velocity.z = 0.0f; //fixes the drift movement when rocket is in the air
            }

            if (transform.position.y < 0.0f) { //checks for position, so no objects will go below ground
                transform.position.y = 0.0f;
                velocity.y = 0.0f;
            }

            transform.position += velocity * dt;
        }
};

struct Entity { //part of the CO.hpp file. allows the use of entities.
    TransformComponent transform;  //allows for the entities to be transformed. inherited from transformComponent
    raylib::Model* model;   
    PhysicsComponent* physics;  //allows for entities to have physics such as movement and turning. inhertied from physicsComponent.
    bool drawBoundingBox;  //variable for setting the bounding box around an entity. 
    float speed = 0.0f;  //variable for setting the speed of an entity
    float maxSpeed = 50.0f;  //variable for setting the max speed of an entity
    float deceleration = 5.0f; //variable for slowing down the speed of an entity
    float heading = 0.0f; //variable used to represent the turning angle of an entity in degrees
    float turnRate = 20.0f;  //variable used for setting the speed of how fast an entity turns
    EntityType type;  //allows for differentiation between different entities
    bool isRocket;  //variable used to show if the rocket entity is being used or not

    Entity(raylib::Model* m, PhysicsComponent* phys, raylib::Vector3 pos, EntityType t, bool rocket, bool bbox = false) //defines a constructor for the struct, and sets parameters.
        : model(m), physics(phys), transform(pos), type(t), isRocket(rocket), turnRate(0.0f), drawBoundingBox(bbox) {} //defines and initializes all the parameters before the constructor begins

    void Update(float dt) { //function used to update the frame time and the direction of the car entity.
        if (physics){ //checks if the physics for the car entity is used. if not, it just updates the entity
            if(auto* carPhysics = dynamic_cast<OrientedPhysicsComponent*>(physics)){ //checks to see if the physics is being cast to the oriented physics component. If successful, the entity is treated as a car with physics.
                float radians = heading * DEG2RAD;  //sets and converts the heading angle from degrees to radians 
                raylib::Vector3 direction(-std::sin(radians), 0, std::cos(radians)); //sets the direction of where the entity will be moving.

                carPhysics->velocity = direction * speed; //applies velocity in the direction the car is moving
                carPhysics->heading = heading; //applies the heading angle
            }
            physics->Update(transform, dt);  //update the entity based on the physics.
        } 
    }

    void Draw(bool selectedModel) { //function used to draw the selected entity
        model->transform = transform.GetMatrix();  //used to draw the matrix after the transformed entity.
        model->Draw({}); //draw model
        if (selectedModel){ //bounding box only appears when entity is selected.
            model->GetTransformedBoundingBox().Draw();  //draw the bounding box around the selected entity
        } 
    }
};

int main(){
    raylib::Window window(800, 600, "CS381 - Assigment 6");
    window.SetState(FLAG_WINDOW_RESIZABLE);
    
    auto camera = raylib::Camera(
        {600, 120, 0}, //moves camera to the right side and zoomed out a bit more with 600
        {0, 0, 0},
        {0, 1, 0},
        45
    );

    raylib::Ray x;

    size_t selected_entity = 0;
    float rocketOffset = -50.0f;

    //Load Rocket Entity
    auto rocket = raylib::Model("meshes/rocket.glb");
    rocket.transform = raylib::Matrix::Identity().Scale(30);

    // Load Sedan Entity
    auto sedan = raylib::Model("meshes/sedan.glb");
    sedan.transform = raylib::Matrix::Identity().Scale(30);

    raylib::Texture2D carTexture("meshes/Textures/colormap.png");

    // Load Firetruck Entity
    auto firetruck = raylib::Model("meshes/firetruck.glb");
    firetruck.transform = raylib::Matrix::Identity().Scale(30);
    
    // Load Ambulance Entity
    auto ambulance = raylib::Model("meshes/ambulance.glb");
    ambulance.transform = raylib::Matrix::Identity().Scale(30);

    // Load Police Entity
    auto police = raylib::Model("meshes/police.glb");
    police.transform = raylib::Matrix::Identity().Scale(30);

    // Load Taxi Entity
    auto taxi = raylib::Model("meshes/taxi.glb");
    taxi.transform = raylib::Matrix::Identity().Scale(30);

    std::vector<Entity> entities;

    entities.emplace_back(&sedan, new OrientedPhysicsComponent({0,0,0}, 90), raylib::Vector3{0,0,0}, EntityType::sedan, false, true);
    entities.emplace_back(&firetruck, new OrientedPhysicsComponent({0,0,0}, 90), raylib::Vector3{0,0,-200}, EntityType::Firetruck, false, true);
    entities.emplace_back(&ambulance, new OrientedPhysicsComponent({0,0,0}, 90), raylib::Vector3{0,0,200}, EntityType::ambulance, false, true);
    entities.emplace_back(&police, new OrientedPhysicsComponent({0,0,0}, 90), raylib::Vector3{0,0,100}, EntityType::Police, false, true);
    entities.emplace_back(&taxi, new OrientedPhysicsComponent({0,0,0}, 90), raylib::Vector3{0,0,-100}, EntityType::Taxi, false, true);
    
    for(size_t i = 0; i < 5; i ++){ //loops all the car positions and places a rocket behind each
        raylib::Vector3 carPosition = entities[i].transform.position;

        raylib::Vector3 rocketPosition = carPosition + raylib::Vector3{0.0f, 0.0f, rocketOffset};
        entities.emplace_back(&rocket, new RocketPhysicsComponent({0,0,0}, 50.0f), rocketPosition, EntityType::Rocket, true, true);
    }

    raylib::Model grass = raylib::Mesh::Plane(1000, 1000, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    cs381::SkyBox sky("textures/skybox.png");

    while(!window.ShouldClose()){
        float dt = window.GetFrameTime();

        float rocketSpeed = 0.0f;
        const float maxSpeed = 50.0f;

        if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
            Ray ray = camera.GetMouseRay(raylib::Mouse::GetPosition());
            for (size_t i = 0; i < entities.size(); ++i) {
                auto& entity = entities[i];
                raylib::Matrix original = entity.model->transform;
                entity.model->transform = entity.transform.GetMatrix();
                BoundingBox bbox = entity.model->GetTransformedBoundingBox();
                entity.model->transform = original;
        
                RayCollision collision = GetRayCollisionBox(ray, bbox);
                if (collision.hit) {
                    selected_entity = i;
                    break;
                }
            }
        }
        
        Entity& activeEntity = entities[selected_entity];
        const float speedGain = 20.0f;
        const float turnRate = 100.0f;

        if(selected_entity < entities.size()){ //only update the selected entity.
            Entity& activeEntity = entities[selected_entity];

        if(activeEntity.type == EntityType::Rocket){
            RocketPhysicsComponent* rocketPhysics = dynamic_cast<RocketPhysicsComponent*>(activeEntity.physics);

            if(rocketPhysics){
                if(raylib::Keyboard::IsKeyDown(KEY_W)){
                    activeEntity.physics->velocity.y += rocketPhysics->fly * dt; //moves up
                }
                else if(raylib::Keyboard::IsKeyDown(KEY_S)){
                    activeEntity.physics->velocity.y -= rocketPhysics->fly * dt; // Move down
                }
                else if(raylib::Keyboard::IsKeyPressed(KEY_SPACE)){
                    activeEntity.speed = 0.0f;
                }
                else{
                    activeEntity.physics->velocity.y = 0.0f;
                }
            }
        }
        else if(activeEntity.type == EntityType::sedan || activeEntity.type == EntityType::Firetruck || 
            activeEntity.type == EntityType::ambulance || activeEntity.type == EntityType::Police || 
            activeEntity.type == EntityType::Taxi){

            OrientedPhysicsComponent* carPhysics = dynamic_cast<OrientedPhysicsComponent*>(activeEntity.physics);

            if(carPhysics){
                if (raylib::Keyboard::IsKeyDown(KEY_W)){ //increases car speed
                    activeEntity.speed += speedGain * dt;
                    activeEntity.speed = std::min(activeEntity.speed, activeEntity.maxSpeed);
                }
                if (raylib::Keyboard::IsKeyDown(KEY_S)){ //slows down car speed 
                    activeEntity.speed -= speedGain * dt;
                    activeEntity.speed = std::max(activeEntity.speed, 0.0f);
                }
                if (raylib::Keyboard::IsKeyDown(KEY_A)){
                    activeEntity.heading += activeEntity.turnRate * dt;
                }
                if (raylib::Keyboard::IsKeyDown(KEY_D)){
                    activeEntity.heading -= activeEntity.turnRate * dt;
                }
                if (raylib::Keyboard::IsKeyDown(KEY_SPACE)){ //pressing space stops car immediately
                    activeEntity.speed = 0.0f;
                } 
            }      
        } 
        }     

        // cast and combine physics components with Entity attributes
        if (auto* orientedPhys = dynamic_cast<OrientedPhysicsComponent*>(activeEntity.physics)) {
            float radians = activeEntity.heading * DEG2RAD;
            raylib::Vector3 direction(std::sin(radians), 0, std::cos(radians));

            orientedPhys->velocity = direction * activeEntity.speed;
            orientedPhys->heading = activeEntity.heading;
        } 

        entities[selected_entity].Update(dt);

        window.BeginDrawing();
            camera.BeginMode();
                window.ClearBackground(BLACK);
                sky.Draw();
                grass.Draw({});
                
                for (size_t i = 0; i < entities.size(); i++) { //only the selected entity will have a bounding box drawn.
                    entities[i].Draw(i == selected_entity);
                }
                
            camera.EndMode();
        window.EndDrawing();

        if(raylib::Keyboard::IsKeyPressed(KEY_TAB)){
            selected_entity = (selected_entity + 1) % entities.size();
        }
    }

    for(auto& e : entities){
        delete e.physics;
    }
}