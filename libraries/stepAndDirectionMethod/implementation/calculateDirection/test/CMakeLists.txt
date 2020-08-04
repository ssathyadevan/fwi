# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/testutilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${GMOCK_INCLUDE_DIRS})
# Build the tests!
build_test(TARGET ConjugateGradientDirectionCalculatorTest SOURCE ConjugateGradientDirectionCalculatorTest.cpp LIBRARIES direction_calculator_library test_utils_library ${GMOCK_BOTH_LIBRARIES})
build_test(TARGET GradientDescentDirectionCalculatorTest SOURCE GradientDescentDirectionCalculatorTest.cpp LIBRARIES direction_calculator_library test_utils_library )
build_test(TARGET DummyDirectionCalculatorTest SOURCE DummyDirectionCalculatorTest.cpp LIBRARIES direction_calculator_library test_utils_library )
# Rename the TARGET and SOURCE with own DirectionCalculator name
#build_test(TARGET YourOwnDirectionCalculatorTest SOURCE YourOwnCalculatorTest.cpp LIBRARIES direction_calculator_library test_utils_library forward_model_interface_library)
