# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${CMAKE_SOURCE_DIR}/libraries/io/include )
include_directories( ${CMAKE_SOURCE_DIR}/libraries/utilities/include )
include_directories(../include)

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")

# Build the tests!
build_test(TARGET grid2DTest SOURCE grid2DTest.cpp LIBRARIES core_library)
build_test(TARGET dataGrid2DTest SOURCE dataGrid2DTest.cpp LIBRARIES core_library)
build_test(TARGET greensSerialTest SOURCE greensSerialTest.cpp LIBRARIES core_library)
build_test(TARGET sourcesTest SOURCE sourcesTest.cpp LIBRARIES core_library)
build_test(TARGET receiversTest SOURCE receiversTest.cpp LIBRARIES core_library)
build_test(TARGET contractionTest SOURCE contractionTest.cpp LIBRARIES core_library)
build_test(TARGET CostFunctionCalculatorTest SOURCE CostFunctionCalculatorTest.cpp LIBRARIES core_library)




