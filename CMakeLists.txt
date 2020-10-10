cmake_minimum_required(VERSION 3.0)

project(ASM2010 C)
set(C_STANDARD 99)
if (NOT CMAKE_BUILD_TYPE)
	message("Build type not specified: assuming release")
	set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

# Options
option(AS2010_BUILD "Build AS2010" ON)
option(AS2010_GUI_BUILD "Build AS2010-GUI" ON)
option(M2010_BUILD "Build M2010" ON)
option(ASM2010_DEFAULTLIB "Use default static libraries (lib32-64/{compiler}/). If this is turned off, you can put your own libraries in lib32-64/" ON)
if (WIN32 AND MSVC)
	# Disable default manifest
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
endif (WIN32 AND MSVC)
# Tell CMake to distinguish between x32 and x64
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# Includes
include_directories("include")

# Source code
set(ASM2010_COMMON_SOURCE
"src/hash_table.c"
"src/hash_table.h"
"src/utils.c"
"src/utils.h"
"src/mcs.c"
"src/mcs.h"
"src/parse.c"
"src/parse.h"
"src/trace_log.c"
"src/trace_log.h"
"src/about.h"
)
set(CS2010_COMMON_SOURCE
"src/cs2010/cs_instructions.c"
"src/cs2010/cs_instructions.h"
"src/cs2010/cs_memory.h"
"src/cs2010/cs_op.c"
"src/cs2010/cs_op.h"
"src/cs2010/cs_registers.h"
"src/cs2010/cs2010.c"
"src/cs2010/cs2010.h"
)
set(AS2010_COMMON_SOURCE
"src/as2010/as_parse.c"
"src/as2010/as_parse.h"
)
set(AS2010_SOURCE
"src/as2010/as2010.c"
)
set(AS2010_GUI_SOURCE
"src/as2010/as2010_gui.c"
)
set(M2010_SOURCE
"src/m2010/m2010.c"
"src/m2010/comp/m_romgrid.c"
"src/m2010/comp/m_romgrid.h"
)
set(TINYCTHREAD_SOURCE
"include/tinycthread/tinycthread.c"
"include/tinycthread/tinycthread.h"
)

# Object libraries
add_library(asm2010-common OBJECT ${ASM2010_COMMON_SOURCE})
add_library(cs2010-common OBJECT ${CS2010_COMMON_SOURCE})
add_library(as2010-common OBJECT ${AS2010_COMMON_SOURCE})
add_library(tinycthread OBJECT ${TINYCTHREAD_SOURCE})

# Check whether GUI is built or not
set(ASM2010_GUI_BUILD AS2010_GUI_BUILD OR M2010_BUILD)

if (ASM2010_GUI_BUILD)
	# Check for the system
	if (ASM2010_DEFAULTLIB)
		if (WIN32)
			if (MSVC)
				set(DEFAULTLIB_PREFIX "win32")
			else (MSVC)
				set(DEFAULTLIB_PREFIX "mingw")
			endif (MSVC)
		elseif (UNIX)
			set(DEFAULTLIB_PREFIX "unix")
		else ()
			# System not supported
			set(DEFAULTLIB_PREFIX "")
		endif (WIN32)
	else (ASM2010_DEFAULT_LIB)
		set(DEFAULTLIB_PREFIX "")
	endif (ASM2010_DEFAULTLIB)

	# Find IUP dependencies (required for both as2010-gui and m2010)
	find_library(IUP NAMES "iup" HINTS "lib/${DEFAULTLIB_PREFIX}" REQUIRED)
	list(APPEND IUP_LIBS ${IUP})
	if (WIN32)
		list(APPEND IUP_LIBS "comctl32")
	elseif (UNIX)
		list(APPEND IUP_LIBS "gtk-3" "gdk-3" "pangocairo-1.0" "pango-1.0" "atk-1.0" "cairo-gobject" "cairo" "gdk_pixbuf-2.0" "gio-2.0" "gobject-2.0" "glib-2.0" "X11" "m")
	endif (WIN32)

	# Find CD dependencies (only for m2010)
	if (M2010_BUILD)
		find_library(IUPCD NAMES "iupcd" HINTS "lib/${DEFAULTLIB_PREFIX}" REQUIRED)
		find_library(CD NAMES "cd" HINTS "lib/${DEFAULTLIB_PREFIX}" REQUIRED)
		list(APPEND CD_LIBS ${IUPCD} ${CD} ${FREETYPE6})
		if (WIN32)
			find_library(ZLIB1 NAMES "zlib1" HINTS "lib/${DEFAULTLIB_PREFIX}" REQUIRED)
			find_library(FREETYPE6 NAMES "freetype6" HINTS "lib/${DEFAULTLIB_PREFIX}" REQUIRED)
			list(APPEND CD_LIBS ${ZLIB1} ${FREETYPE6})
		elseif (UNIX)
			list(APPEND CD_LIBS "fontconfig" "freetype")
		endif (WIN32)
	endif (M2010_BUILD)
