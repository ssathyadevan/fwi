# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/io/include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)

# Build the tests!
build_test(TARGET FDTest SOURCE finiteDifferenceForwardModelInputCardReaderTest.cpp LIBRARIES finite_difference_forward_model_library io_library)
build_test(TARGET Helmholtz2DTest SOURCE helmholtz2dtest.cpp LIBRARIES finite_difference_forward_model_library)
