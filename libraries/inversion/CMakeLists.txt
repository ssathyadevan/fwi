file(GLOB CPP_SOURCES src/*.cpp)
file(GLOB H_HEADERS include/*.h)

set( LIBRARY_INCLUDE_DIRS_INVERSION "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
include_directories( ${LIBRARY_INCLUDE_DIRS_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )


#find_package (Eigen3 REQUIRED)
add_library( inversion_library ${CPP_SOURCES} ${H_HEADERS} )
target_link_libraries (inversion_library Eigen3::Eigen)
