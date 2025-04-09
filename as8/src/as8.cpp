#include <iostream>
#include "raylib-cpp.hpp"
#include "skybox.hpp"
#include "ECS.hpp"

cs381::Entity selectedEntity = 0;
size_t globalComponentCounter = 0;

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

struct MeshRenderComponent{
    raylib::Model* model = nullptr;
    bool showBoundingBox = false;

};

struct Physics2DComponent{
    float heading;
    float speed;
};

struct OrientedPhysics2DComponent{
    float heading;
    float speed;
};

struct Physics3DComponent{
    float heading;
    float speed;
};

struct OrientedPhysics3DComponent{
    raylib::Quaternion rotation;
    raylib::Vector3 velocity;
};

struct TransformComponent{
    raylib::Vector3 position;
    float heading;
    raylib::Vector3 scale = raylib::Vector3{1.0f, 1.0f, 1.0f};
};

struct BufferedInputComponent{
    bool velocity = false;
    bool decreaseVelocity = false;
    bool headingLeft = false;
    bool headingRight = false;
    bool pitchUp = false;
    bool pitchDown = false;
    bool rollLeft = false;
    bool rollRight = false;
    bool stop = false;
};

struct SelectionManagementSystem{
    int selectedIndex = 0;
};
using entity = int;

struct Scene{ //holds all the components and systems.
    struct PhysicsComponent{ //holds the physics components for each entity.
        raylib::Vector3 position; //position of the entity.
        raylib::Vector3 velocity; //velocity of the entity.
    };
    PhysicsComponent physics[10]; //holds the physics components for each entity.
    float heading[10]; //holds the heading of the entity.
    raylib::Model* model[10]; //holds the model of the entity.
    MeshRenderComponent render; //holds the render component for each entity.
    TransformComponent transforms; //holds the transform component for each entity.
    OrientedPhysics2DComponent physics2D; //holds the 2D physics component for each entity.
    OrientedPhysics3DComponent physics3D; //holds the 3D physics component for each entity.

};
auto x = sizeof(Vector3); //size of the Vector3 struct.

void kinematic_physics_system(Scene& scene, float dt){ //updates the position of the entity based on the velocity and dt.
    for(int e = 0; e < 10; e++){ //for each entity in the scene.
        scene.physics[e].position += scene.physics[e].velocity * dt; //updates the position of the entity based on the velocity and dt.
    }
}

void RenderSystem(cs381::Scene<cs381::ComponentStorage>& scene){ //renders the models in the scene.
    for (cs381::Entity e = 0; e < scene.entityMasks.size(); ++e) { //for each entity in the scene.
        if (!scene.HasComponent<TransformComponent>(e) || !scene.HasComponent<MeshRenderComponent>(e)) { //if the entity does not have a transform or render component, skip it.
            continue; //skip the entity.
        }

        auto& transform = scene.GetComponent<TransformComponent>(e); //call and get the transform component of the entity.
        auto& render = scene.GetComponent<MeshRenderComponent>(e); //call and get the transform and render components of the entity.

        if (!render.model){ //if the model is null, skip it.
            continue; //skip the entity.
        }

        raylib::Matrix transformMat = raylib::Matrix::Identity() //create a new matrix.
            .Translate(transform.position) //translate the matrix to the position of the entity.
            .RotateY(transform.heading * DEG2RAD) //rotate the matrix based on the heading of the entity.
            .Scale(transform.scale.x, transform.scale.y, transform.scale.z); //scale the matrix based on the scale of the entity.

        if(scene.HasComponent<OrientedPhysics3DComponent>(e)){ //if the entity has a 3D physics component, get the rotation of the entity.
            auto& physics3D = scene.GetComponent<OrientedPhysics3DComponent>(e); //get the 3D physics component of the entity.
            transformMat = transformMat * physics3D.rotation.ToMatrix(); //multiply the matrix by the rotation of the entity.
        }

        raylib::Model& model = *render.model; //get the model of the entity.
        raylib::Matrix backup = model.transform; //create a backup of the model transform.
        model.transform = transformMat; //set the model transform to the new matrix.
        model.Draw({}); //draw the model.

        if (render.showBoundingBox){ //if the entity has a bounding box, draw it.
            DrawBoundingBox(model.GetTransformedBoundingBox(), WHITE); //draw the bounding box of the model.
        }
        
        model.transform = backup; //set the model transform back to the backup.
    }
}

