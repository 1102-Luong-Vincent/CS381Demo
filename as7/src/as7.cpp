#include <iostream>
#include <raylib-cpp.hpp>
#include <algorithm>
#include <memory>
#define MAX_OBSTACLES 3

float gameSpeed = 1.0f;

struct Entity;

struct Component{
    Entity* owner;

    Component(Entity* o) : owner(o) {}
    virtual void Tick(float dt){
        //return nothing
    }
    virtual ~Component() = default;
};

struct Entity{
    std::vector<std::unique_ptr<Component>> entities;

    template <typename T, typename ... Args>
    T* AddComponent(Args&&... args) {
        auto entity = std::make_unique<T>(this, std::forward<Args>(args)...);
        T* raw = entity.get();
        entities.push_back(std::move(entity));
        return raw;
    }

    template<typename T>
    T* GetComponent() {
        for (auto& e : entities)
            if (auto ptr = dynamic_cast<T*>(e.get()))
                return ptr;
        return nullptr;
    }

    void Tick(float dt){
        for(auto& entity : entities){
            entity->Tick(dt);
        }
    }
};

struct TransformComponent : Component{
    raylib::Vector3 position;
    raylib::Quaternion rotation;
    raylib::Vector3 scale;

    TransformComponent(Entity* e, raylib::Vector3 pos)
        : Component(e), position(pos), scale(raylib::Vector3(1.0f, 1.0f, 1.0f)), rotation(raylib::Quaternion::Identity()) {}

    raylib::Matrix GetMatrix() const {
        return raylib::Matrix::Identity().Scale(scale.x, scale.y, scale.z)
                                         .Rotate(rotation)
                                         .Translate(position);
    }
        
};

struct PhysicsComponent : Component{
    float gravity;
    Vector3 movement;
    Vector3 position;
    float jumpHeight;
    bool isJumping;
    
    PhysicsComponent(Entity* e)
        : Component(e), gravity(-10.0f), movement(raylib::Vector3(0.0f, 0.0f, 0.0f)),  position(raylib::Vector3(0.0f, 0.0f, 0.0f)), jumpHeight(0), isJumping(false) {}

    void Tick(float dt){
        auto transform = owner->GetComponent<TransformComponent>();
        if(!transform){
            return;
        }
        position = transform->position;

        if(isJumping){
            position.y += jumpHeight; //apply gravity to gradually fall after jumping.
            isJumping = false; 
        }

        position.y += gravity * dt;
        transform->position.y = position.y;

        if(position.y <= 0.0f){  //resets jumping to 0
            transform->position.y = 0.0f;
            position.y = 0;
            movement.y = 0;
            isJumping = false;
        }
    }
};

struct InputComponent : Component{ //handles the movement
    Vector2 position;
    bool isJumping;
    float jumpHeight, gravity, jump;
    int laneChange = 1; //start off in the middle.

    InputComponent(Entity* e)
        : Component(e), isJumping(false), jumpHeight(0), gravity(-10.0f), jump(8.0f), laneChange(1) {}
    void Tick(float dt){
        auto physics = owner->GetComponent<PhysicsComponent>();
        auto transform = owner->GetComponent<TransformComponent>();

        if(raylib::Keyboard::IsKeyPressed(KEY_A)){ //lets the character choose the left direction
            laneChange = std::max(0, laneChange - 1); //max to stop it at 0, since we don't want negative numbers
        }
        if(raylib::Keyboard::IsKeyPressed(KEY_D)){ //lets the character choose the right direction
            laneChange = std::min(2, laneChange + 1); //min to stop it at 2, since it would the last lane.
        }
        if(raylib::Keyboard::IsKeyPressed(KEY_SPACE) && transform->position.y <= 0.01f){ //lets the character jump
            physics->jumpHeight = jump;
            physics->isJumping = true;
        }
        float laneSwitch = (laneChange - 1) * 5.0f;
        transform->position.x = laneSwitch;
    }
};    

struct RendererComponent : Component{ //draw player object
    raylib::Model* model;

    RendererComponent(Entity* e, raylib::Model* m)
        : Component(e), model(m) {}

    void Tick(float dt){
        auto transform = owner->GetComponent<TransformComponent>();
        if(transform){
            model->transform = transform->GetMatrix();
            model->Draw({});
        }
    }
};

struct ObstacleComponent : Component{
    float speed = 50.0f; //speed of the obstacle coming towards player

    ObstacleComponent(Entity* e)
        : Component(e) {}