endif (ASM2010_GUI_BUILD)

# AS2010 build
if (AS2010_BUILD)
	add_executable(as2010 ${AS2010_SOURCE} $<TARGET_OBJECTS:asm2010-common> $<TARGET_OBJECTS:cs2010-common> $<TARGET_OBJECTS:as2010-common>)
endif (AS2010_BUILD)

# as2010-gui build
if (AS2010_GUI_BUILD)
	add_executable(as2010-gui ${AS2010_GUI_SOURCE} $<TARGET_OBJECTS:asm2010-common> $<TARGET_OBJECTS:cs2010-common> $<TARGET_OBJECTS:as2010-common>)
	target_link_libraries(as2010-gui ${IUP_LIBS})
	if (WIN32)
		if (${CMAKE_BUILD_TYPE} MATCHES "Release")
			set_target_properties(as2010-gui PROPERTIES WIN32_EXECUTABLE TRUE)
		endif ()
		if (MSVC)
			# Do not link with libcmt (IUP is already linked to)
			target_link_options(as2010-gui PUBLIC "/NODEFAULTLIB:LIBCMT.LIB")
			# Link with IUP manifest
			add_custom_command(
				TARGET as2010-gui
				POST_BUILD
				COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\etc\\win32\\asm2010.manifest\" -outputresource:\"${CMAKE_CURRENT_BINARY_DIR}\\bin\\as2010-gui.exe\"\;\#1
			)
		endif (MSVC)
	endif (WIN32)
endif (AS2010_GUI_BUILD)

# m2010 build
if (M2010_BUILD)
	add_executable(m2010 ${M2010_SOURCE} $<TARGET_OBJECTS:asm2010-common> $<TARGET_OBJECTS:cs2010-common> $<TARGET_OBJECTS:as2010-common> $<TARGET_OBJECTS:tinycthread>)
	target_link_libraries(m2010 ${CD_LIBS} ${IUP_LIBS} )
	if (WIN32)
		if (${CMAKE_BUILD_TYPE} MATCHES "Release")
			set_target_properties(m2010 PROPERTIES WIN32_EXECUTABLE TRUE)
		endif ()
		if (MSVC)
			# Do not link with libcmt (IUP is already linked to)
			target_link_options(m2010 PUBLIC "/NODEFAULTLIB:LIBCMT.LIB")
			# Link with IUP manifest
			add_custom_command(
				TARGET m2010
				POST_BUILD
				COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\etc\\win32\\asm2010.manifest\" -outputresource:\"${CMAKE_CURRENT_BINARY_DIR}\\bin\\m2010.exe\"\;\#1
			)
		endif (MSVC)
	elseif (UNIX)
		target_link_libraries(m2010 "pthread")
	endif (WIN32)
endif (M2010_BUILD)