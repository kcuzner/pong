# Pong

Created for BYU Chip Camp

### Kevin Cuzner

This project is designed to be cross-platform and will build on both Windows and
Linux.

## Linux

 * GCC
 * CMake
 * SDL2
 * pkg-config

Instructions:

In the main directory execute the following:

```
mkdir build
cd build
cmake ..
make
```

## Windows

 * [CMake](http://www.cmake.org/files/v3.2/cmake-3.2.2-win32-x86.exe)
 * [CodeBlocks with MinGW](http://sourceforge.net/projects/codeblocks/files/Binaries/13.12/Windows/codeblocks-13.12mingw-setup.exe)

Instructions:

 1. Open cmake-gui
 2. Point the source path towards this project's directory
 3. Point the build path towards this project's directory/build
 4. Click configure. Select CodeBlocks - MinGW Makefiles
 5. Click generate when configure has finished.
 6. Open Code::Blocks
 7. Navigate to this project's directory/build and open the cbp file
 8. In the toolbar, select the 'pong' target (default is 'all')...it's a
    dropdown menu.
 9. Click the build-and-run button (F9)


