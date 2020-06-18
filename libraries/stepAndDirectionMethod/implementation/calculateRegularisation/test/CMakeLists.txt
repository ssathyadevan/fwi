# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)

# Build the tests!
build_test(TARGET ConjugateGradientWithRegularisationCalculatorTest SOURCE ConjugateGradientWithRegularisationCalculatorTest.cpp LIBRARIES conjugate_gradient_with_regularisation_calculator_library utilities_library test_utils_library forward_model_interface_library)

