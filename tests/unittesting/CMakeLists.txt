include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )


function( build_test )
    set( PREFIX "TEST" )
    set( OPTIONS )
    set( ONE_VALUE_ARGS TARGET SOURCE)
    set( MULTI_VALUE_ARGS )
    cmake_parse_arguments( "${PREFIX}" "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN} )

    add_executable( ${TEST_TARGET} ${TEST_SOURCE} )
    target_link_libraries( ${TEST_TARGET} ${GTEST_BOTH_LIBRARIES} core_library pthread)
    gtest_discover_tests( ${TEST_TARGET} )
endfunction( build_test )

build_test( TARGET unittest SOURCE unittest_1.cpp)
build_test( TARGET genericinputtest SOURCE genericinputtest.cpp)


#add_executable ( unittest unittest_1.cpp)
#target_link_libraries( unittest ${GTEST_LIBRARIES} core_library pthread)
#gtest_discover_tests( unittest )

#add_executable( genericinputtest genericinputtest.cpp)
#target_link_libraries( genericinputtest ${GTEST_BOTH_LIBRARIES} core_library pthread)
#gtest_discover_tests( genericinputtest )

install( TARGETS unittest DESTINATION bin)

