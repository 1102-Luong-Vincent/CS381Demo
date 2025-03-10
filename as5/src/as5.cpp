#include <raylib-cpp.hpp>
#include <vector>
#include "skybox.hpp"
#include "CO.hpp"

cs381::Entity* selectedEntity = nullptr;

void DrawBoundedModel(raylib::Model& model, auto transformer) {
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.GetTransformedBoundingBox().Draw();
    model.transform = backup;
}

void DrawModel(raylib::Model& model, auto transformer) {
    raylib::Matrix backup = model.transform;
    model.transform = transformer(backup);
    model.Draw({});
    model.transform = backup;
}

struct MeshRenderComponent : cs381::Component {
    raylib::Model* model = nullptr;

    MeshRenderComponent(cs381::Entity& e, raylib::Model* model)
        : cs381::Component(e), model(model) {}

    void Tick(float dt) override {
        cs381::Component::Tick(dt);
        if (model == nullptr) return;

        DrawModel(*model, [this](raylib::Matrix& matrix) {
            auto& transform = Object().Transform();
            return matrix.Translate(transform.position).RotateY(transform.heading);
        });

        if (&Object() == selectedEntity) {
            DrawBoundedModel(*model, [this](raylib::Matrix& matrix) {
                auto& transform = Object().Transform();
                return matrix.Translate(transform.position).RotateY(transform.heading);
            });
        }
    }
};

struct PhysicsComponent : cs381::Component {
    float speed = 0.0f; // Speed of the entity
    bool isRocket = false; // Whether the entity is a rocket
    raylib::Vector3 velocity = raylib::Vector3::Zero(); // Velocity vector

    PhysicsComponent(cs381::Entity& e, bool isRocketCheck)
        : cs381::Component(e), isRocket(isRocketCheck) {}

    void Tick(float dt) override {
        cs381::Component::Tick(dt);
        auto& transform = Object().Transform();

        // Calculate movement direction based on heading
        float radians = (float)transform.heading * DEG2RAD; // Convert heading to radians

        if (isRocket) {
            // Rocket moves in 3D space
            velocity.x = sin(radians) * speed; // X-axis movement
            //velocity.y = 0.0f; // Y-axis movement (controlled separately)
            velocity.y = cos(radians) * speed; // Z-axis movement
        } else {
            // Car moves in XZ plane
            velocity.x = sin(radians) * speed;
            velocity.z = cos(radians) * speed;
        }

        // Update position based on velocity
        transform.position += velocity * dt;

        // Prevent rocket from going below ground
        if (isRocket && transform.position.y < 0.0f) {
            transform.position.y = 0.0f;
            velocity.y = 0.0f;
        }
    }

    void IncreaseSpeed(float amount) {
        speed += amount;
    }

    void DecreaseSpeed(float amount) {
        speed -= amount;
    }

    void Rotate(float angle) {
        Object().Transform().heading += angle;
    }

    void ResetSpeed() {
        speed = 0.0f;
        velocity = raylib::Vector3::Zero(); // Explicitly construct a Vector3
    }

    void MoveUp(float amount) {
        if (isRocket) {
            velocity.z += amount;
        }
    }

    void MoveDown(float amount) {
        if (isRocket) {
            velocity.z -= amount;
        }
    }
};

void HandleMouseSelection(raylib::Camera& camera, cs381::Entity* entities, int maxEntities) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Get the mouse position
        raylib::Vector2 mousePosition = GetMousePosition();

        // Cast a ray from the mouse position into the 3D world
        raylib::Ray ray = camera.GetMouseRay(mousePosition);

        // Check for collisions with each entity
        for (int i = 0; i < maxEntities; i++) {
            auto meshRender = entities[i].GetComponent<MeshRenderComponent>();
            if (meshRender && meshRender->get().model) {
                // Get the model's transformation matrix
                raylib::Matrix transform = MatrixMultiply(
                    MatrixTranslate(meshRender->get().Object().Transform().position.x,
                                    meshRender->get().Object().Transform().position.y,
                                    meshRender->get().Object().Transform().position.z),
                    MatrixRotateY(static_cast<float>(meshRender->get().Object().Transform().heading) * DEG2RAD)
                );

                // Transform the bounding box
                raylib::BoundingBox bbox = meshRender->get().model->GetBoundingBox();
                bbox.min = Vector3Transform(bbox.min, transform);
                bbox.max = Vector3Transform(bbox.max, transform);

                // Check for collision between the ray and the bounding box
                raylib::RayCollision collision = ray.GetCollision(bbox);

                if (collision.hit) {
                    // Select the entity if a collision is detected
                    selectedEntity = &entities[i];
                    break;
                }
            }
        }
    }
}

