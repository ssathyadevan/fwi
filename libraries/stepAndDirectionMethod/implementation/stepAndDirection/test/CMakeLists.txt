# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(../include)
include_directories(../testutilities/include)
include_directories(../../tests)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/io/include)

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")

# Build the tests!
build_test(TARGET StepAndDirectionReconstructorTest SOURCE StepAndDirectionReconstructorTest.cpp LIBRARIES step_and_direction_reconstructor_library direction_calculator_library  test_utils_library step_size_calculator_library)
build_test(TARGET StepAndDirectionReconstructorCardReaderTest SOURCE StepAndDirectionReconstructorCardReaderTest.cpp LIBRARIES step_and_direction_reconstructor_library)


