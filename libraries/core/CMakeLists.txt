#file(GLOB CPP_SOURCES_CORE src/*.cpp)
#file(GLOB H_HEADERS_CORE include/*.h)
#file(GLOB CPP_TESTS_CORE test/*.cpp)

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")

# set( LIBRARY_INCLUDE_DIRS_CORE "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE INTERNAL "" )
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#add_library(core_library ${CPP_SOURCES_CORE})
#target_link_libraries (core_library Eigen3::Eigen)

#add_executable(testlib ${CPP_TESTS_CORE})
#target_link_libraries(testlib core_library)
#add_test(testlib testlib)

add_subdirectory(src)
add_subdirectory(test)