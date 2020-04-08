add_library(step_and_direction_interface_library
    stepSizeInterface.cpp                    ../include/stepSizeInterface.h
    DirectionCalculator.cpp                     ../include/DirectionCalculator.h)

set( LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_INTERFACE_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/interfaces/include CACHE INTERNAL "" )
target_include_directories(step_and_direction_interface_library PUBLIC ${LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_INTERFACE_INVERSION})

target_link_libraries(
    step_and_direction_interface_library
    core_library)
