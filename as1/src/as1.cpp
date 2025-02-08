#include <iostream>
#include <raylib-cpp.hpp>
#include "raygui.h"

raylib::Music ping;
raylib::Music crowd;
raylib::Music dialogue;

bool isMusicPlaying = false;
bool isDarkMode = false;
int selectedSlider = 0;

void PingButton(){
    if(!ping.IsPlaying()){
        ping.Play();
    }
    if(!dialogue.IsPlaying()){
        dialogue.Play();
    }
    if(!crowd.IsPlaying()){
        crowd.Play();
    }
}

#include "VolumeControl.h"

int main(){
    raylib::Window window(300, 350, "CS 381 - Assignment 1");
    window.SetState(FLAG_WINDOW_RESIZABLE);
    raylib::AudioDevice audio;

    raylib::Text text;
    text.SetText("Volume Controls");
    
    ping = raylib::Music("../assets/audio/ping.wav");
    dialogue = raylib::Music("../assets/audio/skyrim-song-ragnar-the-red-made-with-Voicemod.mp3"); //loop indefinitely for music track
    crowd = raylib::Music("../assets/audio/price-of-freedom.mp3"); //loop indefinitely for dialogue track

    crowd.SetLooping(true);
    dialogue.SetLooping(true);

    auto guiState = InitGuiVolumeControl();
    Rectangle buttonBounds = {24, 304, 256, 24}; //button dimensions
    
    const char darkMode[22] = "Press E for Dark Mode";

    while(!window.ShouldClose()){

        if(IsKeyPressed(KEY_E)){     //press E to toggle Dark Mode for Window
            isDarkMode = !isDarkMode;
        }

        //use TAB to select between the three sliders
        if(IsKeyPressed(KEY_TAB)){ 
            selectedSlider = (selectedSlider + 1) % 3;
        }
        //use LEFT or RIGHT arrow to adjust volume for the selected slider
        if(IsKeyPressed(KEY_LEFT)){
            if(selectedSlider == 0){
                guiState.SFXSliderValue = std::max(0.0f, guiState.SFXSliderValue - 1.0f);
            }
            if(selectedSlider == 1){
                guiState.MusicSliderValue = std::max(0.0f, guiState.MusicSliderValue - 1.0f);
            }
            if(selectedSlider == 2){
                guiState.DialogueSliderValue = std::max(0.0f, guiState.DialogueSliderValue - 1.0f);
            }
        }
        if(IsKeyPressed(KEY_RIGHT)){ 
            if(selectedSlider == 0){
                guiState.SFXSliderValue = std::min(100.0f, guiState.SFXSliderValue + 1.0f);
            }
            if(selectedSlider == 1){
                guiState.MusicSliderValue = std::min(100.0f, guiState.MusicSliderValue + 1.0f);
            }
            if(selectedSlider == 2){
                guiState.DialogueSliderValue = std::min(100.0f, guiState.DialogueSliderValue + 1.0f);
            }
        }
        window.ClearBackground(isDarkMode ? BLACK : WHITE); //resets the background back to default white once E is toggled again.

        //DrawText("Press E for Dark Mode", 10, 10, 14, isDarkMode ? WHITE : BLACK);
        // DrawText("Press TAB to change sliders", 10, 30, 14, isDarkMode ? WHITE : BLACK);
        // DrawText("Press Left/Right to change volume", 10, 50, 14, isDarkMode ? WHITE : BLACK);

        // int sliderYPosition[] = {130, 190, 250};
        // DrawRectangleLines(24, sliderYPosition[selectedSlider], 256, 24, GRAY);
        
        GuiVolumeControl(&guiState);
        
        if (GuiButton(buttonBounds, "Ping")) {
            ping.SetVolume(guiState.SFXSliderValue / 100.0f);
            crowd.SetVolume(guiState.MusicSliderValue / 100.0f);
            dialogue.SetVolume(guiState.DialogueSliderValue / 100.0f);
            PingButton();  // Calls function that plays the sound
        }

        ping.Update();
        ping.SetLooping(false);

        crowd.Update();

        dialogue.Update();

        window.EndDrawing();
    }
}