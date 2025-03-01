#include "player.h"

Player::Player(){
    ground = 350;
    position = {100, ground - 50}; //starting position
    size = {40, 50};  //size of the model
    gravity = 1000.0f;  
    jump = -450.0f; 
    verticalHeight = 0.0f;
    isJumping = false;
}

void Player::UpdateControls(float dt, Sound jumpSound){
    if(IsKeyPressed(KEY_SPACE) && !isJumping){
        verticalHeight = jump;
        isJumping = true;
        StopSound(jumpSound);
        PlaySound(jumpSound);
    }

    verticalHeight += gravity * dt;
    position.y += verticalHeight * dt;

    if(position.y + size.y >= ground){ //stops model from falling below ground
        position.y = ground - size.y;
        isJumping = false;
        verticalHeight = 0;
    }
}

void Player::DrawModel(){
    DrawRectangle(position.x, position.y, size.x, size.y, BROWN);
}