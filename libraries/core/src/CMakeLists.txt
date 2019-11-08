file(GLOB SOURCES_CORE *.cpp)
file(GLOB HEADERS_CORE ../include/*.h)

add_library(core_library ${SOURCES_CORE} ${HEADERS_CORE})

set( LIBRARY_INCLUDE_DIRS_CORE ${CMAKE_SOURCE_DIR}/libraries/core/include CACHE INTERNAL "" )
target_include_directories(core_library PUBLIC ${LIBRARY_INCLUDE_DIRS_CORE})

target_link_libraries(core_library Eigen3::Eigen)