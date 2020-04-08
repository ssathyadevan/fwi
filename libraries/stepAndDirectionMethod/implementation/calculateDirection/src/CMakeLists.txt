add_library(direction_calculator_library
    conjugateGradientDirectionCalculator.cpp                    ../include/conjugateGradientDirectionCalculator.h
    gradientDescentDirectionCalculator.cpp                     ../include/gradientDescentDirectionCalculator.h)

set( LIBRARY_INCLUDE_DIRS_DIRECTION_CALCULATOR_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/calculateDirection/include CACHE INTERNAL "" )
target_include_directories(direction_calculator_library PUBLIC ${LIBRARY_INCLUDE_DIRS_DIRECTION_CALCULATOR_INVERSION})

target_link_libraries(
    direction_calculator_library
    core_library
    step_and_direction_interface_library)
