# Question 1: 
# How to make window resizable
To make a window resizable, we use the Raylib C++ library function, `SetState()`, with the flag, `FLAG_FLAG_WINDOW_RESIZABLE`. We use this function and the flag because this allows the person to adjust/resize the window dynamically. 

# How to keep text centered in a resizable window
To keep text centered in a resizing window, you use the `MeasureText()` function, which allows you to determine the total width of the text. Then, use the `GetHeight()` and `GetWidth()` library functions, subtract the size of the text from the total width and divide by 2. This is because `GetHeight()` and `GetWidth()` functions references the entire dimensions of the window and dividing the width and height with the text in half will keep the text centered in a resizing window. 

# Question 2: How to fetch my git submodules:
First use the command: `git clone https://github.com/1102-Luong-Vincent/CS381Demo`
    - This clones into my repository for GitHub.

Then use the commands: `git submodule init` and `git submodule update --init --recursive`
    - This clones all of the Raylib's dependencies to our folder and that allows us to use Raylib libraries and functions for our code. 

This is how to fetch my git submodules. 

# Question 3: How to compile my code: 
# make sure you in the src file. that's where my .cpp file is located.
first, use the command: `cmake ..`
    - This command generates necessary build files (such as makefile) which is required to compile the code. 

second, use the command: `make`
    - This command compiles your code and makes it into an executable called `as0`.

# Question 4: How to run my code:
After compiling the program, use this command to run my code: `./as0`
    - Using this command will execute or run my code on the terminal. Once the code runs, the result will pop up. 

# How to enumerate its controls:
    - There are no controls. Controls are not applicable.
