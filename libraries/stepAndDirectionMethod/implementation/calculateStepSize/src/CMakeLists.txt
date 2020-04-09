add_library(calculate_step_size_library
    FixedStepSizeCalculator.cpp                    ../include/FixedStepSizeCalculator.h
    VariableStepSizeCalculator.cpp                    ../include/VariableStepSizeCalculator.h
    GammaStepSizeCalculator.cpp                    ../include/GammaStepSizeCalculator.h)

set( LIBRARY_INCLUDE_DIRS_CALCULATE_STEP_SIZE_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/calculateStepSize/include CACHE INTERNAL "" )
target_include_directories(calculate_step_size_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CALCULATE_STEP_SIZE_INVERSION})

target_link_libraries(calculate_step_size_library
	step_and_direction_interface_library
    	core_library)

