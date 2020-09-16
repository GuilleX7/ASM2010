# ASM2010
A simple assembler for CS2010 (also CS3) RISC, register-register ISA, written in C99.
## Features
- Full support for CS2010 assembly syntax
- Both command line interface and GUI (built-in simple text editor)
## Build
**CMake** is used for building this project. Two targets are available: asm2010 (command line interface) and asm2010-gui.
- **CLI** version uses only standard library, so can be built in almost any conforming C99 platform.
- **GUI** version uses **IUP** (https://www.tecgraf.puc-rio.br/iup/), which is a lightweight, multiplatform C GUI library. You can choose whether building this target by changing ASM2010_BUILD_GUI option. This repository provides an already built version of this library for both Windows and GNU/Linux, x86 and x64. However, you can choose to link against your own library: just turn off (or not, if you want CMake to look in project's lib folders) ASM2010_GUI_DEFAULTLIB in CMake cache and install your own version of the library.

Building is quite simple:

    mkdir build
    cd build
    cmake ..
    cmake --build .
