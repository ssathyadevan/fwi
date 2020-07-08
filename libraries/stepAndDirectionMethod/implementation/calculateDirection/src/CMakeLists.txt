add_library(direction_calculator_library
    ConjugateGradientDirectionCalculator.cpp                   ../include/ConjugateGradientDirectionCalculator.h
    GradientDescentDirectionCalculator.cpp                     ../include/GradientDescentDirectionCalculator.h
    DummyDirectionCalculator.cpp                               ../include/DummyDirectionCalculator.h
#Update with your own DirectionCalculator name
#    YourOwnDirectionCalculator.cpp                             ../include/YourOwnDirectionCalculator.h
    )

set( LIBRARY_INCLUDE_DIRS_DIRECTION_CALCULATOR_INVERSION ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/implementation/calculateDirection/include CACHE INTERNAL "" )
target_include_directories(direction_calculator_library PUBLIC ${LIBRARY_INCLUDE_DIRS_DIRECTION_CALCULATOR_INVERSION} PRIVATE ${CMAKE_SOURCE_DIR}/libraries/io/include PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)

target_link_libraries(
    direction_calculator_library
    core_library
    step_and_direction_interface_library)
