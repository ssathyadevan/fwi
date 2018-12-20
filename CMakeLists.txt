cmake_minimum_required( VERSION 3.5.2 )

project( FWI )

# sets the output directory for all the executables generated to "runtime/bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime/bin )

# sets the output directory for all the static libraries generated to "runtime/lib"
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime/lib )

# sets the output directory for all the  generated to "runtime/lib"
set(CMAKE_MODULE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/runtime/lib )  

# if no DCMAKE_INSTALL_PREFIX is provided as an argument, then all the exectuables are copied to the install folder by default
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
   set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install )
endif()

find_package( GTest REQUIRED )

# display all compiler warnings if compiler is gnu
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif()

# display all compiler warnings if compiler is some version of Microsoft Visual C
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")

find_package (Eigen3 REQUIRED)

if (Eigen3_FOUND)
    message("Eigen found")
endif()

add_subdirectory( libraries )
add_subdirectory( applications )
add_subdirectory( tests )
