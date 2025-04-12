#include <iostream>
#include "raylib-cpp.hpp"
#include "ECS.hpp"
#include <vector>
#include "raylib.h"

float gameSpeed = 1.0f;

using cs381::Entity;
size_t globalComponentCounter = 0;
std::vector<Entity> bulletEntity;

enum GameState {START_SCREEN, GAME_RUNNING, GAME_OVER};
GameState currentState = START_SCREEN;

struct Scene{ //holds all the components and systems.


};
auto x = sizeof(Vector3); //size of the Vector3 struct.

struct BulletComponent{
    float bulletLifeTime = 15.0f;
};

struct EnemyComponent{
    float coolDown = 1.0f;
    float lastShot = 0.0f;
};

struct VelocityComponent{
    raylib::Vector3 movement = {0, 0, 0};
};

struct TransformComponent{
    raylib::Vector3 position;
    raylib::Vector3 scale = {1, 1, 1};
    raylib::Quaternion rotation = raylib::Quaternion::Identity();
};

bool CheckCollision(const raylib::Vector3& a, const raylib::Vector3& b, float sizeA = 2.0f, float sizeB = 1.0f) {
    return fabs(a.x - b.x) < (sizeA + sizeB) * 0.5f &&
           fabs(a.y - b.y) < (sizeA + sizeB) * 0.5f &&
           fabs(a.z - b.z) < (sizeA + sizeB) * 0.5f;
}

void PlayerSystem(cs381::Scene<>& scene, Entity playerEntity, float dt){

    auto& transform = scene.GetComponent<TransformComponent>(playerEntity);
    auto& velocity = scene.GetComponent<VelocityComponent>(playerEntity);  

    transform.position += velocity.movement * dt;   
    
}
void EnemySystem(cs381::Scene<>& scene, std::vector<Entity> enemies, Entity playerEntity, float dt){

    auto& playerTransform = scene.GetComponent<TransformComponent>(playerEntity); // assuming playerEntity == 0
    
    for(auto enemyEntity : enemies){
        if(!scene.HasComponent<EnemyComponent>(enemyEntity) || !scene.HasComponent<TransformComponent>(enemyEntity)) return;

            auto& transform = scene.GetComponent<TransformComponent>(enemyEntity);
            auto& enemy = scene.GetComponent<EnemyComponent>(enemyEntity);

            enemy.lastShot += dt;

            float coolDownAfterIncrease = std::max(0.2f, enemy.coolDown / gameSpeed); //cooldown caps at 0.2 sec after gameSpeed is maxed.
            float speed = GetRandomValue(5, 20);

            if(enemy.lastShot > coolDownAfterIncrease){
                enemy.lastShot = 0.0f;

                int minBullets = 1;
                int maxBullets = std::min(10, static_cast<int>(gameSpeed * 2));
                int numberOfBullets = GetRandomValue(minBullets, maxBullets);

            for(int i = 0; i < numberOfBullets; i++){
                Entity bullet = scene.CreateEntity();
                scene.AddComponent<TransformComponent>(bullet);
                scene.AddComponent<VelocityComponent>(bullet);
                scene.AddComponent<BulletComponent>(bullet);
                bulletEntity.push_back(bullet);

                auto& bulletTransform = scene.GetComponent<TransformComponent>(bullet);
                bulletTransform.position = transform.position;
        
                raylib::Vector3 direction = (playerTransform.position - transform.position).Normalize();
       
                direction.x += GetRandomValue(-20, 20) * 0.01f; //bullets are spread randomly in the x direction
                direction.y += GetRandomValue(-20, 20) * 0.01f; //bullets are spread randomly in the y direction
                direction = direction.Normalize();

                bulletTransform.position += direction * 2.0f;
                scene.GetComponent<VelocityComponent>(bullet).movement = direction * speed;
            }
        }
    }    
}        

