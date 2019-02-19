file(GLOB CPP_SOURCES src/*.cpp)
file(GLOB H_HEADERS include/*.h)

set( LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
include_directories( ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

add_library( random_inversion_library ${CPP_SOURCES} ${H_HEADERS} )
target_link_libraries (random_inversion_library Eigen3::Eigen)
