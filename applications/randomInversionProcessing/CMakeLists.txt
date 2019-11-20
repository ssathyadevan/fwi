include_directories( ${LIBRARY_INCLUDE_DIRS_RANDOM_INVERSION} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )
include_directories( ${LIBRARY_INCLUDE_DIRS_INTEGRAL_FORWARD_MODEL} )

add_executable( FWI_Process_Random_Inversion main.cpp )
target_link_libraries( FWI_Process_Random_Inversion random_inversion_library integral_forward_model_library core_library)
install( TARGETS FWI_Process_Random_Inversion DESTINATION bin)
