# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)

# Build the tests!
build_test(TARGET ConjugateGradientDirectionCalculatorTest SOURCE ConjugateGradientDirectionCalculatorTest.cpp LIBRARIES direction_calculator_library)
build_test(TARGET GradientDescentDirectionCalculatorTest SOURCE GradientDescentDirectionCalculatorTest.cpp LIBRARIES direction_calculator_library)
