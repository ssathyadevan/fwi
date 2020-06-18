set(CPP_SOURCES_AND_H_HEADERS 
	chiIntegerVisualisation.cpp		../include/chiIntegerVisualisation.h
	cpuClock.cpp    			    ../include/cpuClock.h
	csvReader.cpp     			    ../include/csvReader.h
	progressBar.cpp       			../include/progressBar.h
                                  	../include/utilityFunctions.h)

if (MPI_FOUND)
    set(CPP_SOURCES_AND_H_HEADERS ${CPP_SOURCES_AND_H_HEADERS} cpuClockMPI.cpp ../include/cpuClockMPI.h)
endif(MPI_FOUND)

add_library(utilities_library ${CPP_SOURCES_AND_H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_UTILITIES ${CMAKE_SOURCE_DIR}/libraries/utilities/include CACHE INTERNAL "" )
target_include_directories(utilities_library PUBLIC ${LIBRARY_INCLUDE_DIRS_UTILITIES} PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include)

target_link_libraries(utilities_library PRIVATE Eigen3::Eigen io_library)
