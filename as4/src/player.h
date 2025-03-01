#ifndef PLAYER_H
#define PLAYER_H
#include <raylib-cpp.hpp>
#include <iostream>

class Player{
    public:
    
    Vector2 position;
    Vector2 size;
    bool isJumping;
    int ground;
    float gravity, jump, verticalHeight;

    Player();
    void UpdateControls(float dt, Sound jumpSound);
    void DrawModel();
};
#endif