include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/performance/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/io/include)

add_executable( FWI_PreProcess main.cpp )
target_include_directories(FWI_PreProcess PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include)
target_link_libraries( FWI_PreProcess integral_forward_model_library core_library integral_forward_model_library forward_model_interface_library io_library performance_library)
install( TARGETS FWI_PreProcess DESTINATION bin)

