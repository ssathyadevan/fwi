set(CPP_SOURCES_AND_H_HEADERS 
    chiIntegerVisualisation.cpp         ../include/chiIntegerVisualisation.h 
    contraction.cpp                     ../include/contraction.h
    cpuClock.cpp                        ../include/cpuClock.h
    createChiCSV.cpp                    ../include/createChiCSV.h
    csvReader.cpp                       ../include/csvReader.h
    forwardModelInterface.cpp           ../include/forwardModelInterface.h
                                        ../include/freq.h
    frequenciesGroup.cpp                ../include/frequenciesGroup.h
                                        ../include/genericInput.h
    genericInputCardReader.cpp          ../include/genericInputCardReader.h
    greensFunctions.cpp                 ../include/greensFunctions.h
    greensSerial.cpp                    ../include/greensSerial.h
    grid2D.cpp                          ../include/grid2D.h
    inputCardReader.cpp                 ../include/inputCardReader.h
                                        ../include/inversionInterface.h
                                        ../include/json.h
                                        ../include/nSourcesReceivers.h
                                        ../include/pressureField.h
    pressureFieldComplex.cpp            ../include/pressureFieldComplex.h
    pressureFieldComplexSerial.cpp      ../include/pressureFieldComplexSerial.h
    pressureFieldSerial.cpp             ../include/pressureFieldSerial.h
    progressBar.cpp                     ../include/progressBar.h
    receivers.cpp                       ../include/receivers.h
    sources.cpp                         ../include/sources.h
                                        ../include/utilityFunctions.h)

if (MPI_FOUND)
    set(CPP_SOURCES_AND_H_HEADERS ${CPP_SOURCES_AND_H_HEADERS} cpuClockMPI.cpp ../include/cpuClockMPI.h)
endif(MPI_FOUND)

add_library(core_library ${CPP_SOURCES_AND_H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_CORE ${CMAKE_SOURCE_DIR}/libraries/core/include CACHE INTERNAL "" )
target_include_directories(core_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CORE})

target_link_libraries(core_library Eigen3::Eigen utilities_library)