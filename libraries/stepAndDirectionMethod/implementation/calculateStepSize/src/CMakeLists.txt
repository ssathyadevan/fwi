add_library(calculate_step_size_library
    calculateFixedSize.cpp                    ../include/calculateFixedSize.h
    calculateVariableSize.cpp                    ../include/calculateVariableSize.h
    calculateGamma.cpp                    ../include/calculateGamma.h)

set( LIBRARY_INCLUDE_DIRS_CALCULATE_STEP_SIZE_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/calculateStepSize/include CACHE INTERNAL "" )
target_include_directories(calculate_step_size_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CALCULATE_STEP_SIZE_INVERSION})

target_link_libraries(calculate_step_size_library
    core_library)

