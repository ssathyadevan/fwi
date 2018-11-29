project( FWI )

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()
cmake_minimum_required( VERSION 2.8.0 )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

add_subdirectory( libraries )
add_subdirectory( applications )


