file(GLOB CPP_SOURCES src/*.cpp)
file(GLOB H_HEADERS include/*.h)

set( LIBRARY_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )

include_directories( ${LIBRARY_INCLUDE_DIRS} )

message( STATUS "TESTR: ${CPP_SOURCES}")

add_library( inversion_library ${CPP_SOURCES} ${H_HEADERS} )
