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
Press SPACE to jump.
Press R to restart when you die.
Press A to switch to left lane
Press D to switch to right lane
```

# README Question
