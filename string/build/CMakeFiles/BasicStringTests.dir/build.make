# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/admin/cpp_trainings/standart_library/string

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/admin/cpp_trainings/standart_library/string/build

# Include any dependencies generated for this target.
include CMakeFiles/BasicStringTests.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/BasicStringTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BasicStringTests.dir/flags.make

CMakeFiles/BasicStringTests.dir/test_main.cpp.o: CMakeFiles/BasicStringTests.dir/flags.make
CMakeFiles/BasicStringTests.dir/test_main.cpp.o: ../test_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/cpp_trainings/standart_library/string/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BasicStringTests.dir/test_main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BasicStringTests.dir/test_main.cpp.o -c /home/admin/cpp_trainings/standart_library/string/test_main.cpp

CMakeFiles/BasicStringTests.dir/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BasicStringTests.dir/test_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/cpp_trainings/standart_library/string/test_main.cpp > CMakeFiles/BasicStringTests.dir/test_main.cpp.i

CMakeFiles/BasicStringTests.dir/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BasicStringTests.dir/test_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/cpp_trainings/standart_library/string/test_main.cpp -o CMakeFiles/BasicStringTests.dir/test_main.cpp.s

# Object files for target BasicStringTests
BasicStringTests_OBJECTS = \
"CMakeFiles/BasicStringTests.dir/test_main.cpp.o"

# External object files for target BasicStringTests
BasicStringTests_EXTERNAL_OBJECTS =

BasicStringTests: CMakeFiles/BasicStringTests.dir/test_main.cpp.o
BasicStringTests: CMakeFiles/BasicStringTests.dir/build.make
BasicStringTests: lib/libgtest.a
BasicStringTests: lib/libgtest_main.a
BasicStringTests: lib/libgtest.a
BasicStringTests: CMakeFiles/BasicStringTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/admin/cpp_trainings/standart_library/string/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable BasicStringTests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BasicStringTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BasicStringTests.dir/build: BasicStringTests

.PHONY : CMakeFiles/BasicStringTests.dir/build

CMakeFiles/BasicStringTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BasicStringTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BasicStringTests.dir/clean

CMakeFiles/BasicStringTests.dir/depend:
	cd /home/admin/cpp_trainings/standart_library/string/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/admin/cpp_trainings/standart_library/string /home/admin/cpp_trainings/standart_library/string /home/admin/cpp_trainings/standart_library/string/build /home/admin/cpp_trainings/standart_library/string/build /home/admin/cpp_trainings/standart_library/string/build/CMakeFiles/BasicStringTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BasicStringTests.dir/depend

