cmake_minimum_required(VERSION 3.13)
project(ASM2010 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_SHARED_LIBRARY_PREFIX_C "")
set(CMAKE_STATIC_LIBRARY_PREFIX_C "lib")

option(ASM2010_BUILD_STATIC "Build a static library" ON)
option(ASM2010_BUILD_SHARED "Build a shared library" ON)

set(ASM2010_SOURCE
"src/utils.h"
"src/trace_log.h"
"src/trace_log.c"
"src/parse.h"
"src/parse.c"
"src/hash_table.c"
"src/hash_table.h"
"src/m2010/cs.h"
"src/m2010/cs.c"
"src/m2010/cs_signals.h"
"src/m2010/cs_registers.h"
"src/m2010/cs_platforms.h"
"src/m2010/cs_opcodes.h"
"src/m2010/cs_opcodes.c"
"src/m2010/cs_memory.h"
"src/m2010/cs_io.c"
"src/m2010/cs_instructions.h"
"src/m2010/cs_instructions.c"
"src/m2010/cs2010/cs2010_signals.h"
"src/m2010/cs2010/cs2010_platform.h"
"src/m2010/cs2010/cs2010_platform.c"
"src/m2010/cs2010/cs2010_opcodes.h"
"src/m2010/cs2010/cs2010_opcodes.c"
"src/m2010/cs3/cs3_signals.h"
"src/m2010/cs3/cs3_registers.h"
"src/m2010/cs3/cs3_platform.h"
"src/m2010/cs3/cs3_platform.c"
"src/m2010/cs3/cs3_opcodes.h"
"src/m2010/cs3/cs3_opcodes.c"
"src/as_parse/as_parse.h"
"src/as_parse/as_parse.c"
"src/as_parse/as_disassemble.h"
"src/as_parse/as_disassemble.c"
"include/asm2010.h"
)

if(MSVC)
    set(ASM2010_COMPILE_OPTIONS "/W2" "$<$<CONFIG:RELEASE>:/O2>")
else()
    set(ASM2010_COMPILE_OPTIONS "-Wall" "-Wextra" "$<$<CONFIG:RELEASE>:-O3>")
endif()
set(ASM2010_PROPERTIES OUTPUT_NAME asm2010 C_VISIBILITY_PRESET hidden)

if(ASM2010_BUILD_STATIC)
    add_library(ASM2010_static STATIC ${ASM2010_SOURCE})
    set_target_properties(ASM2010_static PROPERTIES ${ASM2010_PROPERTIES})
    target_compile_options(ASM2010_static PUBLIC ${ASM2010_COMPILE_OPTIONS})
endif()

if(ASM2010_BUILD_SHARED)
    add_library(ASM2010_shared SHARED ${ASM2010_SOURCE})
    set_target_properties(ASM2010_shared PROPERTIES ${ASM2010_PROPERTIES})
    target_compile_options(ASM2010_shared PUBLIC ${ASM2010_COMPILE_OPTIONS})

    # Some tweaks for WASM/WASI 
    if (WASI)
        target_link_options(ASM2010_shared PUBLIC "-Wl,--no-entry" "-nostartfiles" "-Wl,--export-dynamic" "-v")
        set_target_properties(ASM2010_shared PROPERTIES SUFFIX ".wasm")
    endif()
endif()

