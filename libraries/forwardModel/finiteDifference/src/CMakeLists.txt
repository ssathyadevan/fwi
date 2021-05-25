add_library(finite_difference_forward_model_library
    FiniteDifferenceForwardModel.cpp                    ../include/FiniteDifferenceForwardModel.h
                                                        ../include/FiniteDifferenceForwardModelInput.h
    FiniteDifferenceForwardModelInputCardReader.cpp     ../include/FiniteDifferenceForwardModelInputCardReader.h
    FiniteDifferenceGrid2D.cpp                          ../include/FiniteDifferenceGrid2D.h
    Helmholtz2D.cpp                                     ../include/Helmholtz2D.h
    FiniteDifferenceForwardModelParallel.cpp            ../include/FiniteDifferenceForwardModelParallel.h
    FiniteDifferenceForwardModelParallelMPI.cpp         ../include/FiniteDifferenceForwardModelParallelMPI.h
                                                        ../include/PmlWidthFactor.h
                                                        ../include/SourceParameter.h)

set( LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL ${CMAKE_SOURCE_DIR}/libraries/forwardModel/finiteDifference/include CACHE INTERNAL "" )

target_include_directories(finite_difference_forward_model_library 
PUBLIC ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL}
 PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include 
PRIVATE ${CMAKE_SOURCE_DIR}/libraries/utilities/include)
target_link_libraries(finite_difference_forward_model_library core_library)

#MPI
find_package(MPI) #make it REQUIRED, if you want
include_directories(SYSTEM ${MPI_INCLUDE_PATH})
 target_link_libraries(finite_difference_forward_model_library ${MPI_CXX_LIBRARIES})
