include_directories( ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARDMODEL} )

add_executable( FWI_Process main.cpp )
target_link_libraries( FWI_Process conjugate_gradient_inversion_library integral_forwardModel_library core_library)
install( TARGETS FWI_Process DESTINATION bin)
