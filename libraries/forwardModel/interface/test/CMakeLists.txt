# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories(../include)
include_directories(../testutilities/include)

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
build_test(TARGET forwardModelInterfaceTest SOURCE forwardModelInterfaceTest.cpp LIBRARIES test_utils_library forward_model_interface_library)