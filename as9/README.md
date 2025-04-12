# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as9
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
Make sure you are in the build folder before proceeding.

# How to compile my code 
To compile my code, use the commands:
```bash
cmake ..
make
```
These commands will compile the code and makes it into an executable called as9.

# To run my code:
To run my code, use the command:
```bash
./as9
```

# Controls
```bash
Press R to restart when you die.
Press W to move forward
Press S to move backward
Press A to move left
Press D to move right
Press ENTER to play.
```

# Extra Credit
```bash
Added music
Added a start game button
Added a time that can track how long you have survived (UI)
```

# README Question
```bash
My game is a kind of bullet hell game where you have enemies shooting bullets/blocks at you and you have to dodge them. The game doesn't have a win condition. You are just meant to survive as long as possible. I added a homing sort of mechanic to the bullets where the bullets will move towards the player. Use the controls W, A, S, and D to move up, down, left, and right to dodge the bullets. 

The game starts off at a start screen where you have to press ENTER to start the game. If you die, you can press R to restart. 
```