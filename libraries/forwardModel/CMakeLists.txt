file(GLOB CPP_SOURCES_FORWARDMODEL src/*.cpp)
file(GLOB H_HEADERS_FORWARDMODEL include/*.h)

set( LIBRARY_INCLUDE_DIRS_FORWARDMODEL "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

add_library( forwardModel_library ${CPP_SOURCES_FORWARDMODEL} ${H_HEADERS_FORWARDMODEL} )
target_link_libraries (forwardModel_library Eigen3::Eigen)

