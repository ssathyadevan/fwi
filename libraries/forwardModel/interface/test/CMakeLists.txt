# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories(../include)
include_directories(../testutilities/include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/io/include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")

# Build the tests!
build_test(TARGET forwardModelInterfaceTest SOURCE forwardModelInterfaceTest.cpp LIBRARIES test_utils_library forward_model_interface_library)
