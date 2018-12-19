include_directories( ${GTEST_INCLUDE_DIRS} )

include_directories( ${UNITTESTING_INCLUDE_DIRS} )

file( GLOB TEST_CPP_SOURCES *.cpp )

add_executable ( unittest unittest_1.cpp)
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

target_link_libraries( unittest ${GTEST_LIBRARIES} pthread forwardModel_library)
install( TARGETS unittest DESTINATION bin)

