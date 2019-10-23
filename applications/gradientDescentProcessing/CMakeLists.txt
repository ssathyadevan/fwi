
include_directories( ${LIBRARY_INCLUDE_DIRS_GRADIENT_DESCENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL} )

add_executable( FWI_Process_Gradient_Descent main.cpp )
target_link_libraries( FWI_Process_Gradient_Descent gradient_descent_inversion_library integral_forwardModel_library core_library)
install( TARGETS FWI_Process_Gradient_Descent DESTINATION bin)
