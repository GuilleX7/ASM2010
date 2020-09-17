# ASM2010
ASM2010 stands for **AS**sembler and e**M**ulator **2010**. It comprises an assembler (AS2010) and a emulator (M2010) for the academic CS2010 instruction set architecture.

Only AS2010 is available now.
## Features
- Full support for CS2010 assembly syntax and machine code
- Both command line interface and GUI (built-in simple text editor) for AS2010
- ~~Easy debug with microoperation or instruction stepping~~ *(M2010 not ready yet)*
## Build
**CMake** is used for building this project. Three targets are available: **as2010**, **as2010-gui** and **m2010.**
- **as2010** is a CLI assembler that uses only standard library, so can be built in almost any conforming C99 platform.
- Both **as2010-gui** and **m2010** use **IUP** (https://www.tecgraf.puc-rio.br/iup/), which is a lightweight, multiplatform C GUI library. This repository provides an already built version of this library for both Windows and GNU/Linux, x86 and x64. However, you can choose to link against your own library: just turn off (or not, if you want CMake to look in project's lib folders) ASM2010_DEFAULTLIB in CMake cache and install your own version of the library.

You can choose whether to build a target or not by setting CMake options "AS2010_BUILD", "AS2010_GUI_BUILD" and "M2010_BUILD".

Once everything is configured, building is quite simple:

    mkdir build
    cd build
    cmake ..
    cmake --build .
