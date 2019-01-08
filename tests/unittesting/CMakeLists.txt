include_directories( ${GTEST_INCLUDE_DIRS} )
add_executable ( unittest unittest_1.cpp)
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

target_link_libraries( unittest ${GTEST_LIBRARIES} pthread)

gtest_discover_tests( unittest )

install( TARGETS unittest DESTINATION bin)

