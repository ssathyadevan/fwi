project( FWI )
cmake_minimum_required( VERSION 2.8.0 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")


add_subdirectory( include )

include_directories( include )
#include_directories( src )

add_executable( FWI main.cpp )


target_link_libraries( FWI inversion_library )