void Physics2DSystem(cs381::Scene<cs381::ComponentStorage>& scene, float dt){ //updates the position of the entity based on the velocity and dt.
    for(cs381::Entity e = 0; e < scene.entityMasks.size(); e++){ //for each entity in the scene.

        if (!scene.HasComponent<TransformComponent>(e) || !scene.HasComponent<OrientedPhysics2DComponent>(e)){ //if the entity does not have a transform or render component, skip it.
            continue; //skip the entity.
        }

        auto& transform = scene.GetComponent<TransformComponent>(e); //call and get the transform component of the entity.
        auto& physics = scene.GetComponent<OrientedPhysics2DComponent>(e); //call and get the physics component of the entity.

        float rad = DEG2RAD * physics.heading; //convert the heading to radians.
        raylib::Vector3 forward = {std::sin(rad), 0, std::cos(rad)}; //create a new vector based on the heading of the entity.
        transform.position += forward * physics.speed * dt; //update the position of the entity based on the velocity and dt.
        transform.heading = physics.heading; //update the heading of the entity.

        if(transform.position.y < 0.0f){ //added collision for rocket models so they don't go underneath.
            transform.position.y = 0; //set the position of the entity to 0.
        }
    }
}

void Physics3DSystem(cs381::Scene<cs381::ComponentStorage>& scene, float dt){ //updates the position of the entity based on the velocity and dt.
    for (cs381::Entity e = 0; e < scene.entityMasks.size(); ++e) { //for each entity in the scene.
      
        if (!scene.HasComponent<TransformComponent>(e) || !scene.HasComponent<OrientedPhysics3DComponent>(e)){ //if the entity does not have a transform or physics component, skip it.
            continue; //skip the entity.
        }

        auto& transform = scene.GetComponent<TransformComponent>(e); //call and get the transform component of the entity.
        auto& physics = scene.GetComponent<OrientedPhysics3DComponent>(e); //call and get the transform and render components of the entity.

        raylib::Matrix rotationMatrix = physics.rotation.ToMatrix(); //get the rotation of the entity.
 
        raylib::Vector3 forward = raylib::Vector3{0.0f, 0.0f, 1.0f}.Transform(rotationMatrix); //create a new vector based on the rotation of the entity.

        //raylib::Vector3 forward = raylib::Vector3{0.0f, 0.0f, 1.0f}.Transform(rotationVelocity);
        
        transform.position += forward * physics.velocity.x * dt; //update the position of the entity based on the velocity and dt.
        //transform.position += physics.velocity * dt;

        //transform.position += rotationVelocity* dt;

        // if(physics.velocity.y < 0.0f){
        //     physics.velocity.y = 0;
        // }
        if(transform.position.y < 0.0f){ //added collision where car models don't go underneath
            transform.position.y = 0; //set the y position of the entity to 0. 
            physics.velocity.y = 0; //set the y position of the physics to 0 so the model doesn't go underneath the ground. 
        }
    }
}

void BufferedInputUpdate(cs381::Scene<cs381::ComponentStorage>&scene){
    if(!scene.HasComponent<BufferedInputComponent>(selectedEntity)) return;

        auto& input = scene.GetComponent<BufferedInputComponent>(selectedEntity);

        input.velocity = IsKeyPressed(KEY_W);
        input.decreaseVelocity = IsKeyPressed(KEY_S);
        input.headingLeft = IsKeyPressed(KEY_A);
        input.headingRight = IsKeyPressed(KEY_D);
        input.pitchUp = IsKeyPressed(KEY_R);
        input.pitchDown = IsKeyPressed(KEY_F);
        input.rollLeft = IsKeyPressed(KEY_Q);
        input.rollRight = IsKeyPressed(KEY_E);
        input.stop = IsKeyPressed(KEY_SPACE);
}
// void InputComponent(cs381::Scene<cs381::ComponentStorage>& scene){
//     auto phys2D = scene.GetComponent<OrientedPhysics2DComponent>(selectedEntity);
//     auto phys3D = scene.GetComponent<OrientedPhysics3DComponent>(selectedEntity);

//     if(scene.HasComponent<OrientedPhysics2DComponent>(selectedEntity)){
//         auto& physics = scene.GetComponent<OrientedPhysics2DComponent>(selectedEntity);

