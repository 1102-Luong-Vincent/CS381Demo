# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as10
git submodule update --init --recursive
```
This is how you fetch my git submodules.

# How to compile my code 
No compilation is needed — this is a Godot project using GDScript.

To open it:
```bash
Open Godot 4.4
Click "Import Project"
Select the project.godot file inside the as10/ folder
Click Play (or press F5) to run
```

# To run my code:
To run my code, go into Godot and:
```bash
Click Play and click Host
Open Godot again, click Play and click Join (make sure to type in the IP Address that you connecting to before clicking join.)
```
Note: I tested this with two instances of Godot running on the same machine, so I could host and join with the IP address from my machine.

# Controls
```bash
+ button - use mouse click to increment the value when pressed
- button - use mouse click to decrement the value when pressed
Make sure to enter the IP address of the machine before clicking join.
```

# Readme Question
The value is updated using an `rpc()` function. When the user clicks the + or – button, it triggers `update_counter(value)` which then is called on all clients using `rpc("update_counter", value)`. After this, the update_counter than assigns the value and updates the UI label in the scene. From this, the host has authority and makes sure that the changes are synced, so that when others join the network, everyone sees the same synced value as the host.
