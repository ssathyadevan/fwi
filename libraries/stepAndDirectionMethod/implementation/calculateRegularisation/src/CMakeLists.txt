add_library(conjugate_gradient_with_regularisation_calculator_library
    ConjugateGradientWithRegularisationCalculator.cpp                   ../include/ConjugateGradientWithRegularisationCalculator.h
                                                                        ../include/ConjugateGradientWithRegularisationParametersInput.h
                                                                        ../include/DeltaAmplification.h
                                                                        ../include/ConjugateGradientRegularisationParameters.h)

set( LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_WITH_REGULARISATION_CALCULATOR_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/calculateRegularisation/include CACHE INTERNAL "" )
target_include_directories(conjugate_gradient_with_regularisation_calculator_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_WITH_REGULARISATION_CALCULATOR_INVERSION} PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include)

target_link_libraries(
    conjugate_gradient_with_regularisation_calculator_library
        core_library
        step_and_direction_reconstructor_library)

