# Include libraries
include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARD_MODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION} )

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
endfunction( build_test )

# Build the tests!
build_test( TARGET InputTest SOURCE InputCardReaderTests.cpp LIBRARIES core_library integral_forwardModel_library random_inversion_library gradient_descent_inversion_library finite_difference_forward_model_library conjugate_gradient_inversion_library )
build_test( TARGET CoreTest SOURCE unittests.cpp LIBRARIES core_library)

install( TARGETS InputTest CoreTest DESTINATION bin)