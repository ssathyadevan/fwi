# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/Documents/parallelized-fwi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/Documents/parallelized-fwi

# Include any dependencies generated for this target.
include libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/depend.make

# Include the progress variables for this target.
include libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/progress.make

# Include the compile flags for this target's objects.
include libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/flags.make

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversion.cpp.o: libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/flags.make
libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversion.cpp.o: libraries/inversion/randomInversion/src/randomInversion.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Documents/parallelized-fwi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversion.cpp.o"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/random_inversion_library.dir/randomInversion.cpp.o -c /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/randomInversion.cpp

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversion.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/random_inversion_library.dir/randomInversion.cpp.i"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/randomInversion.cpp > CMakeFiles/random_inversion_library.dir/randomInversion.cpp.i

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversion.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/random_inversion_library.dir/randomInversion.cpp.s"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/randomInversion.cpp -o CMakeFiles/random_inversion_library.dir/randomInversion.cpp.s

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.o: libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/flags.make
libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.o: libraries/inversion/randomInversion/src/randomInversionInputCardReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Documents/parallelized-fwi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.o"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.o -c /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/randomInversionInputCardReader.cpp

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.i"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/randomInversionInputCardReader.cpp > CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.i

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.s"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/randomInversionInputCardReader.cpp -o CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.s

# Object files for target random_inversion_library
random_inversion_library_OBJECTS = \
"CMakeFiles/random_inversion_library.dir/randomInversion.cpp.o" \
"CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.o"

# External object files for target random_inversion_library
random_inversion_library_EXTERNAL_OBJECTS =

runtime/lib/librandom_inversion_library.a: libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversion.cpp.o
runtime/lib/librandom_inversion_library.a: libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/randomInversionInputCardReader.cpp.o
runtime/lib/librandom_inversion_library.a: libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/build.make
runtime/lib/librandom_inversion_library.a: libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chris/Documents/parallelized-fwi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../../../runtime/lib/librandom_inversion_library.a"
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && $(CMAKE_COMMAND) -P CMakeFiles/random_inversion_library.dir/cmake_clean_target.cmake
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/random_inversion_library.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/build: runtime/lib/librandom_inversion_library.a

.PHONY : libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/build

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/clean:
	cd /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src && $(CMAKE_COMMAND) -P CMakeFiles/random_inversion_library.dir/cmake_clean.cmake
.PHONY : libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/clean

libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/depend:
	cd /home/chris/Documents/parallelized-fwi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/Documents/parallelized-fwi /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src /home/chris/Documents/parallelized-fwi /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src /home/chris/Documents/parallelized-fwi/libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libraries/inversion/randomInversion/src/CMakeFiles/random_inversion_library.dir/depend

