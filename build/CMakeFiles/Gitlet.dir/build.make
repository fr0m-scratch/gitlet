# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.28.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.28.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build"

# Include any dependencies generated for this target.
include CMakeFiles/Gitlet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Gitlet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Gitlet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Gitlet.dir/flags.make

CMakeFiles/Gitlet.dir/src/main.cpp.o: CMakeFiles/Gitlet.dir/flags.make
CMakeFiles/Gitlet.dir/src/main.cpp.o: /Users/jamalcao/Dropbox/My\ Mac\ (Jamal’s\ MacBook\ Pro)/Desktop/miscellaneous/gitlet/src/main.cpp
CMakeFiles/Gitlet.dir/src/main.cpp.o: CMakeFiles/Gitlet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Gitlet.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Gitlet.dir/src/main.cpp.o -MF CMakeFiles/Gitlet.dir/src/main.cpp.o.d -o CMakeFiles/Gitlet.dir/src/main.cpp.o -c "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/main.cpp"

CMakeFiles/Gitlet.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Gitlet.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/main.cpp" > CMakeFiles/Gitlet.dir/src/main.cpp.i

CMakeFiles/Gitlet.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Gitlet.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/main.cpp" -o CMakeFiles/Gitlet.dir/src/main.cpp.s

CMakeFiles/Gitlet.dir/src/Commit.cpp.o: CMakeFiles/Gitlet.dir/flags.make
CMakeFiles/Gitlet.dir/src/Commit.cpp.o: /Users/jamalcao/Dropbox/My\ Mac\ (Jamal’s\ MacBook\ Pro)/Desktop/miscellaneous/gitlet/src/Commit.cpp
CMakeFiles/Gitlet.dir/src/Commit.cpp.o: CMakeFiles/Gitlet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Gitlet.dir/src/Commit.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Gitlet.dir/src/Commit.cpp.o -MF CMakeFiles/Gitlet.dir/src/Commit.cpp.o.d -o CMakeFiles/Gitlet.dir/src/Commit.cpp.o -c "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Commit.cpp"

CMakeFiles/Gitlet.dir/src/Commit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Gitlet.dir/src/Commit.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Commit.cpp" > CMakeFiles/Gitlet.dir/src/Commit.cpp.i

CMakeFiles/Gitlet.dir/src/Commit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Gitlet.dir/src/Commit.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Commit.cpp" -o CMakeFiles/Gitlet.dir/src/Commit.cpp.s

CMakeFiles/Gitlet.dir/src/Repo.cpp.o: CMakeFiles/Gitlet.dir/flags.make
CMakeFiles/Gitlet.dir/src/Repo.cpp.o: /Users/jamalcao/Dropbox/My\ Mac\ (Jamal’s\ MacBook\ Pro)/Desktop/miscellaneous/gitlet/src/Repo.cpp
CMakeFiles/Gitlet.dir/src/Repo.cpp.o: CMakeFiles/Gitlet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Gitlet.dir/src/Repo.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Gitlet.dir/src/Repo.cpp.o -MF CMakeFiles/Gitlet.dir/src/Repo.cpp.o.d -o CMakeFiles/Gitlet.dir/src/Repo.cpp.o -c "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Repo.cpp"

CMakeFiles/Gitlet.dir/src/Repo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Gitlet.dir/src/Repo.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Repo.cpp" > CMakeFiles/Gitlet.dir/src/Repo.cpp.i

CMakeFiles/Gitlet.dir/src/Repo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Gitlet.dir/src/Repo.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Repo.cpp" -o CMakeFiles/Gitlet.dir/src/Repo.cpp.s

CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o: CMakeFiles/Gitlet.dir/flags.make
CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o: /Users/jamalcao/Dropbox/My\ Mac\ (Jamal’s\ MacBook\ Pro)/Desktop/miscellaneous/gitlet/src/StagingArea.cpp
CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o: CMakeFiles/Gitlet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o -MF CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o.d -o CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o -c "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/StagingArea.cpp"

CMakeFiles/Gitlet.dir/src/StagingArea.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Gitlet.dir/src/StagingArea.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/StagingArea.cpp" > CMakeFiles/Gitlet.dir/src/StagingArea.cpp.i

CMakeFiles/Gitlet.dir/src/StagingArea.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Gitlet.dir/src/StagingArea.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/StagingArea.cpp" -o CMakeFiles/Gitlet.dir/src/StagingArea.cpp.s

CMakeFiles/Gitlet.dir/src/Utils.cpp.o: CMakeFiles/Gitlet.dir/flags.make
CMakeFiles/Gitlet.dir/src/Utils.cpp.o: /Users/jamalcao/Dropbox/My\ Mac\ (Jamal’s\ MacBook\ Pro)/Desktop/miscellaneous/gitlet/src/Utils.cpp
CMakeFiles/Gitlet.dir/src/Utils.cpp.o: CMakeFiles/Gitlet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Gitlet.dir/src/Utils.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Gitlet.dir/src/Utils.cpp.o -MF CMakeFiles/Gitlet.dir/src/Utils.cpp.o.d -o CMakeFiles/Gitlet.dir/src/Utils.cpp.o -c "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Utils.cpp"

CMakeFiles/Gitlet.dir/src/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Gitlet.dir/src/Utils.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Utils.cpp" > CMakeFiles/Gitlet.dir/src/Utils.cpp.i

CMakeFiles/Gitlet.dir/src/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Gitlet.dir/src/Utils.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/src/Utils.cpp" -o CMakeFiles/Gitlet.dir/src/Utils.cpp.s

# Object files for target Gitlet
Gitlet_OBJECTS = \
"CMakeFiles/Gitlet.dir/src/main.cpp.o" \
"CMakeFiles/Gitlet.dir/src/Commit.cpp.o" \
"CMakeFiles/Gitlet.dir/src/Repo.cpp.o" \
"CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o" \
"CMakeFiles/Gitlet.dir/src/Utils.cpp.o"

# External object files for target Gitlet
Gitlet_EXTERNAL_OBJECTS =

Gitlet: CMakeFiles/Gitlet.dir/src/main.cpp.o
Gitlet: CMakeFiles/Gitlet.dir/src/Commit.cpp.o
Gitlet: CMakeFiles/Gitlet.dir/src/Repo.cpp.o
Gitlet: CMakeFiles/Gitlet.dir/src/StagingArea.cpp.o
Gitlet: CMakeFiles/Gitlet.dir/src/Utils.cpp.o
Gitlet: CMakeFiles/Gitlet.dir/build.make
Gitlet: /Users/jamalcao/anaconda3/lib/libboost_serialization.dylib
Gitlet: /usr/local/Cellar/openssl@3/3.1.4/lib/libssl.dylib
Gitlet: /usr/local/Cellar/openssl@3/3.1.4/lib/libcrypto.dylib
Gitlet: CMakeFiles/Gitlet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Gitlet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Gitlet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Gitlet.dir/build: Gitlet
.PHONY : CMakeFiles/Gitlet.dir/build

CMakeFiles/Gitlet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Gitlet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Gitlet.dir/clean

CMakeFiles/Gitlet.dir/depend:
	cd "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet" "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet" "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build" "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build" "/Users/jamalcao/Dropbox/My Mac (Jamal’s MacBook Pro)/Desktop/miscellaneous/gitlet/build/CMakeFiles/Gitlet.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/Gitlet.dir/depend

