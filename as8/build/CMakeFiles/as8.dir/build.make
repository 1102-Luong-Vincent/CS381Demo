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
CMAKE_SOURCE_DIR = /mnt/c/CS381/as8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/CS381/as8/build

# Include any dependencies generated for this target.
include CMakeFiles/as8.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/as8.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/as8.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/as8.dir/flags.make

CMakeFiles/as8.dir/src/as8.cpp.o: CMakeFiles/as8.dir/flags.make
CMakeFiles/as8.dir/src/as8.cpp.o: ../src/as8.cpp
CMakeFiles/as8.dir/src/as8.cpp.o: CMakeFiles/as8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/CS381/as8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/as8.dir/src/as8.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/as8.dir/src/as8.cpp.o -MF CMakeFiles/as8.dir/src/as8.cpp.o.d -o CMakeFiles/as8.dir/src/as8.cpp.o -c /mnt/c/CS381/as8/src/as8.cpp

CMakeFiles/as8.dir/src/as8.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/as8.dir/src/as8.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/CS381/as8/src/as8.cpp > CMakeFiles/as8.dir/src/as8.cpp.i

CMakeFiles/as8.dir/src/as8.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/as8.dir/src/as8.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/CS381/as8/src/as8.cpp -o CMakeFiles/as8.dir/src/as8.cpp.s

CMakeFiles/as8.dir/src/skybox.cpp.o: CMakeFiles/as8.dir/flags.make
CMakeFiles/as8.dir/src/skybox.cpp.o: ../src/skybox.cpp
CMakeFiles/as8.dir/src/skybox.cpp.o: CMakeFiles/as8.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/CS381/as8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/as8.dir/src/skybox.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/as8.dir/src/skybox.cpp.o -MF CMakeFiles/as8.dir/src/skybox.cpp.o.d -o CMakeFiles/as8.dir/src/skybox.cpp.o -c /mnt/c/CS381/as8/src/skybox.cpp

CMakeFiles/as8.dir/src/skybox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/as8.dir/src/skybox.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/CS381/as8/src/skybox.cpp > CMakeFiles/as8.dir/src/skybox.cpp.i

CMakeFiles/as8.dir/src/skybox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/as8.dir/src/skybox.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/CS381/as8/src/skybox.cpp -o CMakeFiles/as8.dir/src/skybox.cpp.s

# Object files for target as8
as8_OBJECTS = \
"CMakeFiles/as8.dir/src/as8.cpp.o" \
"CMakeFiles/as8.dir/src/skybox.cpp.o"

# External object files for target as8
as8_EXTERNAL_OBJECTS =

as8: CMakeFiles/as8.dir/src/as8.cpp.o
as8: CMakeFiles/as8.dir/src/skybox.cpp.o
as8: CMakeFiles/as8.dir/build.make
as8: raylib-cpp/raylib/raylib/libraylib.a
as8: raylib-cpp/BufferedRaylib/libbuffered-raylib.a
as8: raylib-cpp/raylib/raylib/libraylib.a
as8: /usr/lib/x86_64-linux-gnu/libOpenGL.so
as8: /usr/lib/x86_64-linux-gnu/libGLX.so
as8: /usr/lib/x86_64-linux-gnu/librt.a
as8: /usr/lib/x86_64-linux-gnu/libm.so
as8: raylib-cpp/BufferedRaylib/FastSignals/libfastsignals/liblibfastsignals.a
as8: CMakeFiles/as8.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/CS381/as8/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable as8"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/as8.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/as8.dir/build: as8
.PHONY : CMakeFiles/as8.dir/build

CMakeFiles/as8.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/as8.dir/cmake_clean.cmake
.PHONY : CMakeFiles/as8.dir/clean

CMakeFiles/as8.dir/depend:
	cd /mnt/c/CS381/as8/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/CS381/as8 /mnt/c/CS381/as8 /mnt/c/CS381/as8/build /mnt/c/CS381/as8/build /mnt/c/CS381/as8/build/CMakeFiles/as8.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/as8.dir/depend

