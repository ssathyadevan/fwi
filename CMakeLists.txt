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

#Set project path define
add_definitions(-DFWI_PROJECT_DIR="${CMAKE_SOURCE_DIR}")

#Check MPI/BOOST
find_package(MPI)
if(MPI_FOUND)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -ldl -lboost_mpi -lboost_serialization -fopenmp -Wall -Wextra -Wpedantic")
    add_definitions(-DMPI)
    include_directories(SYSTEM ${MPI_INCLUDE_PATH})
    find_package(Boost REQUIRED COMPONENTS mpi serialization)
    link_libraries(${Boost_LIBRARIES})
    link_libraries(${MPI_CXX_LIBRARIES})
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fopenmp -Wall -Wextra -Wpedantic -Werror -Werror=vla")
endif()

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

find_package(Qt5 COMPONENTS Core Widgets Qml Quick)
if (Qt5_FOUND)
    add_subdirectory( gui )
endif()

#Add cppcheck target
include(${CMAKE_SOURCE_DIR}/cmake/add_cppcheck_target.cmake)
find_program(CPPCHECK_FOUND NAMES cppcheck)
#TODO: Fix build system generation so that "libraries" is build as a single target and remove globing after
file(GLOB_RECURSE LIBRARY_FILES ${CMAKE_SOURCE_DIR}/libraries/*.h ${CMAKE_SOURCE_DIR}/libraries/*.cpp)
if(CPPCHECK_FOUND)
	set(CPPCHECK_PARAMETERS --enable=all --xml --xml-version=2 --std=c++11 --language=c++)
	add_cppcheck_target(OUTPUTFILE ${CMAKE_SOURCE_DIR}/staticAnalysisCppcheckReport.xml INPUTFILES ${LIBRARY_FILES} PARAMETERS ${CPPCHECK_PARAMETERS})
endif()


