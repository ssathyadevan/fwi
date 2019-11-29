
include_directories( ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )

add_executable( FWI_Process_Gradient_Descent main.cpp )
target_link_libraries( FWI_Process_Gradient_Descent gradient_descent_inversion_library integral_forward_model_library core_library utilities_library)
install( TARGETS FWI_Process_Gradient_Descent DESTINATION bin)
