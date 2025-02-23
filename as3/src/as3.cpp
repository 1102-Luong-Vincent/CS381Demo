#include <iostream>
#include <raylib-cpp.hpp>
#include "skybox.hpp"
#include "color.hpp"

void DrawBoundedModel(raylib::Model& model, auto transformer) {
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.transform = backup;
}

int main() {
    raylib::Window window(800, 600, "CS381 - Assignment 3");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    raylib::Vector3 frontWheelLeft  = { -10.0f, -8.0f,  17.0f };
    raylib::Vector3 frontWheelRight = {  10.0f, -8.0f,  17.0f };
    raylib::Vector3 backWheelLeft   = { -10.0f, -8.0f, -17.0f };
    raylib::Vector3 backWheelRight  = {  10.0f, -8.0f, -17.0f };

    cs381::SkyBox sky("textures/skybox.png");
    raylib::Model model("../assets/Kenny Car Kit/race.glb");
    model.transform = raylib::Matrix::Identity().Scale(5).RotateY(90 * DEG2RAD);

    float coneHeading = -90.0f;  // Facing left
    float coneSpeed = 0.0f;
    float speedIncrease = 2.0f;
    float maxSpeed = 25.0f;
    float angleStep = 1.0f;
    float coneLift = 15.0f;
    bool isFlying = false;
    
    raylib::Model grass = raylib::Mesh::Plane(100, 100, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    raylib::Vector3 conePosition = {};
    
    // Camera Setup
    float cameraDistance = 45.0f; // Distance from car
    float cameraHeight = 15.0f;    // Height above car
    float cameraAngle = 180.0f;   // Initial angle behind car
    float cameraRotation = 1.0f;
    raylib::Camera3D camera(raylib::Vector3{0.0f, cameraHeight, cameraDistance}, raylib::Vector3{0.0f, 0.0f, 0.0f}, raylib::Vector3{0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE);

    while (!window.ShouldClose()) {
        
        // Handle speed control
        if (raylib::Keyboard::IsKeyDown(KEY_W)) {
            coneSpeed += speedIncrease;
            if (coneSpeed > maxSpeed){
                coneSpeed = maxSpeed;
            } 
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_S)) {
            coneSpeed -= speedIncrease;
            if (coneSpeed < 0){
                coneSpeed = 0;
            }
        }
        if (raylib::Keyboard::IsKeyDown(KEY_A)) {
            coneHeading += angleStep;
        }
        if (raylib::Keyboard::IsKeyDown(KEY_D)) {
            coneHeading -= angleStep;
        }
        if (raylib::Keyboard::IsKeyPressed(KEY_SPACE)) {
            coneSpeed = 0;
        }

        if(raylib::Keyboard::IsKeyDown(KEY_Q)){
            conePosition.y += coneLift * window.GetFrameTime();
            isFlying = true;
        }
        if(raylib::Keyboard::IsKeyDown(KEY_E)){
            conePosition.y -= coneLift * window.GetFrameTime();
            if(conePosition.y <= 0){
                conePosition.y = 0;
                isFlying = false;
            }
        }
        // **Calculate Forward Movement**
        raylib::Vector3 forwardDirection = {
            cos(coneHeading * DEG2RAD),  // Negative cos() aligns properly
            0,
            -sin(coneHeading * DEG2RAD)   // sin() aligns properly for forward motion
        };

        // Apply movement
        conePosition = conePosition + forwardDirection * (coneSpeed * window.GetFrameTime());

        // **Camera Controls**
        if (raylib::Keyboard::IsKeyDown(KEY_LEFT)) {
            cameraAngle += cameraRotation * window.GetFrameTime();  // Rotate left
        }
        if (raylib::Keyboard::IsKeyDown(KEY_RIGHT)) {
            cameraAngle -= cameraRotation * window.GetFrameTime();  // Rotate right
        }
        if (raylib::Keyboard::IsKeyDown(KEY_UP)) {
            cameraDistance -= 0.2f;  // Zoom in
            if (cameraDistance < 5.0f) cameraDistance = 5.0f;
        }
        if (raylib::Keyboard::IsKeyDown(KEY_DOWN)) {
            cameraDistance += 0.2f;  // Zoom out
            if (cameraDistance > 30.0f) cameraDistance = 30.0f;
        }

        // Update Camera Position
        camera.position = {
            conePosition.x + sin(raylib::Degree(cameraAngle)) * cameraDistance,
            conePosition.y + cameraHeight,
            conePosition.z + cos(raylib::Degree(cameraAngle)) * cameraDistance
        };
        camera.target = conePosition;  // Always look at the car

        // Render Scene
        window.BeginDrawing();
        window.ClearBackground(raylib::Color::Black());

        camera.BeginMode();
            sky.Draw();
            grass.Draw({});
            DrawBoundedModel(model, [&conePosition, &coneHeading](raylib::Transform t) {
                 return t.Translate(conePosition).RotateY(raylib::Degree(coneHeading));
            });

        camera.EndMode();
        camera.EndMode();

        window.EndDrawing();
    }

    return 0;
}