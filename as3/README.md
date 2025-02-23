# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as3
git submodule update --init --recursive
```
This is you fetch my git submodules.

Make sure to delete the build folder before proceeding:
```bash
rm -rf build
```
Then in as3 folder, use the commands:
```bash
mkdir build
cd build
```
Make sure you are in the build folder and make sure that you go into the Raylib's config.h and enable jpg support before proceeding. 

# How to enable JPG Support
```bash
Go to as0 folder.
Open raylib-cpp folder.
Open raylib folder inside raylib-cpp folder
Open src folder from the raylib folder
go to config.h file.
go to line 162 or `#define SUPPORT_FILEFORMAT_JPG` and un-comment the line to enable jpg support
```

# How to compile my code 
To compile my code, use the commands:
```bash
cmake ..
make
```
These commands will compile the code and makes it into an executable called as3.

# To run my code:
To run my code, use the command:
```bash
./as3
```
Note: You might want to run the code a few times. I noticed that not everything loads up correctly the first time. Re-running the code a few times will load up everything. 

# Controls
```bash
Press and hold down W to increase the car's velocity in the direction it's facing.
Press S to decrease the car's velocity in the direction it's facing.
Press and hold down A to increase the car's heading.
Press and hold down D to decrease the car's heading.
Press Space to reset the car's velocity to zero
Press LEFT ARROW or RIGHT ARROW to move the camera left or right. 
```

# Extra Credit and Controls
Added the ability for the car to fly. Controls for flying:
```bash
Press Q to veritcally rise the car up
Press E to vertically lower the car down
```

# Readme Question
DT stands for Delta Time. Delta Time is important because it measures the time elapsed between the current frame and the previous frame. It also makes object animations move at a smooth and consistent motion, instead of moving inconsistently. To calculate Delta Time, we take into account the movement of the object like its speed or velocity and multiply it with Delta Time. An example of this would be `position = position + velocity *  DeltaTime()`. This calculation ensures that the object's movement and animations move smoothly. 