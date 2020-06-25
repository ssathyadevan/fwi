
include_directories( ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )
include_directories(${CMAKE_SOURCE_DIR}/libraries/utilities/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/performance/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/io/include)

add_executable( FWI_Process_Gradient_Descent main.cpp )
target_include_directories(FWI_Process_Gradient_Descent PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include)
target_link_libraries( FWI_Process_Gradient_Descent gradient_descent_inversion_library integral_forward_model_library core_library forward_model_interface_library io_library performance_library)
install( TARGETS FWI_Process_Gradient_Descent DESTINATION bin)
