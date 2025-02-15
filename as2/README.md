# How to fetch git submodules
To fetch my git submodules, use the commands:
```bash
git clone https://github.com/1102-Luong-Vincent/CS381Demo.git
cd CS381Demo
cd as2
git submodule update --init --recursive
```
This is you fetch my git submodules.

Make sure to delete the build folder before proceeding:
```bash
rm -rf build
```
Then in as2 folder, use the commands:
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
These commands will compile the code and makes it into an executable called as2.

# To run my code:
To run my code, use the command:
```bash
./as2
```
# Controls
Controls are not applicable for this assignment. 

# Readme Question
The point of the `DrawBoundedModel` function is to allow for the model's transformation to be modified dynamically before it is drawn. This is important because this allows the models to be scaled, changed, and positioned without causing any changes to their actual model before being drawn. Without the `DrawBoundedModel` function, any changes we make to the model's transformation would also affect the actual figure or model type of what we are working with. 

The `DrawBoundedModel` function can be used in the future to set transformations when a model is loaded because this function can allow default transformations to be stored for each model type. This function can also make it easier to animate complex models with the use of multiple parts. 

The `DrawBoundedModel` function can also be used to transform a model relative to some "parent". This function could be modified to support a parent-child relationship. For example, let's say for the "parent" model, the model is a car, the child model are the wheels, and the car (parent model) starts to rotate. To transform a model (child model) relative to this parent, we would want the child model to also rotate and follow or mimic the actions of the parent. This will ensure that the model will always be positioned relative to the parent because both models would be performing the same actions (rotating) at the same time. Therefore, the `DrawBoundedModel` function can be used to transform a model relative to some "parent".

# Extra Credit
Added spinning/rotation animations for both the rocket and car models. The rotation spins in a horizontal clockwise/counter-clockwise motion. 