include_directories( ${LIBRARY_INCLUDE_DIRS_FINITE_DIFFERENCE_FORWARDMODEL} )
include_directories( ${LIBRARY_INCLUDE_DIRS_CORE} )

add_executable( FWI_PreProcess_Finite_Difference  main.cpp )
target_link_libraries( FWI_PreProcess_Finite_Difference finite_difference_forwardModel_library core_library)
install( TARGETS FWI_PreProcess_Finite_Difference DESTINATION bin)

