# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)

# Build the tests!
#build_test(TARGET FixedStepSizeCalculatorTest SOURCE FixedStepSizeCalculatorTest.cpp LIBRARIES step_size_calculator_library utilities_library)

