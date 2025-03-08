# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as5
git submodule update --init --recursive
```
This is you fetch my git submodules.

Make sure to delete the build folder before proceeding:
```bash
rm -rf build
```
Then in as5 folder, use the commands:
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
These commands will compile the code and makes it into an executable called as5.

# To run my code:
To run my code, use the command:
```bash
./as5
```

# Controls
```bash
Press W to increase the current selected entity in the direction it's facing.
Press S to decrease the current selected entity in the direction it's facing.
Press A to increase the current selected entity heading.
Press D to decrease the current selected entity heading.
Press Space to reset the car's velocity to zero.
Press TAB to select between the different entities.
```

# Readme Question
My selection management system enables the user to interactively choose different entities within the 3D environment using keyboard by pressing TAB. In addition, each entity has a bounding box with `selectedEntity` as the pointer that keeps track of the currently selected entity. Pressing TAB will allow the user to update `selectedEntity` and move to the next entity. By using this selection management system, this allows for smooth transitions between entities.

Monolithic Entities have fixed structures, where every entity contains a predefined set of attributes and components, rendering it not flexible and consuming more data that is unnecessary. However, because of its fixed structure, it has a rather simple structure that is easy to implement and maintain.

Ad Hoc allows entities to be dynamically assembled from independent components, rending it more complex with an increase in memory usage due to component management overhead. However, because it can be assembled with individual components, Ad Hoc is highly flexible and can be easily expanded with more components.

I prefer Ad Hoc because it reduces data usage, as each entity only contains the components it needs to use, it is more flexible and is more scalable making it easier to extend functionality. 