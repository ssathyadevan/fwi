add_executable ( unittest unittest_1.cpp)
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

target_link_libraries( unittest ${GTEST_LIBRARIES} pthread)

install( TARGETS unittest DESTINATION bin)

