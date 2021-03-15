include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/performance/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/performance/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/forwardModel/base/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/io/include)
include_directories(include)

add_executable( FullFWI main.cpp )
target_include_directories(FullFWI PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
target_link_libraries( FullFWI integral_forward_model_library core_library integral_forward_model_library io_library performance_library)
install( TARGETS FullFWI DESTINATION bin)

