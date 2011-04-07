# Windows file version
if (WIN32)
	if (MINGW)
		message(WARNING "CMake doesn't implement linking with .RC files for MinGW.")
	else ()
		set (qfm_SOURCES ${qfm_SOURCES} resources/winapp.rc PARENT_SCOPE)
	endif ()

#	windres -i resources\winapp.rc -o debug\winapp_res.o --include-dir=./resources $(qfm_DEFINITIONS)
#if (WIN32)
#set (qfm_SOURCES ${qfm_SOURCES} resources/winapp.rc)
#endif(WIN32)

endif ()

# Resources
file (GLOB_RECURSE src_RESOURCES RELATIVE ${qfm_SOURCE_DIR} FOLLOW_SYMLINKS "*.qrc")
set (qfm_RESOURCES ${qfm_RESOURCES} ${src_RESOURCES} PARENT_SCOPE)
