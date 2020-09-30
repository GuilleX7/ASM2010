cmake_minimum_required(VERSION 3.0)

project(ASM2010 C)
set(C_STANDARD 99)
if (NOT CMAKE_BUILD_TYPE)
	message("Build type not specified: assuming release")
	set(CMAKE_BUILD_TYPE "Release")
endif (NOT_CMAKE_BUILD_TYPE)

# Set debug flag (not used yet)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(ASM2010_DEBUG ON)
endif (CMAKE_BUILD_TYPE MATCHES "Debug")

# Options
option(AS2010_BUILD "Build AS2010" ON)
option(AS2010_GUI_BUILD "Build AS2010-GUI" ON)
option(M2010_BUILD "Build M2010" ON)
option(ASM2010_DEFAULTLIB "Search for IUP in project lib folders (lib32 and lib64)" ON)
# Tell CMake to distinguish between x32 and x64
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# Source code
set(ASM2010_COMMON_SOURCE
"src/hash_table.c"
"src/utils.c"
"src/mcs.c"
)
set(CS2010_COMMON_SOURCE
"src/cs2010/cs_instructions.c"
)
set(AS2010_COMMON_SOURCE
"src/as2010/as_parse.c"
)
set(AS2010_SOURCE
"src/as2010/as2010.c"
)
set(AS2010_GUI_SOURCE
"src/as2010/as2010_gui.c"
)
set(M2010_SOURCE
"src/m2010/m2010.c"
)

# Object libraries
add_library(asm2010-common OBJECT ${ASM2010_COMMON_SOURCE})
add_library(cs2010-common OBJECT ${CS2010_COMMON_SOURCE})
add_library(as2010-common OBJECT ${AS2010_COMMON_SOURCE})

# Check whether GUI is built or not
set(ASM2010_GUI_BUILD AS2010_GUI_BUILD OR M2010_BUILD)

if (ASM2010_GUI_BUILD)
	# Includes
	include_directories("include")
	# Find IUP library
	if (ASM2010_DEFAULTLIB)
		if (WIN32 AND MSVC)
			find_library(IUP NAMES "iup" HINTS "lib/win32" REQUIRED)
			# Disable default libcmt
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
			# Disable default manifest
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
		elseif (WIN32)
			find_library(IUP NAMES "iup" HINTS "lib/mingw" REQUIRED)
		else ()
			find_library(IUP NAMES "iup" HINTS "lib/unix" REQUIRED)
		endif (WIN32 AND MSVC)
	else ()
		find_library(IUP NAMES "iup" REQUIRED)
	endif (ASM2010_DEFAULTLIB)
	# Find IUP dependencies
	list(APPEND REQUIRED_GUI_LIBS ${IUP})
	if (WIN32)
		list(APPEND REQUIRED_GUI_LIBS "comctl32")
	elseif (UNIX)
		list(APPEND REQUIRED_GUI_LIBS "gtk-3" "gdk-3" "gdk_pixbuf-2.0" "pangocairo-1.0" "pango-1.0" "cairo" "gobject-2.0" "gmodule-2.0" "glib-2.0" "Xext" "X11" "m")
	endif ()
endif (ASM2010_GUI_BUILD)

# as2010 build
if (AS2010_BUILD)
	add_executable(as2010 ${AS2010_SOURCE} $<TARGET_OBJECTS:asm2010-common> $<TARGET_OBJECTS:cs2010-common> $<TARGET_OBJECTS:as2010-common>)
endif (AS2010_BUILD)

# as2010-gui build
if (AS2010_GUI_BUILD)
	add_executable(as2010-gui ${AS2010_GUI_SOURCE} $<TARGET_OBJECTS:asm2010-common> $<TARGET_OBJECTS:cs2010-common> $<TARGET_OBJECTS:as2010-common>)
	target_link_libraries(as2010-gui ${REQUIRED_GUI_LIBS})
	if (WIN32)
		message(${CMAKE_BUILD_TYPE})
		if (${CMAKE_BUILD_TYPE} MATCHES "Release")
			set_target_properties(as2010-gui PROPERTIES WIN32_EXECUTABLE TRUE)
		endif ()
		if (MSVC)
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
	add_executable(m2010 ${M2010_SOURCE} $<TARGET_OBJECTS:asm2010-common> $<TARGET_OBJECTS:cs2010-common>)
	target_link_libraries(m2010 ${REQUIRED_GUI_LIBS})
	if (WIN32)
		if (${CMAKE_BUILD_TYPE} MATCHES "Release")
			set_target_properties(m2010 PROPERTIES WIN32_EXECUTABLE TRUE)
		endif ()
		if (MSVC)
			# Link with IUP manifest
			add_custom_command(
			   TARGET m2010
			   POST_BUILD
			   COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\etc\\win32\\asm2010.manifest\" -outputresource:\"${CMAKE_CURRENT_BINARY_DIR}\\bin\\m2010.exe\"\;\#1
			)
		endif (MSVC)
	endif (WIN32)
endif (M2010_BUILD)