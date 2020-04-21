add_library(step_size_calculator_library
    FixedStepSizeCalculator.cpp                    ../include/FixedStepSizeCalculator.h
    VariableStepSizeCalculator.cpp                 ../include/VariableStepSizeCalculator.h
    BorzilaiBorweinStepSizeCalculator.cpp          ../include/BorzilaiBorweinStepSizeCalculator.h
    LinearStepSizeCalculator.cpp                   ../include/LinearStepSizeCalculator.h)

set( LIBRARY_INCLUDE_DIRS_CALCULATE_STEP_SIZE_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/calculateStepSize/include CACHE INTERNAL "" )
target_include_directories(step_size_calculator_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CALCULATE_STEP_SIZE_INVERSION})

target_link_libraries(step_size_calculator_library
        core_library
        step_and_direction_interface_library)

