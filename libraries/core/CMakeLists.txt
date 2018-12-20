file(GLOB CPP_SOURCES_CORE src/*.cpp)
file(GLOB H_HEADERS_CORE include/*.h)

set( LIBRARY_INCLUDE_DIRS_CORE "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

find_package (Eigen3 REQUIRED)
if (Eigen3_FOUND)
    message("Eigen found in core")
endif()
add_library( core_library ${CPP_SOURCES_CORE} ${H_HEADERS_CORE} )
target_link_libraries (core_library Eigen3::Eigen)

