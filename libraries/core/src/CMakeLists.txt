file(GLOB CPP_SOURCES *.cpp)
file(GLOB H_HEADERS ../include/*.h)

add_library(core_library ${CPP_SOURCES} ${H_HEADERS})

set( LIBRARY_INCLUDE_DIRS_CORE ${CMAKE_SOURCE_DIR}/libraries/core/include CACHE INTERNAL "" )
target_include_directories(core_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CORE})

target_link_libraries(core_library Eigen3::Eigen)