# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(../include)
include_directories(../../utilities/include)

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")

# Build the tests!
build_test(TARGET genericInputCardReaderTest SOURCE genericInputCardReaderTest.cpp LIBRARIES io_library)




