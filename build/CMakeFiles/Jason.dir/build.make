# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mac/Github/Jason

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mac/Github/Jason/build

# Include any dependencies generated for this target.
include CMakeFiles/Jason.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Jason.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Jason.dir/flags.make

CMakeFiles/Jason.dir/Jason.c.o: CMakeFiles/Jason.dir/flags.make
CMakeFiles/Jason.dir/Jason.c.o: ../Jason.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac/Github/Jason/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Jason.dir/Jason.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Jason.dir/Jason.c.o   -c /Users/mac/Github/Jason/Jason.c

CMakeFiles/Jason.dir/Jason.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Jason.dir/Jason.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mac/Github/Jason/Jason.c > CMakeFiles/Jason.dir/Jason.c.i

CMakeFiles/Jason.dir/Jason.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Jason.dir/Jason.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mac/Github/Jason/Jason.c -o CMakeFiles/Jason.dir/Jason.c.s

CMakeFiles/Jason.dir/Jason.c.o.requires:

.PHONY : CMakeFiles/Jason.dir/Jason.c.o.requires

CMakeFiles/Jason.dir/Jason.c.o.provides: CMakeFiles/Jason.dir/Jason.c.o.requires
	$(MAKE) -f CMakeFiles/Jason.dir/build.make CMakeFiles/Jason.dir/Jason.c.o.provides.build
.PHONY : CMakeFiles/Jason.dir/Jason.c.o.provides

CMakeFiles/Jason.dir/Jason.c.o.provides.build: CMakeFiles/Jason.dir/Jason.c.o


CMakeFiles/Jason.dir/scan.c.o: CMakeFiles/Jason.dir/flags.make
CMakeFiles/Jason.dir/scan.c.o: ../scan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac/Github/Jason/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Jason.dir/scan.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Jason.dir/scan.c.o   -c /Users/mac/Github/Jason/scan.c

CMakeFiles/Jason.dir/scan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Jason.dir/scan.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mac/Github/Jason/scan.c > CMakeFiles/Jason.dir/scan.c.i

CMakeFiles/Jason.dir/scan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Jason.dir/scan.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mac/Github/Jason/scan.c -o CMakeFiles/Jason.dir/scan.c.s

CMakeFiles/Jason.dir/scan.c.o.requires:

.PHONY : CMakeFiles/Jason.dir/scan.c.o.requires

CMakeFiles/Jason.dir/scan.c.o.provides: CMakeFiles/Jason.dir/scan.c.o.requires
	$(MAKE) -f CMakeFiles/Jason.dir/build.make CMakeFiles/Jason.dir/scan.c.o.provides.build
.PHONY : CMakeFiles/Jason.dir/scan.c.o.provides

CMakeFiles/Jason.dir/scan.c.o.provides.build: CMakeFiles/Jason.dir/scan.c.o


CMakeFiles/Jason.dir/symbol.c.o: CMakeFiles/Jason.dir/flags.make
CMakeFiles/Jason.dir/symbol.c.o: ../symbol.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac/Github/Jason/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Jason.dir/symbol.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Jason.dir/symbol.c.o   -c /Users/mac/Github/Jason/symbol.c

CMakeFiles/Jason.dir/symbol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Jason.dir/symbol.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mac/Github/Jason/symbol.c > CMakeFiles/Jason.dir/symbol.c.i

CMakeFiles/Jason.dir/symbol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Jason.dir/symbol.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mac/Github/Jason/symbol.c -o CMakeFiles/Jason.dir/symbol.c.s

CMakeFiles/Jason.dir/symbol.c.o.requires:

.PHONY : CMakeFiles/Jason.dir/symbol.c.o.requires

CMakeFiles/Jason.dir/symbol.c.o.provides: CMakeFiles/Jason.dir/symbol.c.o.requires
	$(MAKE) -f CMakeFiles/Jason.dir/build.make CMakeFiles/Jason.dir/symbol.c.o.provides.build
.PHONY : CMakeFiles/Jason.dir/symbol.c.o.provides

CMakeFiles/Jason.dir/symbol.c.o.provides.build: CMakeFiles/Jason.dir/symbol.c.o


CMakeFiles/Jason.dir/quad.c.o: CMakeFiles/Jason.dir/flags.make
CMakeFiles/Jason.dir/quad.c.o: ../quad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac/Github/Jason/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Jason.dir/quad.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Jason.dir/quad.c.o   -c /Users/mac/Github/Jason/quad.c

