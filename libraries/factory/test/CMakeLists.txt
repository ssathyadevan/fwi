# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FACTORY} )
# Might need core and/or forward & inverison libraries too.
include_directories(../include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include)

# Build the tests!
build_test(TARGET factoryTest SOURCE factoryTest.cpp LIBRARIES step_and_direction_interface_library factory_library test_utils_library)
