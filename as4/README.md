# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as4
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
These commands will compile the code and makes it into an executable called as4.

# To run my code:
To run my code, use the command:
```bash
./as4
```

# Controls
```bash
Press SPACE to jump.
Press R to restart when you die.
```

# README Question
My game is a platformer where the player is a cube or a block, and there are obstacles that the player must avoid by jumping over them. The obstacles are generated at random and the game gets faster as time progresses. There is also a scoreboard where you can aim to beat your own high score. The game is endless, meaning there is no finish line. The goal of the game is to survive as long as possible.  

To play my game, you just need to press SPACE on the keyboard to jump in order to avoid the obstacles. If you were to die, by colliding with an obstacle, you can press R on the keyboard to restart the game. 