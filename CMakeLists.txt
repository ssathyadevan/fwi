project( FWI )
cmake_minimum_required( VERSION 2.8.0 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

add_subdirectory( libraries )

include_directories( ${LIBRARY_INCLUDE_DIRS} )

add_executable( FWI main.cpp )


target_link_libraries( FWI inversion_library )
