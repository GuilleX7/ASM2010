cmake_minimum_required(VERSION 2.8)

project(ASM2010 C)
set(C_STANDARD 99)
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Release)
endif ()

option(AS2010_BUILD "Build AS2010" ON)
option(AS2010_GUI_BUILD "Build AS2010-GUI" ON)
option(ASM2010_DEFAULTLIB "Search for IUP in project lib folders (lib32 and lib64)" ON)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)

set(ASM2010_COMMON_SOURCE
"src/instructions.c"
"src/hash_table.c"
"src/file.c"
)
set(AS2010_COMMON_SOURCE
"src/as2010/as2010_parse.c"
)
set(AS2010_SOURCE
"src/as2010/as2010.c"
)
set(AS2010_GUI_SOURCE
"src/as2010/as2010_gui.c"
)
add_library(asm2010-common STATIC ${ASM2010_COMMON_SOURCE})
add_library(as2010-common STATIC ${AS2010_COMMON_SOURCE})
target_link_libraries(as2010-common "asm2010-common")

set(ASM2010_GUI_BUILD AS2010_GUI_BUILD)

if (ASM2010_GUI_BUILD)
	include_directories("include")
	if (ASM2010_DEFAULTLIB)
		if (WIN32 AND MINGW)
			find_library(IUP NAMES "iup" HINTS "lib/mingw" REQUIRED)
		elseif (WIN32)
			find_library(IUP NAMES "iup" HINTS "lib/win32" REQUIRED)
		else ()
			find_library(IUP NAMES "iup" HINTS "lib/unix" REQUIRED)
		endif (WIN32 AND MINGW)
	else ()
		find_library(IUP NAMES "iup" REQUIRED)
	endif (ASM2010_DEFAULTLIB)
	list(APPEND REQUIRED_GUI_LIBS ${IUP})
	if (WIN32)
		list(APPEND REQUIRED_GUI_LIBS "comctl32")
	elseif (UNIX)
		list(APPEND REQUIRED_GUI_LIBS "gtk-3" "gdk-3" "gdk_pixbuf-2.0" "pangocairo-1.0" "pango-1.0" "cairo" "gobject-2.0" "gmodule-2.0" "glib-2.0" "Xext" "X11" "m")
	endif ()
endif (ASM2010_GUI_BUILD)

if (AS2010_BUILD)
	add_executable(as2010 ${AS2010_SOURCE})
	target_link_libraries(as2010 "as2010-common")
endif (AS2010_BUILD)

if (AS2010_GUI_BUILD)
	add_executable(as2010-gui ${AS2010_GUI_SOURCE})
	target_link_libraries(as2010-gui "as2010-common")
	target_link_libraries(as2010-gui ${REQUIRED_GUI_LIBS})
	if (WIN32)
		if (MSVC)
		  set_target_properties(as2010-gui PROPERTIES LINK_FLAGS "${LINK_FLAGS} /NODEFAULTLIB:LIBCMT")
		endif (MSVC)
		set_target_properties(as2010-gui PROPERTIES WIN32_EXECUTABLE TRUE)
	endif (WIN32)
endif (AS2010_GUI_BUILD)