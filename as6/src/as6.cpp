#include <iostream>
#include <concepts>
#include<functional>

#include "CO.hpp"
#include "BufferedRaylib.hpp"
#include "raylib-cpp.hpp"
#include "skybox.hpp"

template<typename T>
concept Transformer = requires(T t, raylib::Matrix m) {
    { t(m) } -> std::convertible_to<raylib::Matrix>;
};

void DrawBoundBox(raylib::Model& model) {
    auto boundingBox = model.GetBoundingBox();
    auto transformedBox = model.GetTransformedBoundingBox();
    DrawBoundingBox(transformedBox, WHITE);
}

void DrawBoundedModel(raylib::Model& model, Transformer auto transformer,bool isSelected) {
    raylib::Matrix backup = model.transform; 
    model.transform = transformer(backup); 
    model.Draw({}); 
    if (isSelected) DrawBoundBox(model);
    model.transform = backup;
}

struct MeshRenderComponent : cs381::Component {
    raylib::Model* model = nullptr;// Pointer to the 3D model to be rendered. Initialized to nullptr.
    raylib::Vector3 position = {0.0f, 0.0f, 0.0f}; // Initial position of the mesh in 3D space.
    bool selected = false;  // Flag indicating if the mesh is currently selected.
    float heading = 0.0f;// The heading (rotation angle) of the mesh.
    bool isRocket = false;// Determines if this mesh represents a rocket. Affects rotation axis during rendering.

    // Initializes the MeshRenderComponent constructor that sets the given entity, model, rocket flag, position, selection state, and heading.
    MeshRenderComponent(cs381::Entity& e, raylib::Model* model, bool isRocket, raylib::Vector3 position = {0.0f, 0.0f, 0.0f}, bool selected = true, float heading = 0.0f)
        : cs381::Component(e), model(model), isRocket(isRocket), selected(selected){
        // Set the initial position and heading of the object's transform.
        Object().Transform().position = position;
        Object().Transform().heading = heading;
    }
    // Sets the selected status of the mesh.
    void setSelected(bool selectedEntity){
        this->selected = selectedEntity;
    }
    void Tick(float dt) override {  // Called every frame to update and render the mesh.
        if (model == nullptr){ // Skip rendering if no model is assigned.
            return;
        }   

        DrawBoundedModel(*model, [this](raylib::Matrix& matrix) { // Render the model with transformations applied.
            auto& transform = Object().Transform();
            if (isRocket){
                // Apply translation and Z-axis rotation for rockets.
                return matrix.Translate(transform.position).RotateZ(transform.heading);
            }
            else{
                // Apply translation and Y-axis rotation for non-rockets.
                return matrix.Translate(transform.position).RotateY(transform.heading);
            }
        }, selected);
    }
};

struct PhysicsComponent : cs381 :: Component {
    float maxSpeed;// max speed for entity
    float speedIncrease;// increase speed for entity.
    float turnSpeed;// how fast the entity can rotate.
    float speed = 0.0f;// initial speed for entity.
    float rotation = 0.0f;// initial turning for entity.
    bool isRocket = false; // indicates whether the entity selected is a rocket
    bool isBraking = false; // indicates whether the entity is currently braking to a stop
    float speedDecrease = 5.0f; //entities slow down over time when braking is applied

    PhysicsComponent(cs381::Entity& e, float maxSpeed, float accel, float turnSpeed, bool rocket = false)
            : cs381::Component(e), maxSpeed(40.0f), speedIncrease(20.0f), turnSpeed(25.0f), isRocket(rocket){}
    
