# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories(../include)

# Build the tests!
build_test(TARGET FDTest SOURCE finiteDifferenceForwardModelInputCardReaderTest.cpp LIBRARIES finite_difference_forward_model_library)
build_test(TARGET Helmholtz2DTest SOURCE helmholtz2dtest.cpp LIBRARIES finite_difference_forward_model_library)
