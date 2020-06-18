set(CPP_SOURCES_AND_H_HEADERS 
    contraction.cpp                     ../include/contraction.h
    forwardModelInterface.cpp           ../include/forwardModelInterface.h
        ../include/freqInfo.h
    frequenciesGroup.cpp                ../include/frequenciesGroup.h
    greensFunctions.cpp                 ../include/greensFunctions.h
    greensSerial.cpp                    ../include/greensSerial.h
    grid2D.cpp                          ../include/grid2D.h
                                        ../include/inversionInterface.h
    complexDataGrid2D.cpp               ../include/complexDataGrid2D.h
    dataGrid2D.cpp                      ../include/dataGrid2D.h
    receivers.cpp                       ../include/receivers.h
    sources.cpp                         ../include/sources.h)

add_library(core_library ${CPP_SOURCES_AND_H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_CORE ${CMAKE_SOURCE_DIR}/libraries/core/include CACHE INTERNAL "" )
target_include_directories(core_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CORE} ${CMAKE_SOURCE_DIR}/libraries/io/include)

target_link_libraries(core_library Eigen3::Eigen utilities_library)
target_link_libraries(core_library test_utils_library)