        void Tick(float dt) override {
            // Update the heading based on the current rotation and time delta.
            Object().Transform().heading += rotation * dt;
            float heading = Object().Transform().heading;
            // Update position based on speed and heading.
            if(isRocket){ //updates the x and y coordinates of movement for rocket
                Object().Transform().position.x += speed * sin(-heading * DEG2RAD) * dt;
                Object().Transform().position.y += speed * cos(-heading * DEG2RAD) * dt;

                if (Object().Transform().position.y < 0.0f) { //checks to see if the entity position is less than 0
                    Object().Transform().position.y = 0.0f; //sets the position of the entity no less than 0
                    speed = 0.0f; //entity is not moving
                }
            }
            else{
                //updates the x and z coordinates of movement for cars.
                Object().Transform().position.x += speed * sin(heading * DEG2RAD) * dt;
                Object().Transform().position.z += speed * cos(heading * DEG2RAD) * dt;
            }
            // checks to see if braking is applied. If it is, gradually reduce speed and rotation for selected entity.
            if (isBraking) {
                if(speed > 0){ //checks to see if speed is greater than 0 (basically entity is moving).
                    speed -= speedDecrease * dt; // Decrease speed.
                    if(speed < 0){ // checks to see if speed is less than 0 (basically entity is not moving)
                        speed = 0; // Set speed to zero.
                    }
                }
                else if(speed < 0){ // checks to see if speed is less than 0
                    speed += speedDecrease * dt; // Increase speed.
                    if(speed > 0){ // checks to see if speed is greater than 0.
                        speed = 0; // Set speed to zero.
                    }
                }
                if(rotation > 0){ //checks to see if rotation is turning clockwise
                    rotation -= turnSpeed * dt; // turns counterclockwise
                    if(rotation < 0){ // Check to see if rotation is turning counterclockwise
                        rotation = 0; // Make the rotation zero.
                    }
                }
                else if(rotation < 0){ // Check to see if rotation is turning counterclockwise.
                    rotation += turnSpeed * dt; // turns clockwise
                    if (rotation > 0){ // Checks to see if rotation is turning clockwise
                        rotation = 0; // Set rotation to zero.
                    }
                }
                
                if(speed == 0){ //if speed of entity is not moving
                    isBraking = false; //braking to slow down entity is not applied
                }
            }
        }
        // Adjusts the entity's speed.
        void Speed(bool accelerate) {
            if(accelerate){ //if entity gains speed
                if(speed < maxSpeed) { //if the speed of the entity is less than the maximum speed
                    speed += speedIncrease; //the speed of the entity will increase and not go over the max.
                }
            }
            else{  //if entity does not gain speed (false)
                if(speed > 0){ //if speed of the entity is greater than initial speed, 0
                    speed -= speedIncrease; // speed of the entity will decrease 
                    if (speed < 0){ // if speed of the entity is less than 0
                        speed = 0.0f; // entity will not move.
                    }
                }
                else if(speed < 0){ // if speed of the entity is less than 0
                    speed += speedIncrease; // speed of the entity will increase 
                    if (speed > 0){ // if speed of the entity is greater than 0
                        speed = 0.0f; // entity will not move
                    }
                }
                else if(speed > -maxSpeed){ //if speed is greather than maxSpeed
                    speed -= speedIncrease; //entity can reverse.
                }
            }
        }
        //adjusts how the entities rotate/turn
        void Rotation(bool Increase){
            if (Increase){ // if statment is true, increase the rotation of the entity.
                rotation += turnSpeed; // increases the rate of turning to the rotation
            }
            else{ // if statement is false, decrease the rotation of the entity.
                rotation -= turnSpeed; // decreases the rate of turning to the rotation
            }
        }
        //stops the entities completely
        void Stop(){
            speed = 0; // set speed to zero and entity will not move
            rotation = 0; // set rotation to zero and entity will not turn
        }        

        
};
// Component that handles the inputs of the meshes that are being rendered
struct InputComponent : cs381::Component {
    raylib::BufferedInput input; // BufferedInput object to store input data.
    MeshRenderComponent* meshRenderComponent; // Pointer to MeshRenderComponent object.

    // Initializes the InputComponent constructor that sets the given entity and mesh
    InputComponent(cs381::Entity& e, MeshRenderComponent* mesh) 
        : cs381::Component(e), meshRenderComponent(mesh){
        
        //sets up the input movement.
        input["forward"] = raylib::Action::button_axis(
            { raylib::Button::key(KEY_W), raylib::Button::key(KEY_UP) }, //button to move car forward
            { raylib::Button::key(KEY_S), raylib::Button::key(KEY_DOWN) } //button to move car backwards(reverse)
        ).move();

        input["forward"].AddCallback([this](float state, float change) {// forward input
            if (!meshRenderComponent->selected) return; // If callback input is not selected, return nothing.

            if (auto compOpt = Object().GetComponent<PhysicsComponent>()) { // If PhysicsComponent exists.
                if (state > 0) { // If state is positive and greater than 0, entity will move forward.
                    compOpt.value().get().Speed(true); // set the speed function in the the physicsComponent to true.
                } else if (state < 0) { // If state is negative and less than 0, entity will move backward
                    compOpt.value().get().Speed(false); // set the speed function in the physicsComponent to false
                }
            }
        });

        input["sideways"] = raylib::Action::button_axis( // sideways input
            { raylib::Button::key(KEY_A), raylib::Button::key(KEY_LEFT) }, //button to move car left
            { raylib::Button::key(KEY_D), raylib::Button::key(KEY_RIGHT) } //button to move car right
        ).move();

        input["sideways"].AddCallback([this](float state, float change) { // sideways input
            if (!meshRenderComponent->selected) return; // If callback input is not selected, return nothing

            if (auto compOpt = Object().GetComponent<PhysicsComponent>()) { // If PhysicsComponent exists.
                compOpt.value().get().rotation = (state > 0) ? compOpt.value().get().turnSpeed : // If state is positive and greater than 0, rotation function in the physicsComponent will be called and the entity will turn right.
                                                 (state < 0) ? -compOpt.value().get().turnSpeed : 0; // If state is negative and less than 0, rotation function in the physicsComponent will be called and the entity will turn left.
            }
        });

        input["reset"] = raylib::Action::button({ raylib::Button::key(KEY_SPACE) }); // reset input
        input["reset"].AddCallback([this](float state, float change) { // reset input
            if (!meshRenderComponent->selected) return; // If callback input is not selected, return nothing.

            if (state > 0) { // If state is positive and greater than 0.
                if (auto compOpt = Object().GetComponent<PhysicsComponent>()) { // If PhysicsComponent exists.
                    compOpt.value().get().isBraking = true; // get the isBraking flag and set it to true (entity will start slowing down to a stop).
                }
            }
        });
    }

