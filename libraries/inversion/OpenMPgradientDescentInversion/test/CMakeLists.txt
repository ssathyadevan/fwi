# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION} )
# Might have to include core library as well.
include_directories(../include)

# Build the tests!
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
build_test(TARGET OpenMPgradientDescentInversionTest SOURCE OpenMPgradientDescentInversionTest.cpp LIBRARIES OpenMP_gradient_descent_inversion_library integral_forward_model_library core_library)
