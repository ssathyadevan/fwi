include_directories( ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )

add_executable( FWI_Process_Random_Inversion main.cpp )
target_include_directories(FWI_Process_Random_Inversion PRIVATE ${CMAKE_SOURCE_DIR}/libraries/forwardModel/interface/include ${CMAKE_SOURCE_DIR}/libraries/inversion/interface/include)
target_link_libraries( FWI_Process_Random_Inversion random_inversion_library integral_forward_model_library core_library utilities_library forward_model_interface_library)
install( TARGETS FWI_Process_Random_Inversion DESTINATION bin)
