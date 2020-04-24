add_library(step_and_direction_reconstructor_library
    StepAndDirectionReconstructor.cpp                   ../include/StepAndDirectionReconstructor.h
    StepAndDirectionReconstructorInputCardReader.cpp    ../include/StepAndDirectionReconstructorInputCardReader.h
                                                        ../include/ReconstructorParameters.h
                                                        ../include/StepAndDirectionReconstructorInput.h)


set( LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_RECONSTRUCTOR_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/stepAndDirection/include CACHE INTERNAL "" )
target_include_directories(step_and_direction_reconstructor_library PUBLIC ${LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_RECONSTRUCTOR_INVERSION})

target_link_libraries(
    step_and_direction_reconstructor_library
    step_and_direction_interface_library
    core_library)

