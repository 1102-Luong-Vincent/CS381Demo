# Fetching git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd as1
mkdir build && cd build
ln -s ../as0/raylib-cpp/ ./raylib-cpp
```
This is how you fetch my git submodules.

# How to compile my code:
Make sure you are in the `build` folder for as1 before proceeding
To compile my code, use the commands:
```bash
cmake ..
make
```
These commands will compile the code and makes it into an executable called as1.

# To run my code:
To run my code, use the command:
```bash
./as1
```
This will run my code and the Volume Control window will pop up.

# Extra Credit - Controls:
When the program is running, and the Volume Control window is open, on keyboard:
```bash
Press E to toggle Dark Mode. Pressing E again will disable Dark Mode. 
Press TAB to cycle between the three sliders. By default, it starts at the SFXVolume slider.
Use LEFT and RIGHT arrow keys to adjust volume for the selected slider.
```

# Assignment Question
Audio is produced by a speaker by taking and converting electrical signals into sound waves. A magnetic field is then generated where an electrical current travels through coils of wire and causes a cone to vibrate, which in turn pushes and pulls air molecules, creating sound waves that we can then hear. 

`raylib::AudioDevice` is necessary in my code because it initializes and manages the audio system in RayLib.` raylib::AudioDevice` is necessary because without this function, the audio system in RayLib will not be present and as a result, we wouldn't be able to hear anything when playing actual audio.