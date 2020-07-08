# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
# Might have to include core library as well.
include_directories( ../include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/io/include )
include_directories( ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
include_directories( ${CMAKE_SOURCE_DIR}/libraries/utilities/include)

#Build the tests!
build_test(TARGET integralForwardModelTest SOURCE integralForwardModelInputCardReaderTest.cpp LIBRARIES integral_forward_model_library io_library stdc++fs)
