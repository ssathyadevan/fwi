
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
endfunction( build_test )
SET(CMAKE_BUILD_TYPE Debug)

add_subdirectory( utilities )
add_subdirectory( core )
add_subdirectory( forwardModel )
add_subdirectory( inversion )
add_subdirectory( factory )
add_subdirectory( forwardModelContainer )
add_subdirectory( stepAndDirectionMethod )
add_subdirectory( io )
add_subdirectory( performance )
