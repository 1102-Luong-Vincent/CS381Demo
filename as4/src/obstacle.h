#ifndef OBSTACLE_H
#define OBSTACLE_H
#define MAX_OBSTACLES 2
#include <raylib-cpp.hpp>
#include "player.h"

class Obstacle{
    public:

    Vector2 position;
    Vector2 size;
    float moveSpeed;
    float gameSpeed;

    Obstacle();
    void UpdateObstacle(float dt, int index);
    void DrawObstacle();
    bool checkCollision(Player dinosaur);
};

extern Obstacle obstacles[MAX_OBSTACLES]; //defines a fixed array in replacement of vector
#endif