//         if(IsKeyPressed(KEY_W)){
//             physics.speed += 5.0f;
//         }  
//         if(IsKeyPressed(KEY_S)){
//             physics.speed -= 5.0f;        
//         }  
//         if(IsKeyPressed(KEY_A)){
//             physics.heading -= 5.0f;
//         }
//         if(IsKeyPressed(KEY_D)){ 
//             physics.heading += 5.0f;
//         }
//         if(IsKeyPressed(KEY_SPACE)){
//             physics.speed = 0;
//         }
//     }

//     //pitch 
//     if(scene.HasComponent<OrientedPhysics3DComponent>(selectedEntity)){
//         auto& physics = scene.GetComponent<OrientedPhysics3DComponent>(selectedEntity);
//         auto& q = physics.rotation;
//         raylib::Vector3 euler = q.ToEuler();

//         // if(IsKeyPressed(KEY_R)){ //increases the entity's pitch
//         //     q = raylib::Quaternion::FromEuler(euler + raylib::Vector3{DEG2RAD, 0, 0});
//         // }
//         // if(IsKeyPressed(KEY_F)){ //decreases the entity's pitch
//         //     q = raylib::Quaternion::FromEuler(euler - raylib::Vector3(DEG2RAD, 0, 0));
//         // }
//         // if(IsKeyPressed(KEY_Q)){
//         //     q = raylib::Quaternion::FromEuler(euler + raylib::Vector3{0, 0, DEG2RAD});
//         // }
//         // if(IsKeyPressed(KEY_E)){
//         //     q = raylib::Quaternion::FromEuler(euler - raylib::Vector3{0, 0, DEG2RAD});
//         // }

//         if (IsKeyPressed(KEY_R)) { // Pitch up (tilt)
//             euler.x += DEG2RAD * 400.0f * GetFrameTime();
//         }
//         if (IsKeyPressed(KEY_F)) { // Pitch down (tilt)
//             euler.x -= DEG2RAD * 400.0f * GetFrameTime();
//         }
//         if (IsKeyPressed(KEY_Q)) { // Roll left 
//             euler.z += DEG2RAD * 400.0f * GetFrameTime();
//         }
//         if (IsKeyPressed(KEY_E)) { // Roll right
//             euler.z -= DEG2RAD * 400.0f * GetFrameTime();
//         }

//         physics.rotation = raylib::Quaternion::FromEuler(euler);

//         // Thrust control
//         if (IsKeyPressed(KEY_W)){  //move forward
//             //physics.velocity.x += 5.0f;
//             physics.velocity.z += 5.0f * GetFrameTime();
//             // raylib::Vector3 localForward = { 0, 0, 1 };
//             // raylib::Vector3 worldForward = physics.rotation.RotateVector(localForward);
//             // physics.velocity += worldForward * 50.0f * GetFrameTime(); // 50 is thrust strength
//         }
//         if (IsKeyPressed(KEY_S)){ //slow down movement
//             //physics.velocity.x -= 5.0f;
//             physics.velocity.z -= 5.0f * GetFrameTime();
//             // raylib::Vector3 localForward = { 0, 0, 1 };
//             // raylib::Vector3 worldForward = localForward.Transform(physics.rotation.ToMatrix());
//             // physics.velocity -= worldForward * 50.0f * GetFrameTime(); // 50 is thrust strength
//         }
//         if (IsKeyPressed(KEY_SPACE)){ //stop movement
//             physics.velocity = raylib::Vector3{0.0f, 0.0f, 0.0f};
//         }
//     }
// };

