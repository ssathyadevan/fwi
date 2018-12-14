include_directories( ${GTEST_INCLUDE_DIRS} )

include_directories( ${UNITTESTING_INCLUDE_DIRS} )

file( GLOB TEST_CPP_SOURCES *.cpp )
build_tests( FILES ${TEST_CPP_SOURCES}
             LIBS ${GTEST_BOTH_LIBRARIES} UNITTESTING )
