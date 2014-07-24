# CMake tools
include (platform/collect_sources)

collect_sources (src_HEADERS src_SOURCES ${PROJECT_SOURCE_DIR})

# Headers
set (${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${src_HEADERS} PARENT_SCOPE)

# Sources
set (${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${src_SOURCES} PARENT_SCOPE)