CMakeFiles/Jason.dir/quad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Jason.dir/quad.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mac/Github/Jason/quad.c > CMakeFiles/Jason.dir/quad.c.i

CMakeFiles/Jason.dir/quad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Jason.dir/quad.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mac/Github/Jason/quad.c -o CMakeFiles/Jason.dir/quad.c.s

CMakeFiles/Jason.dir/quad.c.o.requires:

.PHONY : CMakeFiles/Jason.dir/quad.c.o.requires

CMakeFiles/Jason.dir/quad.c.o.provides: CMakeFiles/Jason.dir/quad.c.o.requires
	$(MAKE) -f CMakeFiles/Jason.dir/build.make CMakeFiles/Jason.dir/quad.c.o.provides.build
.PHONY : CMakeFiles/Jason.dir/quad.c.o.provides

CMakeFiles/Jason.dir/quad.c.o.provides.build: CMakeFiles/Jason.dir/quad.c.o


CMakeFiles/Jason.dir/object.c.o: CMakeFiles/Jason.dir/flags.make
CMakeFiles/Jason.dir/object.c.o: ../object.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mac/Github/Jason/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Jason.dir/object.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Jason.dir/object.c.o   -c /Users/mac/Github/Jason/object.c

CMakeFiles/Jason.dir/object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Jason.dir/object.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mac/Github/Jason/object.c > CMakeFiles/Jason.dir/object.c.i

CMakeFiles/Jason.dir/object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Jason.dir/object.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mac/Github/Jason/object.c -o CMakeFiles/Jason.dir/object.c.s

CMakeFiles/Jason.dir/object.c.o.requires:

.PHONY : CMakeFiles/Jason.dir/object.c.o.requires

CMakeFiles/Jason.dir/object.c.o.provides: CMakeFiles/Jason.dir/object.c.o.requires
	$(MAKE) -f CMakeFiles/Jason.dir/build.make CMakeFiles/Jason.dir/object.c.o.provides.build
.PHONY : CMakeFiles/Jason.dir/object.c.o.provides

CMakeFiles/Jason.dir/object.c.o.provides.build: CMakeFiles/Jason.dir/object.c.o


# Object files for target Jason
Jason_OBJECTS = \
"CMakeFiles/Jason.dir/Jason.c.o" \
"CMakeFiles/Jason.dir/scan.c.o" \
"CMakeFiles/Jason.dir/symbol.c.o" \
"CMakeFiles/Jason.dir/quad.c.o" \
"CMakeFiles/Jason.dir/object.c.o"

# External object files for target Jason
Jason_EXTERNAL_OBJECTS =

Jason: CMakeFiles/Jason.dir/Jason.c.o
Jason: CMakeFiles/Jason.dir/scan.c.o
Jason: CMakeFiles/Jason.dir/symbol.c.o
Jason: CMakeFiles/Jason.dir/quad.c.o
Jason: CMakeFiles/Jason.dir/object.c.o
Jason: CMakeFiles/Jason.dir/build.make
Jason: CMakeFiles/Jason.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mac/Github/Jason/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable Jason"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Jason.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Jason.dir/build: Jason

.PHONY : CMakeFiles/Jason.dir/build

CMakeFiles/Jason.dir/requires: CMakeFiles/Jason.dir/Jason.c.o.requires
CMakeFiles/Jason.dir/requires: CMakeFiles/Jason.dir/scan.c.o.requires
CMakeFiles/Jason.dir/requires: CMakeFiles/Jason.dir/symbol.c.o.requires
CMakeFiles/Jason.dir/requires: CMakeFiles/Jason.dir/quad.c.o.requires
CMakeFiles/Jason.dir/requires: CMakeFiles/Jason.dir/object.c.o.requires

.PHONY : CMakeFiles/Jason.dir/requires

CMakeFiles/Jason.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Jason.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Jason.dir/clean

CMakeFiles/Jason.dir/depend:
	cd /Users/mac/Github/Jason/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mac/Github/Jason /Users/mac/Github/Jason /Users/mac/Github/Jason/build /Users/mac/Github/Jason/build /Users/mac/Github/Jason/build/CMakeFiles/Jason.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Jason.dir/depend

