# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build"

# Include any dependencies generated for this target.
include CMakeFiles/super_res.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/super_res.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/super_res.dir/flags.make

CMakeFiles/super_res.dir/main.cpp.o: CMakeFiles/super_res.dir/flags.make
CMakeFiles/super_res.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/super_res.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/super_res.dir/main.cpp.o -c "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/main.cpp"

CMakeFiles/super_res.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/super_res.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/main.cpp" > CMakeFiles/super_res.dir/main.cpp.i

CMakeFiles/super_res.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/super_res.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/main.cpp" -o CMakeFiles/super_res.dir/main.cpp.s

CMakeFiles/super_res.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/super_res.dir/main.cpp.o.requires

CMakeFiles/super_res.dir/main.cpp.o.provides: CMakeFiles/super_res.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/super_res.dir/build.make CMakeFiles/super_res.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/super_res.dir/main.cpp.o.provides

CMakeFiles/super_res.dir/main.cpp.o.provides.build: CMakeFiles/super_res.dir/main.cpp.o


CMakeFiles/super_res.dir/MedianEstimation.cpp.o: CMakeFiles/super_res.dir/flags.make
CMakeFiles/super_res.dir/MedianEstimation.cpp.o: ../MedianEstimation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/super_res.dir/MedianEstimation.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/super_res.dir/MedianEstimation.cpp.o -c "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/MedianEstimation.cpp"

CMakeFiles/super_res.dir/MedianEstimation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/super_res.dir/MedianEstimation.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/MedianEstimation.cpp" > CMakeFiles/super_res.dir/MedianEstimation.cpp.i

CMakeFiles/super_res.dir/MedianEstimation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/super_res.dir/MedianEstimation.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/MedianEstimation.cpp" -o CMakeFiles/super_res.dir/MedianEstimation.cpp.s

CMakeFiles/super_res.dir/MedianEstimation.cpp.o.requires:

.PHONY : CMakeFiles/super_res.dir/MedianEstimation.cpp.o.requires

CMakeFiles/super_res.dir/MedianEstimation.cpp.o.provides: CMakeFiles/super_res.dir/MedianEstimation.cpp.o.requires
	$(MAKE) -f CMakeFiles/super_res.dir/build.make CMakeFiles/super_res.dir/MedianEstimation.cpp.o.provides.build
.PHONY : CMakeFiles/super_res.dir/MedianEstimation.cpp.o.provides

CMakeFiles/super_res.dir/MedianEstimation.cpp.o.provides.build: CMakeFiles/super_res.dir/MedianEstimation.cpp.o


CMakeFiles/super_res.dir/OpticalFlow.cpp.o: CMakeFiles/super_res.dir/flags.make
CMakeFiles/super_res.dir/OpticalFlow.cpp.o: ../OpticalFlow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/super_res.dir/OpticalFlow.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/super_res.dir/OpticalFlow.cpp.o -c "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/OpticalFlow.cpp"

CMakeFiles/super_res.dir/OpticalFlow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/super_res.dir/OpticalFlow.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/OpticalFlow.cpp" > CMakeFiles/super_res.dir/OpticalFlow.cpp.i

CMakeFiles/super_res.dir/OpticalFlow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/super_res.dir/OpticalFlow.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/OpticalFlow.cpp" -o CMakeFiles/super_res.dir/OpticalFlow.cpp.s

CMakeFiles/super_res.dir/OpticalFlow.cpp.o.requires:

.PHONY : CMakeFiles/super_res.dir/OpticalFlow.cpp.o.requires

CMakeFiles/super_res.dir/OpticalFlow.cpp.o.provides: CMakeFiles/super_res.dir/OpticalFlow.cpp.o.requires
	$(MAKE) -f CMakeFiles/super_res.dir/build.make CMakeFiles/super_res.dir/OpticalFlow.cpp.o.provides.build
.PHONY : CMakeFiles/super_res.dir/OpticalFlow.cpp.o.provides

CMakeFiles/super_res.dir/OpticalFlow.cpp.o.provides.build: CMakeFiles/super_res.dir/OpticalFlow.cpp.o


CMakeFiles/super_res.dir/ImageLoader.cpp.o: CMakeFiles/super_res.dir/flags.make
CMakeFiles/super_res.dir/ImageLoader.cpp.o: ../ImageLoader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/super_res.dir/ImageLoader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/super_res.dir/ImageLoader.cpp.o -c "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/ImageLoader.cpp"

CMakeFiles/super_res.dir/ImageLoader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/super_res.dir/ImageLoader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/ImageLoader.cpp" > CMakeFiles/super_res.dir/ImageLoader.cpp.i

