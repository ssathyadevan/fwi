# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(../include)
include_directories(../testutilities/include)
include_directories(../../tests)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${GMOCK_INCLUDE_DIRS})

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")
# Build the tests!

build_test(TARGET DirectionCalculatorTest SOURCE DirectionCalculatorTest.cpp LIBRARIES step_and_direction_interface_library test_step_and_direction_utils_library test_utils_library ${GMOCK_BOTH_LIBRARIES})
build_test(TARGET StepSizeCalculatorTest SOURCE StepSizeCalculatorTest.cpp LIBRARIES step_and_direction_interface_library test_step_and_direction_utils_library ${GMOCK_BOTH_LIBRARIES})