    void Tick(float dt){
        auto transform = owner->GetComponent<TransformComponent>();
        
        if(transform){
            transform->position.z += speed * gameSpeed * dt; 
        }
        if(transform->position.z > 5.0f){
            transform->position.z = GetRandomValue(-70, -30);
            int lane = GetRandomValue(0, 2);
            transform->position.x = (lane - 1) * 5.0f;
        }
    }
};
bool checkCollision(Entity* a, Entity* b){
    auto tA = a->GetComponent<TransformComponent>();
    auto tB = b->GetComponent<TransformComponent>();
    if (!tA || !tB) return false;

    BoundingBox boxA = {
        Vector3Subtract(tA->position, {2, 2, 2}),
        Vector3Add(tA->position, {2, 2, 2})
    };
    BoundingBox boxB = {
        Vector3Subtract(tB->position, {2, 2, 2}),
        Vector3Add(tB->position, {2, 2, 2})
    };

    return CheckCollisionBoxes(boxA, boxB);
}
int main(){
    raylib::Window window(800, 450, "CS 381 - Assignment 7");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    raylib::Camera camera;
    camera.position = {0, 10, 30};
    camera.target = {0, 0, 0}; 
    camera.up = {0, 1, 0};
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    raylib::Texture2D image("../assets/textures/Firelink Shrine.JPG"); //loads firelink shrine image

    Entity* player = new Entity();
    player->AddComponent<TransformComponent>(raylib::Vector3{0, 0, 0});
    player->AddComponent<PhysicsComponent>();
    player->AddComponent<InputComponent>();

    std::vector<Entity*> obstacles;
    int obstacleNum = GetRandomValue(2, 3); //generates 2 - 3 obstacles randomly.

    for(int i = 0; i < obstacleNum; i++){
        Entity* obstacle = new Entity();
        float positionX = (i - 1) * 5.0f; 
        float positionZ = GetRandomValue(-150, -30); //distance away from the player

        obstacle->AddComponent<TransformComponent>(raylib::Vector3{positionX, 0.0f, positionZ});
        obstacle->AddComponent<ObstacleComponent>();

        obstacles.push_back(obstacle);
    }


    bool gameOver = false;
    float score = 0.0f, highScore = 0.0f;
    float spawnTimer = 0.0f;
    float spawnInterval = 2.5f; // seconds between spawns

    FILE* file = fopen("highscore.txt", "r");
    if(file){
        fscanf(file, "%d", &highScore);
        fclose(file);
    }

    while(!window.ShouldClose()){
        auto dt = window.GetFrameTime();
        
        gameSpeed += dt * 0.05f;  //increases the game speed by 0.05 every frame
        if(gameSpeed > 4.0f){
            gameSpeed = 4.0f;  //caps the game speed to 4.
        }

        auto transform = player->GetComponent<TransformComponent>();
        if(transform){
            const auto& position = transform->position;

            camera.position = raylib::Vector3(position.x, position.y + 10.0f, position.z + 30.0f); //sets the camera above and behind player
            camera.target = position; //follows the player
        }

        window.BeginDrawing();
        window.ClearBackground(raylib::Color::Black());
        BeginMode3D(camera);

        for (int i = -1; i <= 1; i++) {
            float x = i * 5.0f;
            float width = 4.0f;
            float height = 0.1f;  
            float length = 100.0f; 
            Vector3 pos = { x, -2.01f, -45.0f };
        
            DrawCube(pos, width, height, length, DARKGRAY);
        }

        auto playerTransform = player->GetComponent<TransformComponent>();
        if (playerTransform) {
            auto pos = playerTransform->position;
            DrawCube(pos, 4, 4, 4, GREEN); // width, height, depth
            DrawCubeWires(pos, 4, 4, 4, WHITE);
        }

        for(Entity* o: obstacles){
            auto transform = o->GetComponent<TransformComponent>();
            if(transform){
                auto pos = transform->position;
                DrawCube(pos, 4, 4, 4, BLUE);
                DrawCubeWires(pos, 4, 4, 4, WHITE);
            }
        }
            
        EndMode3D();

        if(!gameOver){
            player->Tick(dt);
            DrawText(TextFormat("Score: %.0f", score), 10, 10, 20, WHITE);
            spawnTimer += dt;

            for(Entity* o: obstacles){
                o->Tick(dt);
            }

            score += dt * 100 * gameSpeed; //increments score based on the game speed

            if (spawnTimer >= spawnInterval && obstacles.size() < MAX_OBSTACLES) {
                spawnTimer = 0.0f;

                Entity* obstacle = new Entity();
                int lane = GetRandomValue(0, 2); // same 3 lanes
                float positionX = (lane - 1) * 5.0f;
                float positionZ = GetRandomValue(-70, -30); // spawn farther back
        
                obstacle->AddComponent<TransformComponent>(raylib::Vector3{positionX, 0.0f, positionZ});
                obstacle->AddComponent<ObstacleComponent>();
        
                obstacles.push_back(obstacle);
            }
        
            // Clean up obstacles that moved off-screen
            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [](Entity* e) {
                auto t = e->GetComponent<TransformComponent>();
                if (t && t->position.z > 10.0f) {
                    delete e;
                    return true;
                }
                return false;
            }), obstacles.end());

            for(Entity* o : obstacles){
                if(checkCollision(player, o)){ //if player collides with obstacle, game over
                    gameOver = true;
                    break;
                }
            }
        }
        else{
            DrawText("GAME OVER", 245, 175, 50, RED);
            DrawText(TextFormat("Score: %.0f", score), 300, 225, 25, WHITE);
            DrawText(TextFormat("High Score: %.0f", highScore), 300, 245, 25, WHITE);
            DrawText("Press R to Restart", 300, 285, 25, WHITE);

            if (score > highScore) {
                highScore = score;
            
                FILE* file = fopen("highscore.txt", "w");
                if (file) {
                    fprintf(file, "%d", (int)highScore); // Save as integer
                    fclose(file);
                }
            }

            if(raylib::Keyboard::IsKeyPressed(KEY_R)){
                gameOver = false;
                gameSpeed = 1.0f;
                score = 0.0f;
                delete player;
                player = new Entity();

                auto trans = player->AddComponent<TransformComponent>(raylib::Vector3{0, 0, 0});
                auto phys = player->AddComponent<PhysicsComponent>();
                phys->position = trans->position;
                player->AddComponent<InputComponent>();

                for(auto& o: obstacles){
                    delete o;
                }
                obstacles.clear();

                int obstacleNum = GetRandomValue(2, 3);

                for (int i = 0; i < obstacleNum; i++){
                    Entity* obstacle = new Entity();
                    float positionX = (i - 1) * 5.0f;
                    float positionZ = GetRandomValue(-100, -30);
                
                    obstacle->AddComponent<TransformComponent>(raylib::Vector3{positionX, 0.0f, positionZ});
                    obstacle->AddComponent<ObstacleComponent>();
                    obstacles.push_back(obstacle);
                }
            }
        }
        window.EndDrawing();   
    }
}