file(GLOB CPP_SOURCES_INTEGRAL_FORWARDMODEL src/*.cpp)
file(GLOB H_HEADERS_INTEGRAL_FORWARDMODEL include/*.h)

set( LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

add_library( integral_forwardModel_library ${CPP_SOURCES_INTEGRAL_FORWARDMODEL} ${H_HEADERS_INTEGRAL_FORWARDMODEL} )
target_link_libraries (integral_forwardModel_library Eigen3::Eigen)

