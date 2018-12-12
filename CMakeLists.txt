project( FWI )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime/bin ) #executables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime/lib ) #static-libraries
set(CMAKE_MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime/lib )  

if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
   set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install )
endif()

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
