cmake_minimum_required(VERSION 2.8)

project(ASM2010 C)
set(C_STANDARD 99)
if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE Release)
endif ()

option(AS2010_DEFAULTLIB "Search for IUP in project lib folders (lib32 and lib64)" ON)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB32_PATHS TRUE)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)

include_directories("include/iup")

set(COMMON_SOURCE
"src/instructions.c"
"src/hash_table.c"
"src/file.c"
)
set(AS2010_SOURCE
"src/as/as_parse.c"
"src/as/as_cli.c"
)
set(AS2010_GUI_SOURCE
"src/as/as_parse.c"
"src/as/as_gui.c"
)

option(AS2010_BUILD "Build AS2010" ON)
if (AS2010_BUILD)
	add_executable(as2010 ${COMMON_SOURCE} ${AS2010_SOURCE})
endif (AS2010_BUILD)

option(AS2010_GUI_BUILD "Build AS2010-GUI" ON)
if (AS2010_GUI_BUILD)
	add_executable(as2010-gui ${COMMON_SOURCE} ${AS2010_GUI_SOURCE})

	if (AS2010_DEFAULTLIB)
		find_library(IUP NAMES iup HINTS "lib" REQUIRED)
	else ()
		find_library(IUP NAMES iup REQUIRED)
	endif (AS2010_DEFAULTLIB)
	list(APPEND REQUIRED_LIBS ${IUP})

	if (WIN32)
		if (MSVC)
		  set_target_properties(as2010-gui PROPERTIES LINK_FLAGS "${LINK_FLAGS} /NODEFAULTLIB:LIBCMT")
		endif (MSVC)
		set_target_properties(as2010-gui PROPERTIES WIN32_EXECUTABLE TRUE)
		list(APPEND REQUIRED_LIBS "comctl32")
	elseif (UNIX)
		list(APPEND REQUIRED_LIBS "gtk-3" "gdk-3" "gdk_pixbuf-2.0" "pangocairo-1.0" "pango-1.0" "cairo" "gobject-2.0" "gmodule-2.0" "glib-2.0" "Xext" "X11" "m")
	endif ()
	target_link_libraries(as2010-gui PUBLIC ${REQUIRED_LIBS})
endif (AS2010_GUI_BUILD)