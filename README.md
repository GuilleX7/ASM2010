
# ASM2010
ASM2010 stands for **AS**sembler and e**M**ulator **2010**. It comprises an assembler (AS2010) and a emulator (M2010) for the academic CS2010 instruction set architecture. Please see https://www.dte.us.es/docencia/etsii/gii-is/estructura-de-computadores/grupo-4-2018/CS2010-2013.pdf.

## Features
- Full support for CS2010 assembly syntax and machine code
- Both command line interface and GUI (built-in simple text editor) for AS2010
- Easy debug with microoperation or instruction stepping with M2010

## Build
**CMake** is used for building this project. Three targets are available: **as2010**, **as2010-gui** and **m2010**. You can choose whether to build a target or not by setting CMake options "AS2010_BUILD", "AS2010_GUI_BUILD" and "M2010_BUILD".

All source code is written in compliant C89, trying to ensure portability across implementations.

### Targets

#### as2010
No external dependencies.

#### as2010-gui & m2010
The external dependency is **IUP** (https://www.tecgraf.puc-rio.br/iup/), which is a lightweight, multiplatform C GUI library. This repository provides an already built version of this library for Windows-MSVC, Windows-MinGW and GNU/Linux-GCC, each of them both in x86 and x64. However, you can choose to link against your own library: install your own version of the library in the lib{arch}/ folder by turning off ASM2010_DEFAULT_LIB, so CMake will tell the linker to look in lib{arch}/ instead of lib{arch}/{platform}. Then, change the CMakeLists file in order to link with your IUP library and all its dependencies by changing the IUP_LIBS list.

## Logisim version
The directory named "logisim" contains two Logisim implementations of the CS2010: one extended with IO and one without.
