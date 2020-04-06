include_directories( ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )

add_executable( FWI_Process main.cpp )
target_link_libraries( FWI_Process conjugate_gradient_inversion_library integral_forward_model_library core_library utilities_library)
install( TARGETS FWI_Process DESTINATION bin)