void BulletSystem(cs381::Scene<>& scene, Entity playerEntity, float dt){
    std::vector<Entity> stillAlive;
    auto& playerPos = scene.GetComponent<TransformComponent>(playerEntity).position;
    float homingSpeed = 0.02f;

    for (Entity e: bulletEntity){
        if(!scene.HasComponent<TransformComponent>(e) || !scene.HasComponent<VelocityComponent>(e) || !scene.HasComponent<BulletComponent>(e)) continue;

            auto& bullet = scene.GetComponent<BulletComponent>(e);
            auto& velocity = scene.GetComponent<VelocityComponent>(e);
            auto& transform = scene.GetComponent<TransformComponent>(e);
        
            bullet.bulletLifeTime -= dt;

            if(bullet.bulletLifeTime > 0.0f){
                if (transform.position.z > playerPos.z + 2.0f) { //should despawn the bullet once it passes the player
                    continue;
                }
            
                raylib::Vector3 toPlayer = (playerPos - transform.position).Normalize(); // vector pointing from bullet to player
                toPlayer.x += GetRandomValue(-10, 10) * 0.01f;
                toPlayer.z += GetRandomValue(-10, 10) * 0.01f;
                toPlayer = toPlayer.Normalize();
                velocity.movement = (velocity.movement * (1.0f - homingSpeed) + toPlayer * homingSpeed * 60.0f).Normalize() * 60.0f; //speed the bullet goes and homes towards the player
                transform.position += velocity.movement * dt;
            }

            if(transform.position.z > playerPos.z + 20 || fabs(transform.position.x - playerPos.x) > 100 || fabs(transform.position.y - playerPos.y) > 100){
                continue;
            }
            stillAlive.push_back(e);
    }            
        bulletEntity = stillAlive;
}

void ControlInputSystem(cs381::Scene<>& scene, Entity playerEntity, float dt){
   
        auto& velocity = scene.GetComponent<VelocityComponent>(playerEntity);
        velocity.movement = raylib::Vector3{0.0f, 0.0f, 0.0f};
        float speed = 20.0f;

        if(IsKeyDown(KEY_W)){
            velocity.movement.z -= speed;
        }
        if(IsKeyDown(KEY_S)){
            velocity.movement.z += speed;
        }
        if(IsKeyDown(KEY_A)){
            velocity.movement.x -= speed;
        }
        if(IsKeyDown(KEY_D)){
            velocity.movement.x += speed;
        }    
}