void InputComponent(cs381::Scene<cs381::ComponentStorage>& scene){
    if(!scene.HasComponent<BufferedInputComponent>(selectedEntity)) return;

        auto& input = scene.GetComponent<BufferedInputComponent>(selectedEntity);

        if(scene.HasComponent<OrientedPhysics2DComponent>(selectedEntity)){
            auto& physics = scene.GetComponent<OrientedPhysics2DComponent>(selectedEntity);

            if(input.velocity){
                physics.speed += 5.0f;
            }  //w
            if(input.decreaseVelocity){
                physics.speed -= 5.0f;
            } //s
            if(input.headingLeft){
                physics.heading -= 5.0f;
            }  //a
            if(input.headingRight){
                physics.heading += 5.0f;
            } //d
            if(input.stop){
                physics.speed = 0.0f;
            } //space
        }

        if(scene.HasComponent<OrientedPhysics3DComponent>(selectedEntity)){
            auto& physics = scene.GetComponent<OrientedPhysics3DComponent>(selectedEntity);
            auto& q = physics.rotation;
            raylib::Vector3 euler = q.ToEuler();
            
            if(input.pitchUp){
                euler.x += DEG2RAD * 400.0f * GetFrameTime();
            }  //r
            if(input.pitchDown){
                euler.x -= DEG2RAD * 400.0f * GetFrameTime();
            }   //f 
            if(input.rollLeft){
                euler.z += DEG2RAD * 400.0f * GetFrameTime();
            } //q
            if(input.rollRight){
                euler.z -= DEG2RAD * 400.0f * GetFrameTime();
            }  //e

        physics.rotation = raylib::Quaternion::FromEuler(euler);

        raylib::Vector3 forward = raylib::Vector3{0.0f, 0.0f, 1.0f}.Transform(physics.rotation.ToMatrix());

        if(input.velocity){
            physics.velocity += forward * 100.0f * GetFrameTime();
        }
        if(input.decreaseVelocity){
            physics.velocity -= forward * 100.0f * GetFrameTime();
        }
        if(input.stop){
            physics.velocity = raylib::Vector3{0.0f, 0.0f, 0.0f};
        }
    }    
}

cs381::Entity CreateRocketEntity(cs381::Scene<cs381::ComponentStorage>& scene, raylib::Model* modelPtr, raylib::Vector3 position = {}, float heading = 0.0f) {
    auto e = scene.CreateEntity(); //create a new entity by calling the scene.

    auto& transform = scene.AddComponent<TransformComponent>(e); //call the scene and add a transform component to the entity.
    transform.position = position;
    transform.heading = heading;
    transform.scale = raylib::Vector3{30.0f, 30.0f, 30.0f}; //scales all the rockets to 30, since im using one rocket model.

    auto& render = scene.AddComponent<MeshRenderComponent>(e);
    render.model = modelPtr;
    render.showBoundingBox = true;

    auto& physics2D = scene.AddComponent<OrientedPhysics2DComponent>(e);
    physics2D.heading = heading;
    physics2D.speed = 0.0f;

    auto& physics3D = scene.AddComponent<OrientedPhysics3DComponent>(e);
    physics3D.rotation = raylib::Quaternion::Identity();
    physics3D.velocity = raylib::Vector3(0.0f, 0.0f, 0.0f);

    scene.AddComponent<BufferedInputComponent>(e);

    return e;
}

cs381::Entity CreateCarEntity(cs381::Scene<cs381::ComponentStorage>& scene, raylib::Model* modelPtr, raylib::Vector3 position = {}, float heading = 0.0f){
    auto e = scene.CreateEntity(); //create a new entity by calling the scene.
 
    auto& transform = scene.AddComponent<TransformComponent>(e); //call the scene and add a transform component to the entity.
    transform.position = position;
    transform.heading = heading;
    transform.scale = raylib::Vector3{30.0f, 30.0f, 30.0f}; //scales all the cars to 30. 

    auto& render = scene.AddComponent<MeshRenderComponent>(e);
    render.model = modelPtr;
    render.showBoundingBox = true;

    auto& physics2D = scene.AddComponent<OrientedPhysics2DComponent>(e);
    physics2D.heading = heading;
    physics2D.speed = 0.0f;

    auto& physics3D = scene.AddComponent<OrientedPhysics3DComponent>(e);
    physics3D.rotation = raylib::Quaternion::Identity();
    physics3D.velocity = raylib::Vector3(0.0f, 0.0f, 0.0f);

    scene.AddComponent<BufferedInputComponent>(e);

    return e;
}

