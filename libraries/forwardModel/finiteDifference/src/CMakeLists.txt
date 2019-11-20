add_library(finite_difference_forward_model_library
    finiteDifferenceForwardModel.cpp                    ../include/finiteDifferenceForwardModel.h
                                                        ../include/finiteDifferenceForwardModelInput.h
    finiteDifferenceForwardModelInputCardReader.cpp     ../include/finiteDifferenceForwardModelInputCardReader.h
    finiteDifferenceGrid2D.cpp                          ../include/finiteDifferenceGrid2D.h
    helmholtz2D.cpp                                      ../include/helmholtz2D.h
                                                        ../include/inputStructs.h)

set( LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL ${CMAKE_SOURCE_DIR}/libraries/forwardModel/finiteDifference/include CACHE INTERNAL "" )
target_include_directories(finite_difference_forward_model_library PUBLIC ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL})

target_link_libraries(finite_difference_forward_model_library core_library)