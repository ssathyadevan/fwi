# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )

# Might need core and/or forward & inverison libraries too.
include_directories(../include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${GMOCK_INCLUDE_DIRS})

# Build the tests!
build_test(TARGET factoryTest SOURCE factoryTest.cpp LIBRARIES factory_library test_utils_library ${GMOCK_BOTH_LIBRARIES})
