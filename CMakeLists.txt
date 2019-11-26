cmake_minimum_required( VERSION 3.10 )

project( FWI )

# We enfore c++ 17 or later and disable any 
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF)


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

find_package(MPI)

if ( UNIX )
if(MPI_FOUND)
    set(CMAKE_CXX_COMPILER /usr/bin/mpicxx)
else(MPI_FOUND)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif(MPI_FOUND)
endif( )
# Setup consistent and strict warning flags across compilers
if ( MSVC )
    # Disable some of the security warnings
    message( STATUS "    Additional compiler flag to disable 'unsafe methods' security-warnings:" )
    message( STATUS "        '-D_CRT_SECURE_NO_WARNINGS' -D_SCL_SECURE_NO_WARNINGS" )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS )

    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
elseif ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Werror=vla")
endif()

enable_testing()

# We required both GTest and Eigen3 
find_package(GTest REQUIRED)
find_package (Eigen3 REQUIRED)

add_subdirectory( libraries )
add_subdirectory( applications )