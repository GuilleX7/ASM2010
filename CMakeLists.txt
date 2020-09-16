cmake_minimum_required(VERSION 2.8)

project(ASM2010 C)
set(C_STANDARD 99)
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Release)
endif ()


set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)

option(ASM2010_BUILD_GUI "Builds graphical user interface for ASM2010" ON)
option(ASM2010_GUI_DEFAULTLIB "Search for IUP in project lib folders (lib32 and lib64)" ON)

set(COMMON_SOURCE
"src/parse.c"
"src/instructions.c"
"src/hash_table.c"
"src/export.c"
)
set(CLI_SOURCE
"src/cli/cli.c"
)
set(GUI_SOURCE
"src/gui/gui.c"
)

add_executable(asm2010 ${COMMON_SOURCE} ${CLI_SOURCE})

if (ASM2010_BUILD_GUI)
	add_executable(asm2010-gui ${COMMON_SOURCE} ${GUI_SOURCE})
	if (ASM2010_GUI_DEFAULTLIB)
		find_library(IUP NAMES iup HINTS "lib" REQUIRED)
	else ()
		find_library(IUP NAMES iup REQUIRED)
	endif (ASM2010_GUI_DEFAULTLIB)

	list(APPEND REQUIRED_LIBS ${IUP})
	if (WIN32)
		if (MSVC)
		  set_target_properties(asm2010-gui PROPERTIES LINK_FLAGS "${LINK_FLAGS} /NODEFAULTLIB:LIBCMT")
		endif (MSVC)
		set_target_properties(asm2010-gui PROPERTIES WIN32_EXECUTABLE TRUE)
		list(APPEND REQUIRED_LIBS "comctl32")
	elseif (UNIX)
		list(APPEND REQUIRED_LIBS "gtk-3" "gdk-3" "gdk_pixbuf-2.0" "pangocairo-1.0" "pango-1.0" "cairo" "gobject-2.0" "gmodule-2.0" "glib-2.0" "Xext" "X11" "m")
	endif ()
	target_link_libraries(asm2010-gui PUBLIC ${REQUIRED_LIBS})
endif (ASM2010_BUILD_GUI)