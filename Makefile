# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = "/Applications/CMake 2.8-3.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-3.app/Contents/bin/cmake" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-3.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/julian/Documents/seamless

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/julian/Documents/seamless

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"/Applications/CMake 2.8-3.app/Contents/bin/ccmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"/Applications/CMake 2.8-3.app/Contents/bin/cmake" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/julian/Documents/seamless/CMakeFiles /Users/julian/Documents/seamless/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/julian/Documents/seamless/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named seamless

# Build rule for target.
seamless: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 seamless
.PHONY : seamless

# fast build rule for target.
seamless/fast:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/build
.PHONY : seamless/fast

SDLMain.o: SDLMain.m.o
.PHONY : SDLMain.o

# target to build an object file
SDLMain.m.o:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/SDLMain.m.o
.PHONY : SDLMain.m.o

SDLMain.i: SDLMain.m.i
.PHONY : SDLMain.i

# target to preprocess a source file
SDLMain.m.i:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/SDLMain.m.i
.PHONY : SDLMain.m.i

SDLMain.s: SDLMain.m.s
.PHONY : SDLMain.s

# target to generate assembly for a file
SDLMain.m.s:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/SDLMain.m.s
.PHONY : SDLMain.m.s

array_image.o: array_image.c.o
.PHONY : array_image.o

# target to build an object file
array_image.c.o:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/array_image.c.o
.PHONY : array_image.c.o

array_image.i: array_image.c.i
.PHONY : array_image.i

# target to preprocess a source file
array_image.c.i:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/array_image.c.i
.PHONY : array_image.c.i

array_image.s: array_image.c.s
.PHONY : array_image.s

# target to generate assembly for a file
array_image.c.s:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/array_image.c.s
.PHONY : array_image.c.s

energy_functions.o: energy_functions.c.o
.PHONY : energy_functions.o

# target to build an object file
energy_functions.c.o:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/energy_functions.c.o
.PHONY : energy_functions.c.o

energy_functions.i: energy_functions.c.i
.PHONY : energy_functions.i

# target to preprocess a source file
energy_functions.c.i:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/energy_functions.c.i
.PHONY : energy_functions.c.i

energy_functions.s: energy_functions.c.s
.PHONY : energy_functions.s

# target to generate assembly for a file
energy_functions.c.s:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/energy_functions.c.s
.PHONY : energy_functions.c.s

main.o: main.c.o
.PHONY : main.o

# target to build an object file
main.c.o:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/main.c.o
.PHONY : main.c.o

main.i: main.c.i
.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s
.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/main.c.s
.PHONY : main.c.s

pixel_access.o: pixel_access.c.o
.PHONY : pixel_access.o

# target to build an object file
pixel_access.c.o:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/pixel_access.c.o
.PHONY : pixel_access.c.o

pixel_access.i: pixel_access.c.i
.PHONY : pixel_access.i

# target to preprocess a source file
pixel_access.c.i:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/pixel_access.c.i
.PHONY : pixel_access.c.i

pixel_access.s: pixel_access.c.s
.PHONY : pixel_access.s

# target to generate assembly for a file
pixel_access.c.s:
	$(MAKE) -f CMakeFiles/seamless.dir/build.make CMakeFiles/seamless.dir/pixel_access.c.s
.PHONY : pixel_access.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... seamless"
	@echo "... SDLMain.o"
	@echo "... SDLMain.i"
	@echo "... SDLMain.s"
	@echo "... array_image.o"
	@echo "... array_image.i"
	@echo "... array_image.s"
	@echo "... energy_functions.o"
	@echo "... energy_functions.i"
	@echo "... energy_functions.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... pixel_access.o"
	@echo "... pixel_access.i"
	@echo "... pixel_access.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

