
# ASM2010
ASM2010 stands for **AS**sembler and e**M**ulator **2010**. It comprises an assembler (AS2010) and a emulator (M2010) for the academic CS2010 instruction set architecture.

Only AS2010 is available now.
## Features
- Full support for CS2010 assembly syntax and machine code
- Both command line interface and GUI (built-in simple text editor) for AS2010
- Easy debug with microoperation or instruction stepping
## Build
**CMake** is used for building this project. Three targets are available: **as2010**, **as2010-gui** and **m2010.** (**asparse** [assembler parser] and **cs2010** [virtual machine] are planned to be library targets in the future, so you don't have to depend on executables). In general, the C standard used here is C99, but as some portions of code are written in C89, there are plans to port all the code to work in C89. You can choose whether to build a target or not by setting CMake options "AS2010_BUILD", "AS2010_GUI_BUILD" and "M2010_BUILD".

###as2010
A CLI assembler that uses only standard library, so can be built in almost any conforming C99 platform. Doesn't need any external dependencies. It only relies on the asparse code (which is provided internally).

###as2010-gui
The external dependency is **IUP** (https://www.tecgraf.puc-rio.br/iup/), which is a lightweight, multiplatform C GUI library. This repository provides an already built version of this library for Windows-MSVC, Windows-MinGW and GNU/Linux-GCC, each of them both in x86 and x64. However, you can choose to link against your own library: install your own version of the library in the lib{arch}/ folder by turning off ASM2010_DEFAULT_LIB, so CMake will tell the linker to look in lib{arch}/ instead of lib{arch}/{platform}. Then, change the CMakeLists file in order to link with your IUP library and all its dependencies by changing the IUP_LIBS list.
    
###m2010
The external dependencies are the same as **as2010-gui** and IUPCD and CD. In Windows, CD also relies on freetype6 and zlib1, but they're all provided. In Unix, however, you will need freetype6 and fontconfig, but you should have them installed if you already have gtk3-dev. Otherwise, you need to install them. Zlib1 seems to be statically linked to the IUP library in the Unix library (?).

## Logisim version
Folder "logisim" contains two implementations of the CS2010: one with IO extension and one without. A port to Digital is under consideration, due to improvements in performance.
