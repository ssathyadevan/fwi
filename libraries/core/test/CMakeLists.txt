# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(../include)
include_directories(../testutilities)
include_directories(../testutilities/include)
include_directories(../../tests)


# Template function for building a test
function( build_test )
    set( PREFIX "TEST" )
    set( OPTIONS )
    set( ONE_VALUE_ARGS TARGET SOURCE)
    set( MULTI_VALUE_ARGS LIBRARIES)
    cmake_parse_arguments( "${PREFIX}" "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

    add_executable( ${TEST_TARGET} ${TEST_SOURCE} )
    target_link_libraries( ${TEST_TARGET} GTest::Main ${TEST_LIBRARIES} )
    gtest_discover_tests( ${TEST_TARGET} )

    install(TARGETS ${TEST_TARGET} DESTINATION bin/test)
endfunction( build_test )

SET(build_test  "${build_test}" CACHE INTERNAL "build_test")

# Build the tests!
build_test(TARGET grid2DTest SOURCE grid2DTest.cpp LIBRARIES core_library)
build_test(TARGET dataGrid2DTest SOURCE dataGrid2DTest.cpp LIBRARIES core_library)
build_test(TARGET complexDataGrid2DTest SOURCE complexDataGrid2DTest.cpp LIBRARIES core_library)
build_test(TARGET greensSerialTest SOURCE greensSerialTest.cpp LIBRARIES core_library)
build_test(TARGET sourcesTest SOURCE sourcesTest.cpp LIBRARIES core_library)
build_test(TARGET receiversTest SOURCE receiversTest.cpp LIBRARIES core_library)
build_test(TARGET contractionTest SOURCE contractionTest.cpp LIBRARIES core_library)
build_test(TARGET forwardModelInterfaceTest SOURCE forwardModelInterfaceTest.cpp LIBRARIES core_library)
build_test(TARGET genericInputCardReaderTest SOURCE genericInputCardReaderTest.cpp LIBRARIES core_library)




