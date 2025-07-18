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
CMAKE_SOURCE_DIR = /mnt/c/CS381/as0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/CS381/as0/src

# Include any dependencies generated for this target.
include raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/compiler_depend.make

# Include the progress variables for this target.
include raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/progress.make

# Include the compile flags for this target's objects.
include raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/flags.make

raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.o: raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/flags.make
raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.o: ../raylib-cpp/BufferedRaylib/examples/test.cpp
raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.o: raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/CS381/as0/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.o"
	cd /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.o -MF CMakeFiles/tst.dir/examples/test.cpp.o.d -o CMakeFiles/tst.dir/examples/test.cpp.o -c /mnt/c/CS381/as0/raylib-cpp/BufferedRaylib/examples/test.cpp

raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tst.dir/examples/test.cpp.i"
	cd /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/CS381/as0/raylib-cpp/BufferedRaylib/examples/test.cpp > CMakeFiles/tst.dir/examples/test.cpp.i

raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tst.dir/examples/test.cpp.s"
	cd /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/CS381/as0/raylib-cpp/BufferedRaylib/examples/test.cpp -o CMakeFiles/tst.dir/examples/test.cpp.s

# Object files for target tst
tst_OBJECTS = \
"CMakeFiles/tst.dir/examples/test.cpp.o"

# External object files for target tst
tst_EXTERNAL_OBJECTS =

raylib-cpp/BufferedRaylib/tst: raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/examples/test.cpp.o
raylib-cpp/BufferedRaylib/tst: raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/build.make
raylib-cpp/BufferedRaylib/tst: raylib-cpp/BufferedRaylib/libbuffered-raylib.a
raylib-cpp/BufferedRaylib/tst: raylib-cpp/raylib/raylib/libraylib.a
raylib-cpp/BufferedRaylib/tst: /usr/lib/x86_64-linux-gnu/libOpenGL.so
raylib-cpp/BufferedRaylib/tst: /usr/lib/x86_64-linux-gnu/libGLX.so
raylib-cpp/BufferedRaylib/tst: /usr/lib/x86_64-linux-gnu/librt.a
raylib-cpp/BufferedRaylib/tst: /usr/lib/x86_64-linux-gnu/libm.so
raylib-cpp/BufferedRaylib/tst: raylib-cpp/BufferedRaylib/FastSignals/libfastsignals/liblibfastsignals.a
raylib-cpp/BufferedRaylib/tst: raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/CS381/as0/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tst"
	cd /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/build: raylib-cpp/BufferedRaylib/tst
.PHONY : raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/build

raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/clean:
	cd /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib && $(CMAKE_COMMAND) -P CMakeFiles/tst.dir/cmake_clean.cmake
.PHONY : raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/clean

raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/depend:
	cd /mnt/c/CS381/as0/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/CS381/as0 /mnt/c/CS381/as0/raylib-cpp/BufferedRaylib /mnt/c/CS381/as0/src /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib /mnt/c/CS381/as0/src/raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : raylib-cpp/BufferedRaylib/CMakeFiles/tst.dir/depend