int main(){
    raylib::Window window(800, 450, "CS381 - Assignment 9");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    raylib::Camera3D camera;
    
    camera.position = {0, 60, 0};
    camera.target = {0, 0, 0};
    camera.up = {0, 0, -1};
    camera.fovy = 70;
    camera.projection = CAMERA_PERSPECTIVE;

    InitAudioDevice();
    Music sound = LoadMusicStream("../music/endless_blue.mp3");
    PlayMusicStream(sound);

    cs381::Scene<cs381::ComponentStorage> scene; //create a new scene.
    cs381::Entity playerEntity= scene.CreateEntity();
    cs381::Entity enemyEntity = scene.CreateEntity(); //first enemy
    cs381::Entity enemyEntity2 = scene.CreateEntity(); //second enemy
    cs381::Entity enemyEntity3 = scene.CreateEntity(); //third enemy

    scene.AddComponent<TransformComponent>(playerEntity);
    scene.AddComponent<VelocityComponent>(playerEntity);

    scene.AddComponent<TransformComponent>(enemyEntity);
    scene.AddComponent<EnemyComponent>(enemyEntity);
    scene.GetComponent<TransformComponent>(enemyEntity).position = raylib::Vector3(0, 0, -40);

    scene.AddComponent<TransformComponent>(enemyEntity2);
    scene.AddComponent<EnemyComponent>(enemyEntity2);
    scene.GetComponent<TransformComponent>(enemyEntity2).position = raylib::Vector3(-15, 0, -40); //left of the enemy

    scene.AddComponent<TransformComponent>(enemyEntity3);
    scene.AddComponent<EnemyComponent>(enemyEntity3);
    scene.GetComponent<TransformComponent>(enemyEntity3).position = raylib::Vector3(15, 0, -40); //right of the enemy
    
    float time = 0.0f, timeSurvived = 0.0f;
    bool gameOver = false;
    std::vector<Entity> enemies = {enemyEntity, enemyEntity2, enemyEntity3};

    FILE* file = fopen("highscore.txt", "r");
    if(file){
        fscanf(file, "%d", &timeSurvived);
        fclose(file);
    }

    while(!window.ShouldClose()){
        float dt = window.GetFrameTime();
        UpdateMusicStream(sound);

        gameSpeed += dt * 0.05f;
        if(gameSpeed > 5.0f){
            gameSpeed = 5.0f;
        }

        if(currentState == START_SCREEN && IsKeyPressed(KEY_ENTER)){
            currentState = GAME_RUNNING;
            time = 0.0f;
            gameSpeed = 1.0f;
        }

        if(currentState == GAME_RUNNING){
            time += dt;
            ControlInputSystem(scene, playerEntity, dt);
            PlayerSystem(scene, playerEntity, dt);
            EnemySystem(scene, enemies, playerEntity, dt);
            BulletSystem(scene, playerEntity, dt);
            
            auto& playerPos = scene.GetComponent<TransformComponent>(playerEntity).position;
            for (Entity e : bulletEntity) {
                if (scene.HasComponent<TransformComponent>(e)) {
                    auto& bulletPos = scene.GetComponent<TransformComponent>(e).position;
                    if (CheckCollision(playerPos, bulletPos)) {
                        gameOver = true;
                        currentState = GAME_OVER;
                    }
                }
            }
        }

        camera.target = scene.GetComponent<TransformComponent>(playerEntity).position;

        window.BeginDrawing();
        window.ClearBackground(raylib::Color::Black());
        BeginMode3D(camera);
        
        auto& transform = scene.GetComponent<TransformComponent>(playerEntity);
        auto& enemyTransform = scene.GetComponent<TransformComponent>(enemyEntity);
        auto& enemyTransform2 = scene.GetComponent<TransformComponent>(enemyEntity2);
        auto& enemyTransform3 = scene.GetComponent<TransformComponent>(enemyEntity3);

        DrawCube(transform.position, 4, 4, 4, GREEN); //draw player
        DrawCube(enemyTransform.position, 4, 4, 4, RED); //draw enemy1
        DrawCube(enemyTransform2.position, 4, 4, 4, WHITE); //draw enemy2
        DrawCube(enemyTransform3.position, 4, 4, 4, PURPLE); //draw enemy3

        for (Entity e : bulletEntity) {
            if (scene.HasComponent<TransformComponent>(e)) {
                auto& transform = scene.GetComponent<TransformComponent>(e);
                DrawCube(transform.position, 4, 4, 4, BLUE);
            }
        }
        
        EndMode3D();

        if(currentState == START_SCREEN){
            DrawText("Bullet Game", 110, 150, 100, WHITE);
            DrawText("Press ENTER to Start", 210, 250, 30, WHITE);
        }
        else if(currentState == GAME_RUNNING){
            DrawText(TextFormat("Time Survived: %.0fs", time), 20, 20, 25, WHITE);
        }    
           
        else if(currentState == GAME_OVER){
            DrawText("GAME OVER!", 245, 175, 50, RED);
            DrawText(TextFormat("Time Survived: %.0fs", time), 300, 225, 25, WHITE);
            DrawText("Press R to Restart", 300, 255, 25, WHITE);

            if (time > timeSurvived) {
                timeSurvived = time;
            
                FILE* file = fopen("highscore.txt", "w");
                if (file) {
                    fprintf(file, "%d", (int)timeSurvived); // Save as integer
                    fclose(file);
                }
            }

            if(IsKeyPressed(KEY_R)){
                gameOver = false;
                gameSpeed = 1.0f;
                time = 0.0f;
                bulletEntity.clear();
                playerEntity = scene.CreateEntity();
                enemyEntity = scene.CreateEntity();

                //calls each component so they can drawn, basically reset when game starts over.
                scene.AddComponent<TransformComponent>(playerEntity); 
                scene.AddComponent<VelocityComponent>(playerEntity);
                scene.GetComponent<TransformComponent>(playerEntity).position = raylib::Vector3(0, 0, 0);

                scene.AddComponent<TransformComponent>(enemyEntity);
                scene.AddComponent<EnemyComponent>(enemyEntity);
                scene.GetComponent<TransformComponent>(enemyEntity).position = raylib::Vector3(0, 0, -40);  

                scene.AddComponent<TransformComponent>(enemyEntity2);
                scene.AddComponent<EnemyComponent>(enemyEntity2);
                scene.GetComponent<TransformComponent>(enemyEntity2).position = raylib::Vector3(-15, 0, -40); //left of the enemy

                scene.AddComponent<TransformComponent>(enemyEntity3);
                scene.AddComponent<EnemyComponent>(enemyEntity3);
                scene.GetComponent<TransformComponent>(enemyEntity3).position = raylib::Vector3(15, 0, -40); //right of the enemy
                
                currentState = START_SCREEN;
            }
        }
      
        window.EndDrawing();
    }    
    UnloadMusicStream(sound);
    CloseAudioDevice(); 
}  