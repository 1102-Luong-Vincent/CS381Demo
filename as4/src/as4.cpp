#include "player.h"
#include "obstacle.h"
#include <raylib-cpp.hpp>
#include <iostream>

int main(){
    raylib::Window window(800, 450, "Platform Game");
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(2048);

    int score = 0, highScore = 0;
    bool gameOver = false;
    Player player;
    Obstacle obstacle;

    FILE* file = fopen("highscore.txt", "r");
    if(file){
        fscanf(file, "%d", &highScore);
        fclose(file);
    }

    Sound jumpSound = LoadSound("../assets/audio/jump_08.wav");
    
    while(!window.ShouldClose()){
        float dt = GetFrameTime();
        int index;

        if(!gameOver){
            player.UpdateControls(dt, jumpSound);
            for(int i = 0; i < MAX_OBSTACLES; i++){
                obstacles[i].UpdateObstacle(dt, index);

                if(obstacles[i].checkCollision(player)){
                    gameOver = true;
                }
            }    
            score += 1;
        }

        window.BeginDrawing();
        window.ClearBackground(raylib::Color::Black());

        if(!gameOver){
            DrawLine(0, 350, 800, 350, WHITE); //draws line from (0, 350) to (800, 350).
            player.DrawModel();

            for(int i = 0; i < MAX_OBSTACLES; i++){
                obstacles[i].DrawObstacle();
            }
            DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
        }
        else{
            if(score > highScore){
                highScore = score;
                file = fopen("highscore.txt", "w");
                if(file){
                    fprintf(file, "%d", highScore);
                    fclose(file);
                }
            }

            DrawText("Game Over!", 300, 175, 30, RED);
            DrawText(TextFormat("Score: %i", score), 300, 225, 25, WHITE);
            DrawText(TextFormat("High Score: %i", highScore), 300, 245, 25, WHITE);
            DrawText("Press R to Restart", 300, 295, 30, GRAY);

            if(IsKeyPressed(KEY_R)){
                player.position.y = 300;
                obstacle.position.x = 600;
                score = 0;
                gameOver = false;

                for(int i = 0; i < MAX_OBSTACLES; i++){ //resets the randomization of the obstacles and the speed to normal
                    obstacles[i].position.x = 800 + (rand() % 200);
                    obstacles[i].gameSpeed = 1.0f;
                }
            }
        }   
        window.EndDrawing();
    }
    UnloadSound(jumpSound);
    CloseWindow();
}        