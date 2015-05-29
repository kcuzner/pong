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
 * [MinGW Installer](https://sourceforge.net/projects/mingw/files/latest/download)

Instructions:

 1. Open MinGW Installation Manager
 2. Navigate to All Packages -> MinGW -> MinGW Libraries
 3. Check the boxes next to mingw32-libz dev and dll
 4. Go to Installation -> Apply
 5. Close MinGW Installation Manager
 6. Open cmake-gui
 7. Point the source path towards this project's directory
 8. Point the build path towards this project's directory/build
 9. Click configure. Select CodeBlocks - MinGW Makefiles
 10. Click generate when configure has finished.
 11. Open Code::Blocks
 12. Navigate to this project's directory/build and open the cbp file
 13. Click build


