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
CMAKE_SOURCE_DIR = /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build

# Include any dependencies generated for this target.
include CMakeFiles/opensky.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/opensky.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/opensky.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/opensky.dir/flags.make

CMakeFiles/opensky.dir/openSkyAPI.cpp.o: CMakeFiles/opensky.dir/flags.make
CMakeFiles/opensky.dir/openSkyAPI.cpp.o: ../openSkyAPI.cpp
CMakeFiles/opensky.dir/openSkyAPI.cpp.o: CMakeFiles/opensky.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/opensky.dir/openSkyAPI.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/opensky.dir/openSkyAPI.cpp.o -MF CMakeFiles/opensky.dir/openSkyAPI.cpp.o.d -o CMakeFiles/opensky.dir/openSkyAPI.cpp.o -c /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/openSkyAPI.cpp

CMakeFiles/opensky.dir/openSkyAPI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opensky.dir/openSkyAPI.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/openSkyAPI.cpp > CMakeFiles/opensky.dir/openSkyAPI.cpp.i

CMakeFiles/opensky.dir/openSkyAPI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opensky.dir/openSkyAPI.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/openSkyAPI.cpp -o CMakeFiles/opensky.dir/openSkyAPI.cpp.s

# Object files for target opensky
opensky_OBJECTS = \
"CMakeFiles/opensky.dir/openSkyAPI.cpp.o"

# External object files for target opensky
opensky_EXTERNAL_OBJECTS =

opensky: CMakeFiles/opensky.dir/openSkyAPI.cpp.o
opensky: CMakeFiles/opensky.dir/build.make
opensky: libutility.a
opensky: CMakeFiles/opensky.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable opensky"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opensky.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/opensky.dir/build: opensky
.PHONY : CMakeFiles/opensky.dir/build

CMakeFiles/opensky.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/opensky.dir/cmake_clean.cmake
.PHONY : CMakeFiles/opensky.dir/clean

CMakeFiles/opensky.dir/depend:
	cd /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build /home/dor/Desktop/openSkyLinux/Linux-OpenSkyAPI-Project/build/CMakeFiles/opensky.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/opensky.dir/depend

