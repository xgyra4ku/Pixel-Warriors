# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.2.1\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.2.1\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\solov\CLionProjects\Pixel-Warriors

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug

# Include any dependencies generated for this target.
include libs/LibKyraText/CMakeFiles/LibKyraText.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/LibKyraText/CMakeFiles/LibKyraText.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/LibKyraText/CMakeFiles/LibKyraText.dir/progress.make

# Include the compile flags for this target's objects.
include libs/LibKyraText/CMakeFiles/LibKyraText.dir/flags.make

libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj: libs/LibKyraText/CMakeFiles/LibKyraText.dir/flags.make
libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj: libs/LibKyraText/CMakeFiles/LibKyraText.dir/includes_CXX.rsp
libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj: C:/Users/solov/CLionProjects/Pixel-Warriors/libs/LibKyraText/src/LibKyraText.cpp
libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj: libs/LibKyraText/CMakeFiles/LibKyraText.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj"
	cd /d C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText && C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj -MF CMakeFiles\LibKyraText.dir\src\LibKyraText.cpp.obj.d -o CMakeFiles\LibKyraText.dir\src\LibKyraText.cpp.obj -c C:\Users\solov\CLionProjects\Pixel-Warriors\libs\LibKyraText\src\LibKyraText.cpp

libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.i"
	cd /d C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText && C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\solov\CLionProjects\Pixel-Warriors\libs\LibKyraText\src\LibKyraText.cpp > CMakeFiles\LibKyraText.dir\src\LibKyraText.cpp.i

libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.s"
	cd /d C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText && C:\PROGRA~1\JETBRA~1\CLION2~1.1\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\solov\CLionProjects\Pixel-Warriors\libs\LibKyraText\src\LibKyraText.cpp -o CMakeFiles\LibKyraText.dir\src\LibKyraText.cpp.s

# Object files for target LibKyraText
LibKyraText_OBJECTS = \
"CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj"

# External object files for target LibKyraText
LibKyraText_EXTERNAL_OBJECTS =

libs/LibKyraText/libLibKyraText.a: libs/LibKyraText/CMakeFiles/LibKyraText.dir/src/LibKyraText.cpp.obj
libs/LibKyraText/libLibKyraText.a: libs/LibKyraText/CMakeFiles/LibKyraText.dir/build.make
libs/LibKyraText/libLibKyraText.a: libs/LibKyraText/CMakeFiles/LibKyraText.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libLibKyraText.a"
	cd /d C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText && $(CMAKE_COMMAND) -P CMakeFiles\LibKyraText.dir\cmake_clean_target.cmake
	cd /d C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\LibKyraText.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/LibKyraText/CMakeFiles/LibKyraText.dir/build: libs/LibKyraText/libLibKyraText.a
.PHONY : libs/LibKyraText/CMakeFiles/LibKyraText.dir/build

libs/LibKyraText/CMakeFiles/LibKyraText.dir/clean:
	cd /d C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText && $(CMAKE_COMMAND) -P CMakeFiles\LibKyraText.dir\cmake_clean.cmake
.PHONY : libs/LibKyraText/CMakeFiles/LibKyraText.dir/clean

libs/LibKyraText/CMakeFiles/LibKyraText.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\solov\CLionProjects\Pixel-Warriors C:\Users\solov\CLionProjects\Pixel-Warriors\libs\LibKyraText C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText C:\Users\solov\CLionProjects\Pixel-Warriors\cmake-build-debug\libs\LibKyraText\CMakeFiles\LibKyraText.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : libs/LibKyraText/CMakeFiles/LibKyraText.dir/depend

