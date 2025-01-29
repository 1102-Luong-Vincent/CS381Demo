#include <iostream>
#include "raylib-cpp.hpp"

int main(){
    raylib::Window window(800, 450, "CS381 - Assignment 0"); //dimensions of window and title of the window
    raylib::Color textColor = raylib::Color::Black(); //default color of the text displayed on the window

    window.SetState(FLAG_WINDOW_RESIZABLE);

    float leftMovement = 0;
    float movementSpeed = 1.0f;
    float rightMovement = true;

    while(!window.ShouldClose()){

        if(rightMovement){
            leftMovement += movementSpeed;

            if(leftMovement > 100){
                rightMovement = false;
            }
        }    
            else{
                leftMovement -= movementSpeed; //change direction
                
                if(leftMovement < -100){
                    rightMovement = true; //change direction
                }
            }

        window.BeginDrawing();
        ClearBackground(LIGHTGRAY); //gives the background window color. 

        int textSize = MeasureText("Congrats! You created your first window!", 20);

        int widthSize = ((window.GetWidth() - textSize) / 2) + leftMovement;
        int heightSize = (window.GetHeight() - 20) / 2;

        //added color to each word of the text box.
        DrawText("Congrats!", widthSize, heightSize, 20, YELLOW);
        DrawText("you", widthSize + MeasureText("Congrats! ", 20), heightSize, 20, RED);
        DrawText("created", widthSize + MeasureText("Congrats! you ", 20), heightSize, 20, BLUE);
        DrawText("your", widthSize + MeasureText("Congrats! you created ", 20), heightSize, 20, GREEN);
        DrawText("first", widthSize + MeasureText("Congrats! you created your ", 20), heightSize, 20, PURPLE);
        DrawText("window", widthSize + MeasureText("Congrats! you created your first ", 20), heightSize, 20, ORANGE);

        window.EndDrawing();
    }
}