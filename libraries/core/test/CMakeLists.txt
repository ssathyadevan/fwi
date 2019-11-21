# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(../include)

# Template function for building a test
function( build_test )
    set( PREFIX "TEST" )
    set( OPTIONS )
    set( ONE_VALUE_ARGS TARGET SOURCE)
    set( MULTI_VALUE_ARGS LIBRARIES)
    cmake_parse_arguments( "${PREFIX}" "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

    add_executable( ${TEST_TARGET} ${TEST_SOURCE} )
    target_link_libraries( ${TEST_TARGET} ${GTEST_BOTH_LIBRARIES} pthread ${TEST_LIBRARIES} )
    gtest_discover_tests( ${TEST_TARGET} )

    install(TARGETS ${TEST_TARGET} DESTINATION bin/test)
endfunction( build_test )

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")

# Build the tests!
build_test(TARGET grid2DTest SOURCE grid2DTest.cpp LIBRARIES core_library)
build_test(TARGET pressureFieldSerialTest SOURCE pressureFieldSerialTest.cpp LIBRARIES core_library)
build_test(TARGET pressureFieldComplexSerialTest SOURCE pressureFieldComplexSerialTest.cpp LIBRARIES core_library)
build_test(TARGET greensSerialTest SOURCE greensSerialTest.cpp LIBRARIES core_library)
build_test(TARGET sourcesTest SOURCE sourcesTest.cpp LIBRARIES core_library)