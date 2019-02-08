include_directories( ${LIBRARY_INCLUDE_DIRS_CONJUGATE_GRADIENT_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_FORWARDMODEL} )

add_executable( FWI_Process_Random_Inversion main.cpp )
target_link_libraries( FWI_Process_Random_Inversion conjugate_gradient_inversion_library random_inversion_library forwardModel_library core_library)
install( TARGETS FWI_Process_Random_Inversion DESTINATION bin)
