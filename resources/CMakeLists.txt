# Windows file version
if (WIN32)
	if (MINGW)
		message(WARNING "CMake doesn't implement linking with .RC files for MinGW.")
	else ()
		set (${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} resources/winapp.rc PARENT_SCOPE)
	endif ()

#	windres -i resources\winapp.rc -o debug\winapp_res.o --include-dir=./resources $(${PROJECT_NAME}_DEFINITIONS)
#if (WIN32)
#set (${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} resources/winapp.rc)
#endif(WIN32)

endif ()

# Resources
file (GLOB_RECURSE src_RESOURCES RELATIVE ${${PROJECT_NAME}_SOURCE_DIR} FOLLOW_SYMLINKS "*.qrc")
set (${PROJECT_NAME}_RESOURCES ${${PROJECT_NAME}_RESOURCES} ${src_RESOURCES} PARENT_SCOPE)
