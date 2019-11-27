set(CPP_SOURCES_AND_H_HEADERS 
    log.cpp			../include/log.h)

add_library(utilities_library ${CPP_SOURCES_AND_H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_UTILITIES ${CMAKE_SOURCE_DIR}/libraries/utilities/include CACHE INTERNAL "" )
target_include_directories(utilities_library PUBLIC ${LIBRARY_INCLUDE_DIRS_UTILITIES})

target_link_libraries(utilities_library Eigen3::Eigen)