    void Tick(float dt) override { // Tick
        input.PollEvents(); // Poll events.
    }
};

template<typename T>
struct delegate{};

template<typename Treturn, typename... Targs>
struct delegate<Treturn(Targs...)>: public std::vector<std::function<Treturn(Targs...)>> {

    void operator()(Targs... args){
        for(auto& f: *this)
            f(args...);
    }
};

int main(){
    raylib::Window window(800, 600, "CS381 - Assignment6");
    window.SetState(FLAG_WINDOW_RESIZABLE);
    auto camera = raylib::Camera(
        {0,150,550},
        {0, 0, 0},
        {0, 1, 0},
        45
    );

    auto rocket1 = raylib::Model("meshes/rocket.glb");
    rocket1.transform = raylib::Matrix::Identity().Scale(30);
    auto rocket2 = raylib::Model("meshes/rocket.glb");
    rocket2.transform = raylib::Matrix::Identity().Scale(30);
    auto rocket3 = raylib::Model("meshes/rocket.glb");
    rocket3.transform = raylib::Matrix::Identity().Scale(30);
    auto rocket4 = raylib::Model("meshes/rocket.glb");
    rocket4.transform = raylib::Matrix::Identity().Scale(30);
    auto rocket5 = raylib::Model("meshes/rocket.glb");
    rocket5.transform = raylib::Matrix::Identity().Scale(30);

    auto sedan = raylib::Model("meshes/sedan.glb");
    sedan.transform = raylib::Matrix::Identity().Scale(30);
    raylib::Texture2D carTexture("meshes/Textures/colormap.png");

    auto ambulance = raylib::Model("meshes/ambulance.glb");
    ambulance.transform = raylib::Matrix::Identity().Scale(30);

    auto police = raylib::Model("meshes/police.glb");
    police.transform = raylib::Matrix::Identity().Scale(30);

    auto firetruck = raylib::Model("meshes/firetruck.glb");
    firetruck.transform = raylib::Matrix::Identity().Scale(30);

    auto taxi = raylib::Model("meshes/taxi.glb");
    taxi.transform = raylib::Matrix::Identity().Scale(30);

    raylib::Model grass = raylib::Mesh::Plane(1000, 1000, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    cs381::SkyBox sky("textures/skybox.png");

    std::vector<cs381::Entity> entities;
    int selectedIndex = 0;
    
    entities.emplace_back().AddComponent<MeshRenderComponent>(&rocket1, true, raylib::Vector3{-100.0f, 0.0f, -200.0f}, true);
    auto& rocket1Mesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(100.0f,15.0f,5.0f,true);
    entities.back().AddComponent<InputComponent>(&rocket1Mesh);
  
    entities.emplace_back().AddComponent<MeshRenderComponent>(&rocket2, true, raylib::Vector3{-100.0f, 0.0f, -100.0f}, false);
    auto& rocket2Mesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(100.0f,15.0f,5.0f,true);
    entities.back().AddComponent<InputComponent>(&rocket2Mesh);
 
    entities.emplace_back().AddComponent<MeshRenderComponent>(&rocket3, true, raylib::Vector3{-100.0f, 0.0f, 0.0f}, false);
    auto& rocket3Mesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(100.0f,15.0f,5.0f,true);
    entities.back().AddComponent<InputComponent>(&rocket3Mesh);
    
    entities.emplace_back().AddComponent<MeshRenderComponent>(&rocket4, true, raylib::Vector3{-100.0f, 0.0f, 100.0f}, false);
    auto& rocket4Mesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(100.0f,15.0f,5.0f,true);
    entities.back().AddComponent<InputComponent>(&rocket4Mesh);
    
    entities.emplace_back().AddComponent<MeshRenderComponent>(&rocket5, true, raylib::Vector3{-100.0f, 0.0f, 200.0f}, false);
    auto& rocket5Mesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(100.0f,15.0f,5.0f,true);
    entities.back().AddComponent<InputComponent>(&rocket5Mesh);
    
    entities.emplace_back().AddComponent<MeshRenderComponent>(&sedan, false, raylib::Vector3{0.0f, 0.0f, 0.0f}, false, 90.0f);
    auto& sedanMesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(35.0f,8.0f,6.0f,false);
    entities.back().AddComponent<InputComponent>(&sedanMesh);

    entities.emplace_back().AddComponent<MeshRenderComponent>(&ambulance, false, raylib::Vector3{0.0f, 0.0f, 200.0f}, false, 90.0f);
    auto& ambulanceMesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(100.0f,15.0f,15.0f,false);
    entities.back().AddComponent<InputComponent>(&ambulanceMesh);

    entities.emplace_back().AddComponent<MeshRenderComponent>(&police, false, raylib::Vector3{0.0f, 0.0f, 100.0f}, false, 90.0f);
    auto& policeMesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(50.0f,10.0f,10.0f,false);
    entities.back().AddComponent<InputComponent>(&policeMesh);

    entities.emplace_back().AddComponent<MeshRenderComponent>(&firetruck, false, raylib::Vector3{0.0f, 0.0f, -200.0f}, false, 90.0f);
    auto& firetruckMesh= entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(40.0f,7.0f,5.0f,false);
    entities.back().AddComponent<InputComponent>(&firetruckMesh);

    entities.emplace_back().AddComponent<MeshRenderComponent>(&taxi, false, raylib::Vector3{0.0f, 0.0f, -100.0f}, false, 90.0f);
    auto& taxiMesh = entities.back().GetComponent<MeshRenderComponent>().value().get();
    entities.back().AddComponent<PhysicsComponent>(45.0f,9.0f,7.0f,false);
    entities.back().AddComponent<InputComponent>(&taxiMesh);

    cs381::Entity* selectedEntity = &entities[selectedIndex];

    while(!window.ShouldClose()){
        if (raylib::Mouse::IsButtonPressed(MOUSE_LEFT_BUTTON)) {
            raylib::Ray ray = camera.GetMouseRay(raylib::Mouse::GetPosition());
            
            for (size_t i = 0; i < entities.size(); ++i) {
                if (auto compOpt = entities[i].GetComponent<MeshRenderComponent>()) {
                    MeshRenderComponent& meshComp = compOpt.value().get();
                    raylib::Model* model = meshComp.model;
                    if (!model) continue;
                    
                    auto transformer = [&](raylib::Matrix m) {
                        auto& transform = meshComp.Object().Transform();
                        return meshComp.isRocket
                            ? m.Translate(transform.position).RotateZ(static_cast<float>(transform.heading) * DEG2RAD)
                            : m.Translate(transform.position).RotateY(static_cast<float>(transform.heading) * DEG2RAD);
                    };
                    
                    raylib::Matrix entityOriginal = model->transform;
                    model->transform = transformer(entityOriginal);
                    BoundingBox bbox = model->GetTransformedBoundingBox();
                    model->transform = entityOriginal;
                    
                    RayCollision collision = GetRayCollisionBox(ray, bbox);
                    if (collision.hit) {
                        if (selectedEntity != nullptr && selectedEntity != &entities[i]) {
                            if (auto compOptPrev = selectedEntity->GetComponent<MeshRenderComponent>()) {
                                compOptPrev.value().get().setSelected(false);
                            }
                        }
                        selectedIndex = i;
                        selectedEntity = &entities[i];
                        if (auto compOptNew = selectedEntity->GetComponent<MeshRenderComponent>()) {
                            compOptNew.value().get().setSelected(true);
                        }
                        break;
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_TAB)) {
            if (auto compOpt = selectedEntity->GetComponent<MeshRenderComponent>()) {
                compOpt.value().get().setSelected(false);
            }
    
            selectedIndex = (selectedIndex + 1) % entities.size();
            selectedEntity = &entities[selectedIndex];
            
            if (auto compOpt = selectedEntity->GetComponent<MeshRenderComponent>()) {
                compOpt.value().get().setSelected(true);
            }
        }
        
        window.BeginDrawing();
            camera.BeginMode();
                window.ClearBackground(BLACK);
                sky.Draw();
                grass.Draw({});

                auto dt = window.GetFrameTime();
                for (auto& e : entities){
                    e.Tick(dt);
                }

            camera.EndMode();
        window.EndDrawing();
    }
    return 0;
}