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
include applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/depend.make

# Include the progress variables for this target.
include applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/progress.make

# Include the compile flags for this target's objects.
include applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/flags.make

applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/main.cpp.o: applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/flags.make
applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/main.cpp.o: applications/preProcessing/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Documents/parallelized-fwi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/main.cpp.o"
	cd /home/chris/Documents/parallelized-fwi/applications/preProcessing && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FWI_PreProcess.dir/main.cpp.o -c /home/chris/Documents/parallelized-fwi/applications/preProcessing/main.cpp

applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FWI_PreProcess.dir/main.cpp.i"
	cd /home/chris/Documents/parallelized-fwi/applications/preProcessing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Documents/parallelized-fwi/applications/preProcessing/main.cpp > CMakeFiles/FWI_PreProcess.dir/main.cpp.i

applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FWI_PreProcess.dir/main.cpp.s"
	cd /home/chris/Documents/parallelized-fwi/applications/preProcessing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Documents/parallelized-fwi/applications/preProcessing/main.cpp -o CMakeFiles/FWI_PreProcess.dir/main.cpp.s

# Object files for target FWI_PreProcess
FWI_PreProcess_OBJECTS = \
"CMakeFiles/FWI_PreProcess.dir/main.cpp.o"

# External object files for target FWI_PreProcess
FWI_PreProcess_EXTERNAL_OBJECTS =

runtime/bin/FWI_PreProcess: applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/main.cpp.o
runtime/bin/FWI_PreProcess: applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/build.make
runtime/bin/FWI_PreProcess: runtime/lib/libintegral_forward_model_library.a
runtime/bin/FWI_PreProcess: runtime/lib/libcore_library.a
runtime/bin/FWI_PreProcess: applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chris/Documents/parallelized-fwi/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../runtime/bin/FWI_PreProcess"
	cd /home/chris/Documents/parallelized-fwi/applications/preProcessing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FWI_PreProcess.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/build: runtime/bin/FWI_PreProcess

.PHONY : applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/build

applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/clean:
	cd /home/chris/Documents/parallelized-fwi/applications/preProcessing && $(CMAKE_COMMAND) -P CMakeFiles/FWI_PreProcess.dir/cmake_clean.cmake
.PHONY : applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/clean

applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/depend:
	cd /home/chris/Documents/parallelized-fwi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/Documents/parallelized-fwi /home/chris/Documents/parallelized-fwi/applications/preProcessing /home/chris/Documents/parallelized-fwi /home/chris/Documents/parallelized-fwi/applications/preProcessing /home/chris/Documents/parallelized-fwi/applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : applications/preProcessing/CMakeFiles/FWI_PreProcess.dir/depend