CMakeFiles/super_res.dir/ImageLoader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/super_res.dir/ImageLoader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/ImageLoader.cpp" -o CMakeFiles/super_res.dir/ImageLoader.cpp.s

CMakeFiles/super_res.dir/ImageLoader.cpp.o.requires:

.PHONY : CMakeFiles/super_res.dir/ImageLoader.cpp.o.requires

CMakeFiles/super_res.dir/ImageLoader.cpp.o.provides: CMakeFiles/super_res.dir/ImageLoader.cpp.o.requires
	$(MAKE) -f CMakeFiles/super_res.dir/build.make CMakeFiles/super_res.dir/ImageLoader.cpp.o.provides.build
.PHONY : CMakeFiles/super_res.dir/ImageLoader.cpp.o.provides

CMakeFiles/super_res.dir/ImageLoader.cpp.o.provides.build: CMakeFiles/super_res.dir/ImageLoader.cpp.o


CMakeFiles/super_res.dir/GradientUtils.cpp.o: CMakeFiles/super_res.dir/flags.make
CMakeFiles/super_res.dir/GradientUtils.cpp.o: ../GradientUtils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/super_res.dir/GradientUtils.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/super_res.dir/GradientUtils.cpp.o -c "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/GradientUtils.cpp"

CMakeFiles/super_res.dir/GradientUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/super_res.dir/GradientUtils.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/GradientUtils.cpp" > CMakeFiles/super_res.dir/GradientUtils.cpp.i

CMakeFiles/super_res.dir/GradientUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/super_res.dir/GradientUtils.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/GradientUtils.cpp" -o CMakeFiles/super_res.dir/GradientUtils.cpp.s

CMakeFiles/super_res.dir/GradientUtils.cpp.o.requires:

.PHONY : CMakeFiles/super_res.dir/GradientUtils.cpp.o.requires

CMakeFiles/super_res.dir/GradientUtils.cpp.o.provides: CMakeFiles/super_res.dir/GradientUtils.cpp.o.requires
	$(MAKE) -f CMakeFiles/super_res.dir/build.make CMakeFiles/super_res.dir/GradientUtils.cpp.o.provides.build
.PHONY : CMakeFiles/super_res.dir/GradientUtils.cpp.o.provides

CMakeFiles/super_res.dir/GradientUtils.cpp.o.provides.build: CMakeFiles/super_res.dir/GradientUtils.cpp.o


# Object files for target super_res
super_res_OBJECTS = \
"CMakeFiles/super_res.dir/main.cpp.o" \
"CMakeFiles/super_res.dir/MedianEstimation.cpp.o" \
"CMakeFiles/super_res.dir/OpticalFlow.cpp.o" \
"CMakeFiles/super_res.dir/ImageLoader.cpp.o" \
"CMakeFiles/super_res.dir/GradientUtils.cpp.o"

# External object files for target super_res
super_res_EXTERNAL_OBJECTS =

super_res: CMakeFiles/super_res.dir/main.cpp.o
super_res: CMakeFiles/super_res.dir/MedianEstimation.cpp.o
super_res: CMakeFiles/super_res.dir/OpticalFlow.cpp.o
super_res: CMakeFiles/super_res.dir/ImageLoader.cpp.o
super_res: CMakeFiles/super_res.dir/GradientUtils.cpp.o
super_res: CMakeFiles/super_res.dir/build.make
super_res: CMakeFiles/super_res.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable super_res"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/super_res.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/super_res.dir/build: super_res

.PHONY : CMakeFiles/super_res.dir/build

CMakeFiles/super_res.dir/requires: CMakeFiles/super_res.dir/main.cpp.o.requires
CMakeFiles/super_res.dir/requires: CMakeFiles/super_res.dir/MedianEstimation.cpp.o.requires
CMakeFiles/super_res.dir/requires: CMakeFiles/super_res.dir/OpticalFlow.cpp.o.requires
CMakeFiles/super_res.dir/requires: CMakeFiles/super_res.dir/ImageLoader.cpp.o.requires
CMakeFiles/super_res.dir/requires: CMakeFiles/super_res.dir/GradientUtils.cpp.o.requires

.PHONY : CMakeFiles/super_res.dir/requires

CMakeFiles/super_res.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/super_res.dir/cmake_clean.cmake
.PHONY : CMakeFiles/super_res.dir/clean

CMakeFiles/super_res.dir/depend:
	cd "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository" "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository" "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build" "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build" "/Users/Paul/Documents/Uni/Master/Bildverarbeitung Praktikum/repository/build/CMakeFiles/super_res.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/super_res.dir/depend
