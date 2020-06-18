include_directories(../include)

set(CPP_SOURCES_AND_HEADERS_STEP_AND_DIRECTION_UTILITIES
    DirectionCalculatorMock.cpp              include/DirectionCalculatorMock.h)

add_library(test_step_and_direction_utils_library ${CPP_SOURCES_AND_HEADERS_STEP_AND_DIRECTION_UTILITIES})

set( LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_UTILITIES ${CMAKE_SOURCE_DIR}/libraries/stepAndDirectionMethod/interfaces/testutilities/include CACHE INTERNAL "" )

target_include_directories(test_step_and_direction_utils_library PUBLIC ${LIBRARY_INCLUDE_DIRS_STEP_AND_DIRECTION_UTILITIES} PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)

target_link_libraries(test_step_and_direction_utils_library core_library)
target_link_libraries(test_step_and_direction_utils_library step_and_direction_interface_library)

