# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as7
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
These commands will compile the code and makes it into an executable called as7.

# To run my code:
To run my code, use the command:
```bash
./as7
```

# Controls
```bash
Press SPACE to jump.
Press R to restart when you die.
Press A to switch to left lane
Press D to switch to right lane
```

# README Question
My game is an endless runner where the player is a cube or a block, and there are obstacles that the player must avoid by either jumping over them or moving out of the way. There are three different paths and by either pressing A or D, the player can move to either path to avoid obstacles. By default, the player starts in the middle path. The obstacles are generated at random and the game gets faster as time progresses. There is also a scoreboard where you can aim to beat your own high score. The game is endless, meaning there is no finish line. The goal of the game is to survive as long as possible. 

To play my game, you can press SPACE on the keyboard to jump in order to avoid the obstacles, or you can press A to move to the left path, or press D to move to the right path to avoid the obstacles. If you were to die, by colliding with an obstacle, you can press R on the keyboard to restart the game. In addition, after dying, you can see the current score you obtained and can try to beat it.