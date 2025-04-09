# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as8
git submodule update --init --recursive
```
This is how you fetch my git submodules.

Make sure to delete the build folder before proceeding:
```bash
rm -rf build
```
Then in as8 folder, use the commands:
```bash
mkdir build
cd build
```
Make sure you are in the build folder before proceeding. 

# How to compile my code 
To compile my code, use the commands:
```bash
cmake ..
make
```
These commands will compile the code and makes it into an executable called as8.

# To run my code:
To run my code, use the command:
```bash
./as8
```

# Controls
```bash
Press W to increase the current selected entity in the direction it's facing.
Press S to decrease the current selected entity in the direction it's facing.
Press A to increase the current selected entity heading.
Press D to decrease the current selected entity heading.
Press Q to smoothly increase the entity's roll.
Press R to smoothly increase the entity's pitch.
Press F to smoothly decrease the entity's pitch
Press E to smoothly decrease the entity's roll. 
Press Space to reset the car's velocity to zero.
Press TAB to select between the different entities.
```

# Extra Credit
```bash 
Added third person where the camera follows the selected entity.

```
# Readme Question
Implemented `ECS.hpp` and commented each line per code in the assignment