int main(){

    raylib::Window window(800, 600, "CS381 - Assignment 8");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    auto camera = raylib::Camera(
        {0, 120, 500},
        {0, 0, 0},
        {0, 1, 0},
        45
    );

    auto rocketModel = raylib::Model("meshes/rocket.glb"); //use one rocket model to print out all 5 rockets.
    rocketModel.transform = raylib::Matrix::Identity().Scale(30);

    auto sedan = raylib::Model("meshes/sedan.glb");
    sedan.transform = raylib::Matrix::Identity().Scale(30);

    auto ambulance = raylib::Model("meshes/ambulance.glb");
    ambulance.transform = raylib::Matrix::Identity().Scale(30);

    auto police = raylib::Model("meshes/police.glb");
    police.transform = raylib::Matrix::Identity().Scale(30);

    auto firetruck = raylib::Model("meshes/firetruck.glb");
    firetruck.transform = raylib::Matrix::Identity().Scale(30);

    auto taxi = raylib::Model("meshes/taxi.glb");
    taxi.transform = raylib::Matrix::Identity().Scale(30);

    raylib::Texture2D carTexture("meshes/Textures/colormap.png");

    cs381::SkyBox sky("textures/skybox.png");

    raylib::Model grass = raylib::Mesh::Plane(1000, 1000, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    cs381::Scene<cs381::ComponentStorage> scene; //create a new scene.
    float carZ = 0.0f;  
    float rocketZ = -100.0f;
    float spacingX = 100.0f;

    //places rocket behind each car 
    selectedEntity = CreateRocketEntity(scene, &rocketModel, {-2 * spacingX, 0, rocketZ}, 90.0f); //places rocket behind each car based on the spacingX variable.
    CreateRocketEntity(scene, &rocketModel, {-spacingX, 0, rocketZ}, 90.0f); //places rocket behind each car based on the spacingX variable.
    CreateRocketEntity(scene, &rocketModel, {0, 0, rocketZ}, 90.0f); //places rocket behind each car based on the spacingX variable.
    CreateRocketEntity(scene, &rocketModel, {spacingX, 0, rocketZ}, 90.0f); //places rocket behind each car based on the spacingX variable.
    CreateRocketEntity(scene, &rocketModel, {2 * spacingX, 0, rocketZ}, 90.0f); //places rocket behind each car based on the spacingX variable.

    CreateCarEntity(scene, &sedan, {-2 * spacingX, 0, carZ}, 90.0f); //car places car in front of each rocket based on the spacingX variable.
    CreateCarEntity(scene, &ambulance, {-spacingX, 0, carZ}, 90.0f); //calls scene and places car in front of each rocket based on the spacingX variable. 
    CreateCarEntity(scene, &police, {0, 0, carZ}, 90.0f); //calls scene and places car in front of each rocket based on the spacingX variable.
    CreateCarEntity(scene, &firetruck, {spacingX, 0, carZ}, 90.0f); //calls scene and places car in front of each rocket based on the spacingX variable.
    CreateCarEntity(scene, &taxi, {2 * spacingX, 0, carZ}, 90.0f); //calls scene and places car in front of each rocket based on the spacingX variable.

    while(!window.ShouldClose()){

        if(IsKeyPressed(KEY_TAB)){
            do{
                selectedEntity = (selectedEntity + 1) % scene.entityMasks.size();
            }
            while(!scene.HasComponent<MeshRenderComponent>(selectedEntity));
        }

        float dt = GetFrameTime();
        //InputComponent(scene);
        BufferedInputUpdate(scene);
        InputComponent(scene);
        Physics2DSystem(scene, dt);
        Physics3DSystem(scene, dt);

        for (cs381::Entity e = 0; e < scene.entityMasks.size(); ++e) {
            if (!scene.HasComponent<MeshRenderComponent>(e)){
                continue;
            }    
            auto& render = scene.GetComponent<MeshRenderComponent>(e);
            render.showBoundingBox = (e == selectedEntity);
        }

        if(scene.HasComponent<TransformComponent>(selectedEntity)){ //camera follows the selected entity.
            auto& transform = scene.GetComponent<TransformComponent>(selectedEntity);

            raylib::Vector3 currentEntity = transform.position;
            raylib::Vector3 offset = {0.0f, 150.0f, 500.0f}; //places camera behind and above the entity.

            if(scene.HasComponent<OrientedPhysics3DComponent>(selectedEntity)){
                auto& physics = scene.GetComponent<OrientedPhysics3DComponent>(selectedEntity);
                offset = offset.Transform(physics.rotation.ToMatrix());
            }
            else{
                float radians = transform.heading * DEG2RAD;
                offset.z = offset.z * std::cos(radians) - offset.x * std::sin(radians);
                offset.x = offset.z * std::sin(radians) + offset.x * std::cos(radians);
            }
            camera.position = currentEntity + offset;
            camera.target = currentEntity;
        }
        window.BeginDrawing();
        BeginMode3D(camera);
        window.ClearBackground(raylib::Color::Black());
        sky.Draw();
        grass.Draw({});

        RenderSystem(scene); //render the models in the scene.
        EndMode3D();
        window.EndDrawing();
    }
}