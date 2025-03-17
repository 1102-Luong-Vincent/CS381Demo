# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as6
git submodule update --init --recursive
```
This is you fetch my git submodules.

Make sure to delete the build folder before proceeding:
```bash
rm -rf build
```
Then in as6 folder, use the commands:
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
These commands will compile the code and makes it into an executable called as6.

# To run my code:
To run my code, use the command:
```bash
./as6
```

# Controls
```bash
Press and hold W to increase the velocity of the current selected entity in the direction it's facing.
Press and hold S to decrease the velocity of the current selected entity in the direction it's facing.
Press and hold A to increase the velocity of the current selected entity heading.
Press and hold D to decrease the velocity of the current selected entity heading.
Press Space to reset the car's velocity to zero.
Press TAB to select between the different entities.
```

# Extra Credit
```bash
Added Mouse Selection. Using LEFT MOUSE BUTTON and clicking on any of the models will select and choose that entity for you to control. 

Added collision where the rocket will not go through the ground.
```

# README Question
```bash
Implemented CO.hpp to my assignment. One comment has been updated per line of code.
```