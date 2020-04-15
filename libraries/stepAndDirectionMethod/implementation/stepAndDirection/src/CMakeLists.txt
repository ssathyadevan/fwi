add_library(step_and_direction_implementation_library
    StepAndDirectionInterface.cpp            ../include/StepAndDirectionInterface.h
    DirectionInput.cpp                       ../include/DirectionInput.h)


set( LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_IMPLEMENTATION_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/stepAndDirection/include CACHE INTERNAL "" )
target_include_directories(step_and_direction_implementation_library PUBLIC ${LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_IMPLEMENTATION_INVERSION})

target_link_libraries(
    step_and_direction_implementation_library
    step_and_direction_interface_library
    core_library)

