include_directories(../include)

set(CPP_SOURCES_AND_H_HEADERS_TEST_UTILS
    forwardmodelinterfacemock.cpp              include/forwardmodelinterfacemock.h)

add_library(test_utils_library ${CPP_SOURCES_AND_H_HEADERS_TEST_UTILS})

set( LIBRARY_INCLUDE_DIRS_CORE ${CMAKE_SOURCE_DIR}/libraries/core/testutilities/include CACHE INTERNAL "" )
target_include_directories(test_utils_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CORE})

target_link_libraries(test_utils_library Eigen3::Eigen utilities_library)
target_link_libraries(test_utils_library core_library)