int main() {
    raylib::Window window(800, 600, "CS381 - Assignment 5");

    // Setup camera
    auto camera = raylib::Camera(
        {0, 80, -200}, 
        {0, 20, 0},  
        {0, 1 ,0},
        45,
        CAMERA_PERSPECTIVE
    );

    // Load models
    raylib::Model carModel("meshes/sedan.glb");
    carModel.transform = raylib::Matrix::Identity().Scale(15);
    raylib::Model rocketModel("meshes/rocket.glb");
    rocketModel.transform = raylib::Matrix::Identity().Scale(15);

    // Initialize entities
    const int MAX_ENTITIES = 3;
    cs381::Entity entities[MAX_ENTITIES];
    int selectedIndex = 0;

    // Car 1
    entities[0].AddComponent<MeshRenderComponent>(&carModel);
    entities[0].AddComponent<PhysicsComponent>(false);
    entities[0].Transform().position = raylib::Vector3{-30.0f, 0.0f, 0.0f};

    // Car 2
    entities[1].AddComponent<MeshRenderComponent>(&carModel);
    entities[1].AddComponent<PhysicsComponent>(false);
    entities[1].Transform().position = raylib::Vector3{30.0f, 0.0f, 0.0f};

    // Rocket
    entities[2].AddComponent<MeshRenderComponent>(&rocketModel);
    entities[2].AddComponent<PhysicsComponent>(true);
    entities[2].Transform().position = raylib::Vector3{0.0f, 0.0f, 0.0f};

    selectedEntity = &entities[selectedIndex];

    cs381::SkyBox sky("textures/skybox.png");

    Texture2D carTexture = LoadTexture("Textures/colormap.png");
    raylib::Model grass = raylib::Mesh::Plane(500, 500, 1, 1).LoadModelFrom();
    raylib::Texture grassTexture = raylib::Texture("../assets/textures/grass.jpg");
    grass.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;

    while (!window.ShouldClose()) {
        float dt = GetFrameTime();

        // Selection system
        if (IsKeyPressed(KEY_TAB)) {
            selectedIndex = (selectedIndex + 1) % MAX_ENTITIES;
            selectedEntity = &entities[selectedIndex];
        }

        // Mouse selection
        HandleMouseSelection(camera, entities, MAX_ENTITIES);

        auto physics = selectedEntity->GetComponent<PhysicsComponent>();
        if (physics) {
            if(raylib::Keyboard::IsKeyPressed(KEY_W)){
                physics->get().IncreaseSpeed(5.0f);
            } 
            if(raylib::Keyboard::IsKeyPressed(KEY_S)){
                physics->get().DecreaseSpeed(5.0f);
            } 
            if(raylib::Keyboard::IsKeyPressed(KEY_A)){
                physics->get().Rotate(5.0f);
            } 
            if(raylib::Keyboard::IsKeyPressed(KEY_D)){
                physics->get().Rotate(-5.0f);
            }
            if(raylib::Keyboard::IsKeyPressed(KEY_SPACE)){
                physics->get().ResetSpeed();
            } 

            // Rocket-specific controls for 3D movement
            if (physics->get().isRocket){
                if (raylib::Keyboard::IsKeyPressed(KEY_UP)){
                    physics->get().MoveUp(5.0f); // Move up
                } 
                if (raylib::Keyboard::IsKeyPressed(KEY_DOWN)){
                    physics->get().MoveDown(5.0f); // Move down
                } 
            }
        }

        for (int i = 0; i < MAX_ENTITIES; i++) {
            entities[i].Tick(dt);
        }

        for (int i = 0; i < MAX_ENTITIES; i++) {
            auto physics = entities[i].GetComponent<PhysicsComponent>();
        }

        window.BeginDrawing();
        window.ClearBackground(RAYWHITE);
        camera.BeginMode();

        sky.Draw();
        grass.Draw({});

        for (int i = 0; i < MAX_ENTITIES; i++) {
            entities[i].Tick(dt);
        }

        camera.EndMode();
        window.EndDrawing();
    }
}
