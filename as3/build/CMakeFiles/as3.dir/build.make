# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/CS381/as3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/CS381/as3/build

# Include any dependencies generated for this target.
include CMakeFiles/as3.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/as3.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/as3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/as3.dir/flags.make

CMakeFiles/as3.dir/src/as3.cpp.o: CMakeFiles/as3.dir/flags.make
CMakeFiles/as3.dir/src/as3.cpp.o: ../src/as3.cpp
CMakeFiles/as3.dir/src/as3.cpp.o: CMakeFiles/as3.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/CS381/as3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/as3.dir/src/as3.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/as3.dir/src/as3.cpp.o -MF CMakeFiles/as3.dir/src/as3.cpp.o.d -o CMakeFiles/as3.dir/src/as3.cpp.o -c /mnt/c/CS381/as3/src/as3.cpp

CMakeFiles/as3.dir/src/as3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/as3.dir/src/as3.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/CS381/as3/src/as3.cpp > CMakeFiles/as3.dir/src/as3.cpp.i

CMakeFiles/as3.dir/src/as3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/as3.dir/src/as3.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/CS381/as3/src/as3.cpp -o CMakeFiles/as3.dir/src/as3.cpp.s

CMakeFiles/as3.dir/src/skybox.cpp.o: CMakeFiles/as3.dir/flags.make
CMakeFiles/as3.dir/src/skybox.cpp.o: ../src/skybox.cpp
CMakeFiles/as3.dir/src/skybox.cpp.o: CMakeFiles/as3.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/CS381/as3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/as3.dir/src/skybox.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/as3.dir/src/skybox.cpp.o -MF CMakeFiles/as3.dir/src/skybox.cpp.o.d -o CMakeFiles/as3.dir/src/skybox.cpp.o -c /mnt/c/CS381/as3/src/skybox.cpp

CMakeFiles/as3.dir/src/skybox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/as3.dir/src/skybox.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/CS381/as3/src/skybox.cpp > CMakeFiles/as3.dir/src/skybox.cpp.i

CMakeFiles/as3.dir/src/skybox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/as3.dir/src/skybox.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/CS381/as3/src/skybox.cpp -o CMakeFiles/as3.dir/src/skybox.cpp.s

# Object files for target as3
as3_OBJECTS = \
"CMakeFiles/as3.dir/src/as3.cpp.o" \
"CMakeFiles/as3.dir/src/skybox.cpp.o"

# External object files for target as3
as3_EXTERNAL_OBJECTS =

as3: CMakeFiles/as3.dir/src/as3.cpp.o
as3: CMakeFiles/as3.dir/src/skybox.cpp.o
as3: CMakeFiles/as3.dir/build.make
as3: raylib-cpp/raylib/raylib/libraylib.a
as3: /usr/lib/x86_64-linux-gnu/libOpenGL.so
as3: /usr/lib/x86_64-linux-gnu/libGLX.so
as3: /usr/lib/x86_64-linux-gnu/librt.a
as3: /usr/lib/x86_64-linux-gnu/libm.so
as3: CMakeFiles/as3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/CS381/as3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable as3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/as3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/as3.dir/build: as3
.PHONY : CMakeFiles/as3.dir/build

CMakeFiles/as3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/as3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/as3.dir/clean

CMakeFiles/as3.dir/depend:
	cd /mnt/c/CS381/as3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/CS381/as3 /mnt/c/CS381/as3 /mnt/c/CS381/as3/build /mnt/c/CS381/as3/build /mnt/c/CS381/as3/build/CMakeFiles/as3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/as3.dir/depend

