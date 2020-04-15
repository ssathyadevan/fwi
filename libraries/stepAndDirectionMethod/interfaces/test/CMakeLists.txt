# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)

# Build the tests!
build_test(TARGET DirectionCalculatorTest SOURCE DirectionCalculatorTest.cpp LIBRARIES step_and_direction_interface_library test_step_and_direction_utils_library utilities_library)
build_test(TARGET StepSizeCalculatorTest SOURCE StepSizeCalculatorTest.cpp LIBRARIES step_and_direction_interface_library test_step_and_direction_utils_library utilities_library)
