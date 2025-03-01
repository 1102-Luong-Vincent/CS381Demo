#include "obstacle.h"

Obstacle obstacles[MAX_OBSTACLES];

Obstacle::Obstacle(){
    position = {800 + (rand() % 150), 350 - 40}; //starts off camera and randomizes location 
    size = {20, 40}; //size of the model
    moveSpeed = 200.0f; //speed of how fast the obstacle moves towards player
    gameSpeed = 1.0f;
}

void Obstacle::UpdateObstacle(float dt, int index){ //move obstacle to left as camera moves
    gameSpeed += dt * 0.1f; //changes how fast the speed increments

    if(gameSpeed > 4.0f){
        gameSpeed = 4.0f;
    }

    position.x -= moveSpeed * gameSpeed * dt;

    if(position.x < -size.x){
        position.x = 800 + (rand() % 150); //randomizes the respawn position of the obstacles.

        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (i != index && abs(position.x - obstacles[i].position.x) < 100){
                position.x += 50;  // Push forward slightly
            }
        }
    }
}

void Obstacle::DrawObstacle(){
    DrawRectangle(position.x, position.y, size.x, size.y, GREEN);
}

bool Obstacle::checkCollision(Player dinosaur){
    return (dinosaur.position.x < this->position.x + this->size.x &&
        dinosaur.position.x + dinosaur.size.x > this->position.x &&
        dinosaur.position.y < this->position.y + this->size.y &&
        dinosaur.position.y + dinosaur.size.y > this->position.y);
}