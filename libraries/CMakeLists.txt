file(GLOB CPP_SOURCES src/*.cpp)
file(GLOB H_HEADERS include/*.h)

file(GLOB CPP_SOURCES_FORWARDMODEL srcForwardModel/*.cpp)
file(GLOB H_HEADERS_FORWARDMODEL includeForwardModel/*.h)


set( LIBRARY_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
set( LIBRARY_INCLUDE_DIRS_FORWARDMODEL "${CMAKE_CURRENT_SOURCE_DIR}/includeForwardModel" CACHE INTERNAL "" )

include_directories( ${LIBRARY_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )

message( STATUS "TESTR: ${CPP_SOURCES}")

add_library( inversion_library ${CPP_SOURCES} ${H_HEADERS} )
add_library( forwardModel_library ${CPP_SOURCES_FORWARDMODEL} ${H_HEADERS_FORWARDMODEL} )

