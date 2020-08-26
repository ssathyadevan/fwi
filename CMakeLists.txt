cmake_minimum_required( VERSION 3.10 )
project( FWI )

#We enforce c++ 17 or later and disable any 
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF)

#Set install prefix
set( CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/FWIInstall )

#Ensure that MinGW dependencies are statically linked
if( WIN32 )
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic")
endif()

#Set gcc compiler flags
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -Werror=vla")

#Set project path define
add_definitions(-DFWI_PROJECT_DIR="${CMAKE_SOURCE_DIR}")

#We require Eigen3 
find_package (Eigen3 REQUIRED)

include(CTest)
include(GoogleTest)
include(${CMAKE_SOURCE_DIR}/cmake/build_test.cmake)

#Download gtest framework
add_subdirectory( cmake )

#Add gtest and gmock to project
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
				${CMAKE_BINARY_DIR}/googletest-build
				EXCLUDE_FROM_ALL)

add_subdirectory( libraries )
add_subdirectory( doc )
add_subdirectory( applications )

